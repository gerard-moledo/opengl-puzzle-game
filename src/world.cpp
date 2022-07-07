#include "world.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <algorithm>
#include <sstream>

#include "system.hpp"

void glfwMouseCallback(GLFWwindow* window, int button, int action, int mods)
{
    World* world = (World*) glfwGetWindowUserPointer(window);

    if (world->mode == Mode::play) return;
    
    if (action == GLFW_PRESS)
    {
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        glm::vec3 ray = world->GetMouseRay((float)mouseX, (float)mouseY);
        glm::vec3 eye = PolarToRect(world->eyePolar);
        glm::vec3 planeNormal = glm::vec3(0.0f, 1.0f, 0.0f);

        float rayDotNormal = glm::dot(ray, planeNormal);
        float t = -(glm::dot(eye, planeNormal) + 1) / rayDotNormal;

        glm::vec3 hit = eye + ray * t;
        Vector2i hitGrid { (int) roundf(hit.x / 2), (int) roundf(-hit.z / 2) };

        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
            {
                if (std::all_of(world->blocks.begin(), world->blocks.end(), [&](Block block) { return block.currentCell != hitGrid; }))
                {
                    world->player.currentCell = hitGrid;
                    world->player.targetCell = hitGrid;
                    world->player.worldPos = glm::vec3(hitGrid.x, 0.0f, hitGrid.y);
                }
            }
            else
            {
                bool occupied = false;
                for (Block& block : world->blocks)
                {
                    if (block.currentCell == hitGrid)
                    {
                        block.type = (BlockType)(((int)block.type + 1) % 3);

                        occupied = true;
                    }
                }

                if (!occupied)
                {
                    world->blocks.emplace_back(Block { BlockType::block, hitGrid });
                }
            }
        }

        if (button == GLFW_MOUSE_BUTTON_MIDDLE)
        {
            if (std::all_of(world->blocks.begin(), world->blocks.end(), [&](Block& block) { return block.currentCell != hitGrid; }))
            {
                world->player.currentCell = hitGrid;
                world->player.targetCell = hitGrid;
                world->player.worldPos = glm::vec3(hitGrid.x, 0.0f, hitGrid.y);
            }
        }

        if (button == GLFW_MOUSE_BUTTON_RIGHT)
        {
            auto begin = world->blocks.begin();
            auto end = world->blocks.end();
            world->blocks.erase(
                std::remove_if(begin, end, [&](Block& block) {
                    return block.currentCell == hitGrid;
                }), end
            );
        }
    }
}

void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    World* world = (World*) glfwGetWindowUserPointer(window);

    if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
    {
        world->mode = world->mode == Mode::play ? Mode::edit : Mode::play;
        if (world->mode == Mode::edit)
        {
            std::vector<UI> editText = world->CreateText("EDIT MODE", Vector2i { 20, System::height - 120 }, 40.0f, glm::vec4(0.0f, 0.5f, 1.0f, 1.0f));

            world->uiElements.insert(world->uiElements.end(), editText.begin(), editText.end());
        }
        if (world->mode == Mode::play)
        {
            auto itEdit = std::find_if(world->uiElements.begin(), world->uiElements.end(), [&](UI& ui) { return ui.glyphIndex == (32 + 5) && ui.color == glm::vec4(0.0f, 0.5f, 1.0f, 1.0f); });
            world->uiElements.erase(itEdit, itEdit + 9);
        }
    }

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        if (world->gameState == GameState::onLevelEnd)
        {
            world->level = world->level == System::levels.size() ? 1 : world->level + 1;
            world->LoadLevel();
        }
    }

    if (world->mode == Mode::play) return;

    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
    {
        world->CopyLevel();
    }

    Vector2i resize = Vector2i { 0, 0 };
    
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)   resize.x = -2;
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)  resize.x =  2;
    if (key == GLFW_KEY_UP && action == GLFW_PRESS)     resize.y =  2;
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)   resize.y = -2;

    if (resize != Vector2i { 0, 0 })
    {
        world->Resize(world->width + resize.x, world->height + resize.y);
    }
}

void glfwScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    World* world = (World*) glfwGetWindowUserPointer(window);

    if (world->mode == Mode::play) return;
    
    if (yOffset > 0)
    {
        world->level = world->level == 1 ? System::levels.size() : world->level - 1;
        world->LoadLevel();
    }
    if (yOffset < 0)
    {
        world->level = world->level == System::levels.size() ? 1 : world->level + 1;
        world->LoadLevel();
    }
}

World::World() :
    floor(),
    player(),
    blocks(0, Block(BlockType::goal, { 0, 0 })),
    uiElements(0, UI(Vector2i(), glm::vec2(), glm::vec4(1.0f), Texture(), 0)),
    projection(glm::mat4(1.0f)),
    view(glm::mat4(1.0f)),
    eyePolarPrev(glm::vec3(0.0f)),
    eyePolar(glm::vec3(0.0f)),
    uiProjection(glm::mat4(1.0f))
{
    projection = glm::perspective(glm::radians(45.0f), (float) System::width / System::height, 0.1f, 1000.0f);

    eyePolar = glm::vec3(0.0f, glm::radians(60.0f), 30.0f);

    uiProjection = glm::ortho<float>(0, System::width, 0.0f, System::height, -100.0f, 100.0f);
    
    Resize(width, height);
}

extern "C" {
    void AddLevel(char* levelText)
    {
        printf("%s", levelText);
        std::string levelString = levelText;

        std::stringstream ss;
        ss.str(levelString);
        
        LevelInfo info;

        int row = 0, rows = 0;
        int column = 0, columns = 0;
        char cell;

        ss >> cell >> cell >> cell >> cell;
        while (ss >> cell)
        {
            printf("%c", cell);
            if (cell == '-') break;
            if (cell == '<')
            {
                ss >> cell >> cell >> cell >> cell;
                
                row++;
                if (column > columns) columns = column;
                column = 0;
            }

            Vector2i location { column, row };
            
            switch (cell)
            {
                case '@':
                    info.playerStart = location;
                    break;
                case '#':
                    info.blockData.emplace_back(std::pair<BlockType, Vector2i>(BlockType::wall, location));
                    break;
                case 'B':
                    info.blockData.emplace_back(std::pair<BlockType, Vector2i>(BlockType::block, location));
                    break;
                case 'o':
                    info.blockData.emplace_back(std::pair<BlockType, Vector2i>(BlockType::goal, location));
                    break;
            }

            column++;
        }
        rows = row;

        info.size = Vector2i { columns, rows };
        info.playerStart = LevelToWorld(info.playerStart, info.size);
        for (auto& data : info.blockData)
        {
            data.second = LevelToWorld(data.second, info.size);
        }

        System::levels.emplace_back(info);
    }
}


void World::Initialize()
{
    glfwSetWindowUserPointer(System::window, this);
    glfwSetMouseButtonCallback(System::window, glfwMouseCallback);
    glfwSetKeyCallback(System::window, glfwKeyCallback);
    glfwSetScrollCallback(System::window, glfwScrollCallback);

    if (mode == Mode::play)
    {
        LoadLevel();
    }
}

void World::LoadLevel()
{
    blocks.clear();
    uiElements.clear();
    
    Vector2i playerStart = System::levels[level - 1].playerStart;
    player.currentCell = playerStart;
    player.targetCell = playerStart;
    player.worldPos = glm::vec3(playerStart.x, 0.0f, playerStart.y);
    player.state = MoveState::idle;

    for (auto& data : System::levels[level - 1].blockData)
    {
        blocks.emplace_back(Block(data.first, data.second));
    }

    Resize(System::levels[level - 1].size.x, System::levels[level - 1].size.y);

    if (level >= 10)
    {
        uiElements.emplace_back(Vector2i { 10 + 40 * 0, System::height - 100 }, glm::vec2(100.0f), glm::vec4(1.0f), System::textureFont, (level / 10) + 16);
        uiElements.emplace_back(Vector2i { 10 + 40 * 1, System::height - 100 }, glm::vec2(100.0f), glm::vec4(1.0f), System::textureFont, (level % 10) + 16);
    }
    else
    {
        uiElements.emplace_back(Vector2i { 10 + 40, System::height - 100 }, glm::vec2(100.0f), glm::vec4(1.0f), System::textureFont, level + 16);
    }

    if (mode == Mode::edit)
    {
        std::vector<UI> editText = CreateText("EDIT MODE", Vector2i { 20, System::height - 120 }, 40.0f, glm::vec4(0.0f, 0.5f, 1.0f, 1.0f));

        uiElements.insert(uiElements.end(), editText.begin(), editText.end());
    }
    
    gameState = GameState::onPlay;
}

void World::CopyLevel()
{
    std::string levelText {};
    
    for (int cell = 0; cell < width * height; cell++)
    {
        Vector2i location { cell % width, cell / width };
        if (location.x == 0) levelText += "\n";
        
        if (LevelToWorld(location, Vector2i { width, height }) == player.currentCell)
        {
            levelText += "@ ";
            continue;
        }
        
        auto it = std::find_if(blocks.begin(), blocks.end(), [&](Block block) {
            return LevelToWorld(location, Vector2i { width, height }) == block.currentCell;
        });
        if (it != blocks.end())
        {
            switch (it->type)
            {
                case BlockType::wall:   levelText += "# "; break;
                case BlockType::block:  levelText += "B "; break;
                case BlockType::goal:   levelText += "o "; break;
            }
            continue;
        }
        
        levelText += ". ";
    }
    levelText += "\n\n-------------------------------------\n";

#ifndef __EMSCRIPTEN__
    glfwSetClipboardString(System::window, levelText.c_str());
    printf("%s", levelText.c_str());
#else
    EM_ASM({
        let body = document.querySelector("body");
        if (!document.getElementById("paste"))
        {
            let p = document.createElement("p");
            p.id = "paste";
            body.appendChild(p);
        }
        let p = document.getElementById("paste");
        let str = UTF8ToString($0);
        str = str.replaceAll("\n", "<br>");
        p.style = "font-family: Courier New;";
        p.innerHTML = str;
    }, levelText.c_str());
#endif
}

std::vector<UI> World::CreateText(std::string text, Vector2i position, float size, glm::vec4 color)
{
    std::vector<UI> uiText;
    
    for (int letter = 0; letter < text.length(); letter++)
    {
        uiText.emplace_back(position + Vector2i { letter * (int) size / 3, 0 }, glm::vec2(size), color, System::textureFont, text[letter] - 32);
    }

    return uiText;
}

void World::Update(float dt)
{
    eyePolarPrev = eyePolar;
    
    if (mode == Mode::edit)
    {
        if (glfwGetKey(System::window, GLFW_KEY_J) == GLFW_PRESS)
            eyePolar.x += -dt;
        if (glfwGetKey(System::window, GLFW_KEY_L) == GLFW_PRESS)
            eyePolar.x += dt;
        if (glfwGetKey(System::window, GLFW_KEY_I) == GLFW_PRESS)
            eyePolar.y = glm::clamp<float>(eyePolar.y + dt, -glm::half_pi<float>() + glm::epsilon<float>(), glm::half_pi<float>() - glm::epsilon<float>());
        if (glfwGetKey(System::window, GLFW_KEY_K) == GLFW_PRESS)
            eyePolar.y = glm::clamp<float>(eyePolar.y - dt, -glm::half_pi<float>() + glm::epsilon<float>(), glm::half_pi<float>() - glm::epsilon<float>());
        if (glfwGetKey(System::window, GLFW_KEY_U) == GLFW_PRESS)
            eyePolar.z += -dt * 50;
        if (glfwGetKey(System::window, GLFW_KEY_O) == GLFW_PRESS)
            eyePolar.z += dt * 50;
    }


    if (gameState == GameState::onPlay)
    {
        player.Update(dt);

        for (Block& block : blocks)
        {
            block.Update(dt);
        }

        ModifyState();

        if (std::count_if(blocks.begin(), blocks.end(), [&](Block block) { return block.onGoal; }) == std::count_if(blocks.begin(), blocks.end(), [&](Block block) { return block.type == BlockType::goal; }) && mode == Mode::play)
        {
            gameState = GameState::onLevelEnd;

            std::vector<UI> completeText = CreateText("LEVEL COMPLETE!", Vector2i { 150, System::height / 2 }, 100.0f, glm::vec4(1.0f, 0.9f, 0.0f, 1.0f));
            std::vector<UI> nextLevelText = CreateText("SPACE TO CONTINUE", Vector2i { 230, System::height / 2 - 100 }, 60.0f, glm::vec4(1.0f));
            UI backdrop = UI(Vector2i { 130, System::height / 2 - 85 }, glm::vec2(540.0f, 200.0f), glm::vec4(0.7f), System::textureButton, -1);

            uiElements.emplace_back(backdrop);
            uiElements.insert(uiElements.end(), completeText.begin(), completeText.end());
            uiElements.insert(uiElements.end(), nextLevelText.begin(), nextLevelText.end());
        }
    }

    if (gameState == GameState::onLevelEnd)
    {
        player.worldPosPrev = player.worldPos;

        for (Block& block : blocks)
        {
            block.worldPosPrev = block.worldPos;
        }
    }
}

void World::ModifyState()
{
    if (player.state == MoveState::premove || player.state == MoveState::postmove)
    {
        if (glm::abs(player.currentCell.x + player.direction.x) > width / 2 || 
            glm::abs(player.currentCell.y + player.direction.y) > height / 2)
        {
            player.state = MoveState::idle;
        }
        for (Block& block : blocks)
        {
            if (block.type == BlockType::goal) continue;
            
            if (player.currentCell + player.direction == block.currentCell)
            {
                if (player.state == MoveState::premove)
                {
                    player.state = MoveState::idle;
                }
                if (player.state == MoveState::postmove)
                {
                    player.state = MoveState::idle;

                    if (block.type == BlockType::block && !block.onGoal)
                    {
                        block.state = MoveState::postmove;
                        block.direction = player.direction;
                    }
                }
                break;
            }
        }

        if (player.state == MoveState::premove || player.state == MoveState::postmove)
        {
            player.state = MoveState::moving;
            player.targetCell = player.currentCell + player.direction;
        }
    }

    for (Block& blockActive : blocks)
    {
        if (blockActive.state == MoveState::postmove)
        {   
            if (glm::abs(blockActive.currentCell.x + blockActive.direction.x) > width / 2 ||
                glm::abs(blockActive.currentCell.y + blockActive.direction.y) > height / 2)
            {
                blockActive.state = MoveState::idle;
                blockActive.onGoal = CheckBlockOnGoal(blockActive);
                continue;
            }
            
            for (Block& blockChecked : blocks)
            {
                if (blockChecked.type == BlockType::goal) continue;
                
                if (blockActive.currentCell + blockActive.direction == blockChecked.currentCell)
                {
                    blockActive.state = MoveState::idle;
                    blockActive.onGoal = CheckBlockOnGoal(blockActive);
                    
                    if (blockChecked.type == BlockType::block && !blockChecked.onGoal)
                    {
                        blockChecked.state = MoveState::postmove;
                        blockChecked.direction = blockActive.direction;
                    }

                    break;
                }
            }

            if (blockActive.state == MoveState::postmove)
            {
                blockActive.targetCell = blockActive.currentCell + blockActive.direction;
                blockActive.state = MoveState::moving;
            }
        }
    }
}
 
void World::Render(float lag)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::vec3 eyePolarLerped = glm::mix(eyePolarPrev, eyePolar, lag);
    glm::vec3 eye = PolarToRect(eyePolarLerped);
    view = glm::mat4(1.0f);
    view = glm::lookAt(eye, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 renderTransform = glm::mat4(1.0f);
    renderTransform = projection * view * renderTransform;

    glEnable(GL_DEPTH_TEST);

    floor.Draw(renderTransform);
    player.Draw(renderTransform, lag);
    for (Block& block : blocks)
    {
        block.Draw(renderTransform, lag);
    }

    glm::mat4 uiTransform = glm::mat4(1.0f);
    uiTransform = uiProjection * uiTransform;

    glDisable(GL_DEPTH_TEST);
    for (UI& ui : uiElements)
    {
        ui.Draw(uiTransform);
    }
}

bool World::CheckBlockOnGoal(Block& blockChecked)
{
    auto it = std::find_if(blocks.begin(), blocks.end(), [&](Block block) {
        return block.currentCell == blockChecked.currentCell && block.type == BlockType::goal;
    });

    return it != blocks.end();
}

void World::Resize(int newWidth, int newHeight)
{
    width = newWidth;
    height = newHeight;

    floor.model = glm::mat4(1.0f);
    floor.model = glm::translate(floor.model, glm::vec3(0.0f, -1.0f, 0.0f));
    floor.model = glm::scale(floor.model, glm::vec3((float)width, 0.0f, (float)height));

    floor.scale = glm::vec2((float)width, (float)height);
}

glm::vec3 World::GetMouseRay(float mouseX, float mouseY)
{
    glm::vec4 clipCoord = glm::vec4(2.0f * mouseX / System::width - 1.0f, -(2.0f * mouseY / System::height - 1.0f), -1.0f, 1.0f);
    glm::vec4 eyeCoord = glm::vec4((glm::inverse(projection) * clipCoord).xy(), -1.0f, 0.0f);
    glm::vec3 ray = glm::normalize((inverse(view) * eyeCoord).xyz());

    return ray;
}
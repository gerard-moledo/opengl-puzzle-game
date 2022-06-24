#include "world.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <algorithm>

void glfwMouseCallback(GLFWwindow* window, int button, int action, int mods)
{
    World* world = (World*)glfwGetWindowUserPointer(window);

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
            bool occupied = false;
            for (Block& block : world->blocks)
            {
                if (block.currentCell == hitGrid)
                {
                    block.type = (BlockType) ( ((int) block.type + 1) % 3);

                    occupied = true;
                }
            }

            if (!occupied)
            {
                world->blocks.emplace_back(Block { world->renderer, BlockType::block, hitGrid });
            }
        }

        if (button == GLFW_MOUSE_BUTTON_MIDDLE)
        {
            bool occupied = false;
            for (Block& block : world->blocks)
            {
                if (block.currentCell == hitGrid)
                {
                    occupied = true;
                }
            }

            if (!occupied)
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

World::World(Renderer& renderer) :
    renderer(renderer),
    floor(renderer),
    player(renderer),
    blocks(0, Block(renderer, BlockType::goal, { 0, 0 })),
    projection(glm::mat4(1.0f)),
    view(glm::mat4(1.0f)),
    eyePolarPrev(glm::vec3(0.0f)),
    eyePolar(glm::vec3(0.0f))
{
    projection = glm::perspective(glm::radians(45.0f), (float) System::width / System::height, 0.1f, 100.0f);

    eyePolar = glm::vec3(0.0f, glm::radians(60.0f), 30.0f);
    
    Resize(width, height);
}

void World::Initialize()
{
    glfwSetWindowUserPointer(System::window, this);
    glfwSetMouseButtonCallback(System::window, glfwMouseCallback);
    glfwSetKeyCallback(System::window, glfwKeyCallback);
}

void World::Update(float dt)
{
    eyePolarPrev = eyePolar;
    
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



    
    player.Update(dt);

    for (Block& block : blocks)
    {
        block.Update(dt);
    }
    
    ModifyState();
}

void World::ModifyState()
{
    if (player.state == State::premove || player.state == State::postmove)
    {
        if (glm::abs(player.currentCell.x + player.direction.x) > width / 2 || 
            glm::abs(player.currentCell.y + player.direction.y) > height / 2)
        {
            player.state = State::idle;
        }
        for (Block& block : blocks)
        {
            if (block.type == BlockType::goal) continue;
            
            if (player.currentCell + player.direction == block.currentCell)
            {
                if (player.state == State::premove)
                {
                    player.state = State::idle;
                }
                if (player.state == State::postmove)
                {
                    player.state = State::idle;

                    if (block.type == BlockType::block && !block.onGoal)
                    {
                        block.state = State::postmove;
                        block.direction = player.direction;
                    }
                }
                break;
            }
        }

        if (player.state == State::premove || player.state == State::postmove)
        {
            player.state = State::moving;
            player.targetCell = player.currentCell + player.direction;
        }
    }

    for (Block& blockActive : blocks)
    {
        if (blockActive.state == State::postmove)
        {   
            if (glm::abs(blockActive.currentCell.x + blockActive.direction.x) > width / 2 ||
                glm::abs(blockActive.currentCell.y + blockActive.direction.y) > height / 2)
            {
                blockActive.state = State::idle;
                blockActive.onGoal = CheckBlockOnGoal(blockActive);
                continue;
            }
            
            for (Block& blockChecked : blocks)
            {
                if (blockChecked.type == BlockType::goal) continue;
                
                if (blockActive.currentCell + blockActive.direction == blockChecked.currentCell)
                {
                    blockActive.state = State::idle;
                    blockActive.onGoal = CheckBlockOnGoal(blockActive);
                    
                    if (blockChecked.type == BlockType::block && !blockChecked.onGoal)
                    {
                        blockChecked.state = State::postmove;
                        blockChecked.direction = blockActive.direction;
                    }

                    break;
                }
            }

            if (blockActive.state == State::postmove)
            {
                blockActive.targetCell = blockActive.currentCell + blockActive.direction;
                blockActive.state = State::moving;
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

    floor.Draw(renderTransform);
    player.Draw(renderTransform, lag);
    for (Block& block : blocks)
    {
        block.Draw(renderTransform, lag);
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

    floor.scale = glm::vec3((float)width, 0.0f, (float)height);
}

glm::vec3 World::GetMouseRay(float mouseX, float mouseY)
{
    glm::vec4 clipCoord = glm::vec4(2.0f * mouseX / System::width - 1.0f, -(2.0f * mouseY / System::height - 1.0f), -1.0f, 1.0f);
    glm::vec4 eyeCoord = glm::vec4((glm::inverse(projection) * clipCoord).xy(), -1.0f, 0.0f);
    glm::vec3 ray = glm::normalize((inverse(view) * eyeCoord).xyz());

    return ray;
}
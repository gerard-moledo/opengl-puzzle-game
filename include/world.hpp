#ifndef WORLD_H
#define WORLD_H

#define GLM_FORCE_SWIZZLE
#include "glm/glm.hpp"

#include <vector>
#include <string>
#include <utility>

#include "renderer.hpp"
#include "models.hpp"
#include "player.hpp"
#include "block.hpp"
#include "utils.hpp"

class World
{
public:
    World();

    int level = 1;
    int width = 7;
    int height = 7;

    Mode mode = Mode::play;
    GameState gameState = GameState::onPlay;
    
    FloorModel floor;
    Player player;
    std::vector<Block> blocks;

    std::vector<UI> uiElements;
    
    glm::mat4 projection;
    glm::mat4 view;
    glm::vec3 eyePolarPrev;
    glm::vec3 eyePolar;

    glm::mat4 uiProjection;

    glm::vec3 GetMouseRay(float mouseX, float mouseY);
    void Resize(int newWidth, int newHeight);
    
    void Initialize();
    void Update(float dt);
    void Render(float lag);

    void LoadLevel();
    void CopyLevel();

    std::vector<UI> CreateText(std::string text, Vector2i position, float size, glm::vec4 color);
    
private:
    void ModifyState();
    bool CheckBlockOnGoal(Block& block);
};

#endif
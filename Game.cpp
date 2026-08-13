#include "Game.h"

void Game::Initialize()
{
    player.SetPosition(glm::vec3(0.0f, 1.0f, 0.0f));
    player.SetMoveSpeed(5.0f);

    camera.Position = glm::vec3(0.0f, 3.0f, 5.0f);
}
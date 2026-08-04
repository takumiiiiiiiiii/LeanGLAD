#include "Player.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
Player::Player(Transform initialTransform, float size)
    : cube(size),
      transform(initialTransform),
      moveSpeed(10.0f)
{
}

void Player::Update(float dt)
{
    Move(dt);
    Draw();
}

void Player::Move(float dt)
{
    glm::vec3 dir(0.0f);

    if (Input::IsKeyPressed(GLFW_KEY_W))
    {
        std::cout << "Press W" << std::endl;
        dir.z -= 1.0f;
    }

    if (Input::IsKeyPressed(GLFW_KEY_S))
    {
        std::cout << "Press S" << std::endl;
        dir.z += 1.0f;
    }

    if (Input::IsKeyPressed(GLFW_KEY_A))
    {
        std::cout << "Press A" << std::endl;
        dir.x -= 1.0f;
    }

    if (Input::IsKeyPressed(GLFW_KEY_D))
    {
        std::cout << "Press D" << std::endl;
        dir.x += 1.0f;
    }

    if (glm::length(dir) > 0.0f)
    {
        dir = glm::normalize(dir);
    }

    transform.Translate(dir * moveSpeed * dt);
}
void Player::SetPosition(const glm::vec3& pos)
{
    transform.SetPosition(pos);
}

glm::vec3 Player::GetPosition() const
{
    return transform.GetPosition();
}

void Player::Draw()
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model,transform.GetPosition());

    // model行列をpositionで作成
    // shaderへ送る
    cube.Draw();
}



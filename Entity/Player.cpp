#include "Player.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
Player::Player(Transform initialTransform, float size)
    : cube(size),
      transform(initialTransform),
      moveSpeed(1.0f)
{
}

void Player::Update(float deltaTime)
{
    Move(deltaTime);
    Draw();
}

void Player::Move(float deltaTime)
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

    transform.Translate(dir * moveSpeed * deltaTime);
}
void Player::MoveWithCameraOrientation(Camera& camera, float deltaTime)
{
    glm::vec3 forward = camera.Front;
    forward.y = 0.0f;
    forward = glm::normalize(forward);
    glm::vec3 ringt = camera.Right;
    ringt.y = 0.0f;
    ringt = glm::normalize(ringt);

    glm::vec3 movement(0.0f);

    if (Input::IsKeyPressed(GLFW_KEY_W))
    {
        std::cout << "Press W" << std::endl;
        movement += forward;
    }

    if (Input::IsKeyPressed(GLFW_KEY_S))
    {
        std::cout << "Press S" << std::endl;
        movement -= forward;
    }

    if (Input::IsKeyPressed(GLFW_KEY_A))
    {
        std::cout << "Press A" << std::endl;
        movement -= ringt;
    }

    if (Input::IsKeyPressed(GLFW_KEY_D))
    {
        std::cout << "Press D" << std::endl;
        movement += ringt;
    }

    // 水平方向のみに制限
    movement.y = 0.0f;

    if (glm::length(movement) > 0.0f)
    {
        movement = glm::normalize(movement);
    }

    transform.Translate(movement * moveSpeed * deltaTime);
}
void Player::MoveWithVector(const glm::vec3& direction, float deltaTime)
{
    transform.Translate(direction * moveSpeed * deltaTime);
}

void Player::SetPosition(const glm::vec3& pos)
{
    transform.SetPosition(pos);
}

void Player::SetMoveSpeed(float speed)
{
    moveSpeed = speed;
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



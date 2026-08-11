#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Transform.h"
#include "../Shapes/Cube.h"
#include "../Core/Input.h"
#include "../Camera.h"
class Player
{
public:
    Player(Transform initialTransform, float size = 1.0f);
    void Update(float deltaTime);

    void Draw();
    void Move(float deltaTime);
    void MoveWithCameraOrientation(Camera& camera, float deltaTime);
    void MoveWithVector(const glm::vec3& direction, float deltaTime);

    glm::vec3 GetPosition() const;
    void SetPosition(const glm::vec3& pos);
    void SetMoveSpeed(float speed);
private:
    Cube cube;
    Transform transform;
    float moveSpeed;
};
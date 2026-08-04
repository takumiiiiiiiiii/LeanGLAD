#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Transform.h"
#include "../Shapes/Cube.h"
#include "../Core/Input.h"
class Player
{
public:
    Player(Transform initialTransform, float size = 1.0f);
    void Update(float deltaTime);

    void Draw();
    void Move(float deltaTime);

    glm::vec3 GetPosition() const;
    void SetPosition(const glm::vec3& pos);
private:
    Cube cube;
    Transform transform;
    float moveSpeed;
};
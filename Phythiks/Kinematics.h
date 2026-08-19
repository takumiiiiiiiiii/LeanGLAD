// Kinematics.h
#pragma once
#include <glm/glm.hpp>

struct Kinematics
{
    glm::vec3 velocity{0.0f};
    glm::vec3 acceleration{0.0f};

    void Integrate(float deltaTime)
    {
        velocity += acceleration * deltaTime;
    }
};
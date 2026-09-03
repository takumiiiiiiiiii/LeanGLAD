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
    void Move(glm::vec3 vector)
    {
        velocity += vector*velocity;
    }
    glm::vec3 GetVelocity(float deltaTime)
    {
        return velocity;
    }
    float GetSpeed(float deltaTime)
    {
        return glm::length(velocity);
    }
    float GetAccelerationMagnitude(float deltaTime)
    {
        return glm::length(acceleration);
    }
};
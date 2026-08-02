#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform
{
public:

    Transform();

    // Position
    void SetPosition(const glm::vec3& position);
    void Translate(const glm::vec3& offset);
    const glm::vec3& GetPosition() const;

    // Rotation (degree)
    void SetRotation(const glm::vec3& rotation);
    void Rotate(const glm::vec3& angle);
    const glm::vec3& GetRotation() const;

    // Scale
    void SetScale(const glm::vec3& scale);
    const glm::vec3& GetScale() const;

    // Model Matrix
    glm::mat4 GetModelMatrix() const;

private:

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};
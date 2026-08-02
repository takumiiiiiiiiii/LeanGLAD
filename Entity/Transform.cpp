#include "Transform.h"

Transform::Transform()
{
    position = glm::vec3(0.0f);
    rotation = glm::vec3(0.0f);
    scale = glm::vec3(1.0f);
}

void Transform::SetPosition(const glm::vec3& pos)
{
    position = pos;
}

void Transform::Translate(const glm::vec3& offset)
{
    position += offset;
}

const glm::vec3& Transform::GetPosition() const
{
    return position;
}

void Transform::SetRotation(const glm::vec3& rot)
{
    rotation = rot;
}

void Transform::Rotate(const glm::vec3& angle)
{
    rotation += angle;
}

const glm::vec3& Transform::GetRotation() const
{
    return rotation;
}

void Transform::SetScale(const glm::vec3& s)
{
    scale = s;
}

const glm::vec3& Transform::GetScale() const
{
    return scale;
}

glm::mat4 Transform::GetModelMatrix() const
{
    glm::mat4 model(1.0f);

    model = glm::translate(model, position);

    model = glm::rotate(model,
                        glm::radians(rotation.x),
                        glm::vec3(1.0f,0.0f,0.0f));

    model = glm::rotate(model,
                        glm::radians(rotation.y),
                        glm::vec3(0.0f,1.0f,0.0f));

    model = glm::rotate(model,
                        glm::radians(rotation.z),
                        glm::vec3(0.0f,0.0f,1.0f));

    model = glm::scale(model, scale);

    return model;
}
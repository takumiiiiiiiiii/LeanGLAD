#include "Object.h"

Object::Object(const Transform& initialTransform)
    : transform(initialTransform),
      active(true)
{
}

Transform& Object::GetTransform()
{
    return transform;
}

const Transform& Object::GetTransform() const
{
    return transform;
}

void Object::SetActive(bool isActive)
{
    active = isActive;
}

bool Object::IsActive() const
{
    return active;
}

void Object::Update(float)
{
}

void Object::Draw(Shader&)
{
}

#pragma once

#include "Transform.h"

class Shader;

// Base class for every entity that exists in the game world.
// Transform remains a value object, while Object owns the transform and
// provides the common lifetime/update/draw interface for world entities.
class Object
{
public:
    explicit Object(const Transform& initialTransform = Transform());
    virtual ~Object() = default;

    Transform& GetTransform();
    const Transform& GetTransform() const;

    void SetActive(bool active);
    bool IsActive() const;

    virtual void Update(float deltaTime);
    virtual void Draw(Shader& shader);

protected:
    Transform transform;

private:
    bool active;
};

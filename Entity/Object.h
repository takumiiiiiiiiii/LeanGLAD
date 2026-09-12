#pragma once

#include "Transform.h"
#include "../Collision/AABB.h"
// #include "../Game.h"
class Shader;
// Base class for every entity that exists in the game world.
// Transform remains a value object, while Object owns the transform and
// provides the common lifetime/update/draw interface for world entities.
enum class ObjectEvent
{
    None,
    GoalReached,
    GameOver,
    ItemGet,
    CheckPoint
};
class Object
{
public:
    explicit Object(const Transform& initialTransform = Transform());
    virtual ~Object() = default;

    Transform& GetTransform();
    const Transform& GetTransform() const;

    void SetActive(bool active);
    bool IsActive() const;
    virtual bool CheckPlayerEnter(const AABB& playerAABB)
    {
        return false;
    }
    virtual ObjectEvent OnPlayerEnter()
    {
        return ObjectEvent::None;
    }
    virtual void Update(float deltaTime);
    virtual void Draw(Shader& shader);

protected:
    Transform transform;

private:
    bool active;
};

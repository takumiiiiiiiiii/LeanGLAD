#include "AABB.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

AABB::AABB()
    : min(0.0f),
      max(0.0f)
{
}

AABB::AABB(const glm::vec3& min, const glm::vec3& max)
    : min(min),
      max(max)
{
}

void AABB::Set(const glm::vec3& center, const glm::vec3& size)
{
    glm::vec3 halfSize = size * 0.5f;

    min = center - halfSize;
    max = center + halfSize;
}

void AABB::SetMinMax(
    const glm::vec3& min,
    const glm::vec3& max)
{
    this->min = min;
    this->max = max;
}

bool AABB::Intersects(const AABB& other) const
{
    // X軸
    if (max.x < other.min.x || min.x > other.max.x)
    {
        return false;
    }

    // Y軸
    if (max.y < other.min.y || min.y > other.max.y)
    {
        return false;
    }

    // Z軸
    if (max.z < other.min.z || min.z > other.max.z)
    {
        return false;
    }

    return true;
}

bool AABB::Contains(const glm::vec3& point) const
{
    return point.x >= min.x && point.x <= max.x &&
           point.y >= min.y && point.y <= max.y &&
           point.z >= min.z && point.z <= max.z;
}

glm::vec3 AABB::GetCenter() const
{
    return (min + max) * 0.5f;
}

glm::vec3 AABB::GetSize() const
{
    return max - min;
}

glm::vec3 AABB::GetMin() const
{
    return min;
}

glm::vec3 AABB::GetMax() const
{
    return max;
}

void AABB::DrawDebug() const
{
    // glm::vec3 center = GetCenter();
    // glm::vec3 size = GetSize();

    // glPushMatrix();

    // glTranslatef(
    //     center.x,
    //     center.y,
    //     center.z
    // );

    // glScalef(
    //     size.x,
    //     size.y,
    //     size.z
    // );

    // glutWireCube(1.0);

    // glPopMatrix();
}
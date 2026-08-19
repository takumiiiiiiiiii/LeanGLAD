#include "RayCast.h"
#include <glm/glm.hpp>
bool intersectRayTriangle(const glm::vec3& orig, const glm::vec3& dir, const Triangle& tri, float& t) {
    const float EPS = 1e-6f;
    glm::vec3 e1 = tri.v1 - tri.v0, e2 = tri.v2 - tri.v0;
    glm::vec3 p = glm::cross(dir, e2);
    float det = glm::dot(e1, p);
    if (fabs(det) < EPS) return false;
    float invDet = 1.0f / det;
    glm::vec3 s = orig - tri.v0;
    float u = glm::dot(s, p) * invDet;
    if (u < 0 || u > 1) return false;
    glm::vec3 q = glm::cross(s, e1);
    float v = glm::dot(dir, q) * invDet;
    if (v < 0 || u + v > 1) return false;
    t = glm::dot(e2, q) * invDet;
    return t > EPS;
}
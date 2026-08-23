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

void screenPosToWorldRay(
    double mouseX, double mouseY,
    int screenWidth, int screenHeight,
    const glm::mat4& view,
    const glm::mat4& projection,
    glm::vec3& outRayOrigin,
    glm::vec3& outRayDir)
{
    // 1. スクリーン座標を正規化(左上原点, ピクセル) -> NDC(-1〜1, 中心原点)
    // GLFWはY軸が下向きなので反転させる
    float x = (2.0f * static_cast<float>(mouseX)) / screenWidth - 1.0f;
    float y = 1.0f - (2.0f * static_cast<float>(mouseY)) / screenHeight;

    // 2. 近平面(z=-1)と遠平面(z=1)の2点をクリップ座標として定義
    glm::vec4 rayStartNDC(x, y, -1.0f, 1.0f);
    glm::vec4 rayEndNDC(x, y, 1.0f, 1.0f);

    // 3. view*projectionの逆行列でワールド座標に戻す
    glm::mat4 invVP = glm::inverse(projection * view);

    glm::vec4 rayStartWorld = invVP * rayStartNDC;
    rayStartWorld /= rayStartWorld.w;

    glm::vec4 rayEndWorld = invVP * rayEndNDC;
    rayEndWorld /= rayEndWorld.w;

    outRayOrigin = glm::vec3(rayStartWorld);
    outRayDir = glm::normalize(glm::vec3(rayEndWorld - rayStartWorld));
}
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Triangle.h"
bool intersectRayTriangle(const glm::vec3& orig, const glm::vec3& dir, const Triangle& tri, float& t);
// スクリーン座標からワールド空間のレイ(視点位置と方向)を生成する
void screenPosToWorldRay(
    double mouseX, double mouseY,
    int screenWidth, int screenHeight,
    const glm::mat4& view,
    const glm::mat4& projection,
    glm::vec3& outRayOrigin,
    glm::vec3& outRayDir);
#pragma once
#include <glm/glm.hpp>


class AABB
{
private:
    glm::vec3 min;
    glm::vec3 max;

public:
    AABB();
    AABB(const glm::vec3& min, const glm::vec3& max);

    // 中心座標とサイズから設定
    void Set(const glm::vec3& center, const glm::vec3& size);

    // min / max を直接設定
    void SetMinMax(const glm::vec3& min, const glm::vec3& max);

    // 他のAABBとの衝突判定
    bool Intersects(const AABB& other) const;

    // 点がAABB内部にあるか
    bool Contains(const glm::vec3& point) const;

    // 中心座標
    glm::vec3 GetCenter() const;

    // サイズ
    glm::vec3 GetSize() const;

    // 最小座標
    glm::vec3 GetMin() const;

    // 最大座標
    glm::vec3 GetMax() const;

    // GLUTによるデバッグ描画
    void DrawDebug() const;
};
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Triangle.h"
#include "RayCast.h"
class CollisionMesh {
public:
    //メッシュの座標と法線ベクトルをまとめる
    void addTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2);
    //地面と交差しているかを判定
    bool raycast(const glm::vec3& origin, const glm::vec3& dir, float& outDist, glm::vec3& outNormal) const;
    //保持している三角形一覧を取得(参照なのでコピーは発生しない)
    const std::vector<Triangle>& getTriangles() const { return triangles; }

    void prindDebug() const;
private:
    std::vector<Triangle> triangles;
};
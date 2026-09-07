#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Triangle.h"
#include "RayCast.h"
class CollisionMesh {
public:
    //メッシュの座標と法線ベクトルをまとめる
    void addTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2);
    void addTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2 ,const glm::vec3& n);
    void addTriangleForObject(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& n, const void* object);
    // Cube ID付き三角形追加(新しい方法)
    void addTriangleWithCubeId(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& n, uint32_t cubeId);
    bool removeCube(const glm::vec3& center, float size);
    // Cube IDで三角形を削除(最も確実な方法)
    bool removeCubeById(uint32_t cubeId);
    bool removeTrianglesForObject(const void* object);
    //地面と交差しているかを判定
    bool raycast(const glm::vec3& origin, const glm::vec3& dir, float& outDist, glm::vec3& outNormal) const;
    //保持している三角形一覧を取得(参照なのでコピーは発生しない)
    // 注: 現在は未使用（デバッグ用に残している）
    // const std::vector<Triangle>& getTriangles() const;

    void prindDebug() const;
private:
    struct TriangleWithCubeId {
        Triangle triangle;
        uint32_t cubeId;  // どのCubeに属するか
        const void* object = nullptr;
    };
    std::vector<TriangleWithCubeId> triangles;
};

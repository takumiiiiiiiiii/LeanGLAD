#include "CollisionMesh.h"
#include <algorithm>
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp> // glm::to_string用
    //メッシュの座標と法線ベクトルをまとめる
    void CollisionMesh::addTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2) {
        glm::vec3 n = glm::normalize(glm::cross(v1 - v0, v2 - v0));
        triangles.push_back({v0, v1, v2, n});
    }
    void CollisionMesh::addTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2 ,const glm::vec3& n) {
        triangles.push_back({v0, v1, v2, n});
    }
    size_t CollisionMesh::addTriangleGetIndex(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& n) {
        size_t index = triangles.size();
        triangles.push_back({v0, v1, v2, n});
        return index;
    }
    bool CollisionMesh::removeCubeByIndices(const std::vector<size_t>& indices) {
        if (indices.empty()) return false;

        // インデックスをソート（逆順削除用）
        std::vector<size_t> sortedIndices = indices;
        std::sort(sortedIndices.rbegin(), sortedIndices.rend()); // 降順ソート

        bool removed = false;
        for (size_t idx : sortedIndices) {
            if (idx < triangles.size()) {
                triangles.erase(triangles.begin() + idx);
                removed = true;
            }
        }
        return removed;
    }
    bool CollisionMesh::removeCube(const glm::vec3& center, float size) {
        const float halfSize = size * 0.5f;
        const glm::vec3 min = center - glm::vec3(halfSize);
        const glm::vec3 max = center + glm::vec3(halfSize);
        const float epsilon = 0.0001f;

        const auto isInsideCube = [&](const glm::vec3& vertex) {
            return vertex.x >= min.x - epsilon && vertex.x <= max.x + epsilon &&
                   vertex.y >= min.y - epsilon && vertex.y <= max.y + epsilon &&
                   vertex.z >= min.z - epsilon && vertex.z <= max.z + epsilon;
        };

        const auto oldSize = triangles.size();
        triangles.erase(
            std::remove_if(triangles.begin(), triangles.end(), [&](const Triangle& triangle) {
                return isInsideCube(triangle.v0) &&
                       isInsideCube(triangle.v1) &&
                       isInsideCube(triangle.v2);
            }),
            triangles.end());

        return triangles.size() != oldSize;
    }
    //地面と交差しているかを判定
    bool CollisionMesh::raycast(const glm::vec3& origin, const glm::vec3& dir, float& outDist, glm::vec3& outNormal) const {
        float closest = std::numeric_limits<float>::max();
        bool hit = false;
        for (const auto& tri : triangles) {
            float t;
            if (intersectRayTriangle(origin, dir, tri, t) && t < closest) {
                closest = t;
                outNormal = tri.normal;
                hit = true;
            }
        }
        
        if (hit) outDist = closest;
        return hit;
    }
    void CollisionMesh::prindDebug()const{
        // std::cout << "=== CollisionMesh: " << triangles.size() << " triangles ===\n";
        for (size_t i = 0; i < triangles.size(); ++i) {
            const Triangle& tri = triangles[i];
            // std::cout << "[" << i << "] "
            // << "v0=" << glm::to_string(tri.v0) << " "
            //      << "v1=" << glm::to_string(tri.v1) << " "
            // << "v2=" << glm::to_string(tri.v2) << " "
            // << "normal=" << glm::to_string(tri.normal) << "\n";
        }
    }
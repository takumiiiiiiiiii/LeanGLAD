#include "CollisionMesh.h"
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp> // glm::to_string用
    //メッシュの座標と法線ベクトルをまとめる
    void CollisionMesh::addTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2) {
        
        try{
            glm::vec3 n = glm::normalize(glm::cross(v1 - v0, v2 - v0));
            triangles.push_back({v0, v1, v2, n});
        }catch(int i){
            std::cout << "cantUse" << std::endl;
        }

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
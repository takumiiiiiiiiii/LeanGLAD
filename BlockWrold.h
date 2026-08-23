#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "Cube.h"
#include "Collision/CollisionMesh.h"

class Shader;

// 配置済みブロック1個分の情報(位置とCubeの実体をまとめて保持)
struct PlacedBlock
{
    glm::vec3 position;
    std::unique_ptr<Cube> cube;
};

class BlockWorld
{
public:
    explicit BlockWorld(CollisionMesh& collisionMesh, float cubeSize = 1.0f);

    // worldPosにキューブ1個分の空きがあれば配置する
    void PlaceBlock(const glm::vec3& worldPos);

    void DrawAll(Shader& shader);

private:
    bool IsOccupied(const glm::vec3& worldPos) const;

    CollisionMesh& collisionMesh;
    float cubeSize;
    std::vector<PlacedBlock> blocks;
};
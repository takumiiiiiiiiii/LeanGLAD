#include "BlockWrold.h"

BlockWorld::BlockWorld(CollisionMesh& collisionMesh, float cubeSize)
    : collisionMesh(collisionMesh), cubeSize(cubeSize)
{
}

bool BlockWorld::IsOccupied(const glm::vec3& worldPos) const
{
    // キューブサイズの1割程度を許容誤差として、同じセルへの重複配置を防ぐ
    const float epsilon = cubeSize * 0.1f;
    for (const auto& block : blocks)
    {
        if (glm::distance(block.position, worldPos) < epsilon)
            return true;
    }
    return false;
}

void BlockWorld::PlaceBlock(const glm::vec3& worldPos)
{
    if (IsOccupied(worldPos))
        return;
    glm::vec3 offset  = worldPos;
    auto cube = std::make_unique<Cube>(cubeSize);
    cube->SetTranformPosition(offset);

    // 新しく置いたキューブも次のRaycastの対象にする(これがないと2段目を積めない)
    cube->RegisterCollision(collisionMesh);

    blocks.push_back(PlacedBlock{ offset, std::move(cube) });
}

void BlockWorld::DrawAll(Shader& shader)
{
    for (auto& block : blocks)
    {
        block.cube->Draw(shader);
    }
}
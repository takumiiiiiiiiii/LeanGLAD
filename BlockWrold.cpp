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
    // セルインデックスをfloorで求め、そのセルの中心(index + 0.5)*cubeSizeにスナップする。
    // Cubeは中心基準(-0.5〜+0.5)なので、中心をセル境界の真ん中に合わせる必要がある。
    // truncではなくfloorを使うことで、負の座標側でもズレなく動作する。
    glm::vec3 snapped;
    snapped.x = (std::floor(worldPos.x / cubeSize) + 0.5f) * cubeSize;
    snapped.y = (std::floor(worldPos.y / cubeSize) + 0.5f) * cubeSize;
    snapped.z = (std::floor(worldPos.z / cubeSize) + 0.5f) * cubeSize;

    if (IsOccupied(snapped))
        return;

    auto cube = std::make_unique<Cube>(cubeSize);
    cube->SetTranformPosition(snapped);

    // 新しく置いたキューブも次のRaycastの対象にする(これがないと2段目を積めない)
    cube->RegisterCollision(collisionMesh);

    blocks.push_back(PlacedBlock{ snapped, std::move(cube) });
}

void BlockWorld::DrawAll(Shader& shader)
{
    for (auto& block : blocks)
    {
        block.cube->Draw(shader);
    }
}
#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "Shapes/Cube.h"
#include "Collision/CollisionMesh.h"
#include <fstream>
#include <sstream>
#include <iostream>
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

    //ワールド座標をせる座標に
    glm::vec3 SnapToGrid(const glm::vec3& worldPos)const;
    //選択した座標にブロックが存在するか
    bool CheckBlockSelectedPos(const glm::vec3& Pos)const;
    //
    bool GetBlockPosition(const glm::vec3& pos,glm::vec3& blockPosition)const;
    bool DeleteBlockSelected(const glm::vec3& Pos);

    bool SaveToFile(const std::string& filepath,const std::string& filename) const;


    glm::vec3 parseLine (const std::string& line, std::size_t lineNumber) const;
    std::vector<glm::vec3> readCoordinatesFromFile(const std::string& filepath) const;
    bool LoadCubeStateFromFile(const std::string& filepath);
private:
    bool IsOccupied(const glm::vec3& worldPos) const;
    //ファイルの処理

    CollisionMesh& collisionMesh;
    float cubeSize;
    std::vector<PlacedBlock> blocks;
};
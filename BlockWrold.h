#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "Shapes/Cube.h"
#include "Shapes/Plane.h"
#include "Collision/CollisionMesh.h"
#include <fstream>
#include <sstream>
#include <iostream>
class Shader;
class Object;

// 配置済みブロック1個分の情報(位置、種類、サイズ、Objectの実体をまとめて保持)
struct PlacedBlock
{
    glm::vec3 position;
    std::string objectType;  // "Cube", "Plane" など
    float objectSize;        // オブジェクトのサイズ
    std::unique_ptr<class Object> object;  // CubeやPlaneなどの基底クラス
};

class BlockWorld
{
public:
    explicit BlockWorld(CollisionMesh& collisionMesh, float cubeSize = 1.0f);

    // worldPosにオブジェクトを配置する（objectTypeとsizeで種類とサイズを指定）
    // @param worldPos ワールド座標
    // @param objectType オブジェクト種類（"Cube"、"Plane"など）
    // @param objectSize オブジェクトのサイズ（デフォルト: 1.0f）
    void PlaceBlock(const glm::vec3& worldPos, const std::string& objectType = "Cube", float objectSize = 1.0f);

    void DrawAll(Shader& shader);

    //ワールド座標をせる座標に
    glm::vec3 SnapToGrid(const glm::vec3& worldPos)const;
    //選択した座標にブロックが存在するか
    bool CheckBlockSelectedPos(const glm::vec3& Pos)const;

    bool GetBlockPosition(const glm::vec3& pos,glm::vec3& blockPosition)const;
    //座標のブロックを削除
    bool DeleteBlockSelected(const glm::vec3& Pos);

    bool SaveToFile(const std::string& filepath,const std::string& filename) const;
    float Getcubesize() {return cubeSize;};

    glm::vec3 parseLine (const std::string& line, std::size_t lineNumber) const;
    std::vector<glm::vec3> readCoordinatesFromFile(const std::string& filepath) const;
    
    // 拡張フォーマット: "x,y,z,objectType,size" をパースする
    struct ParsedBlockData {
        glm::vec3 position;
        std::string objectType;
        float objectSize;
    };
    ParsedBlockData parseBlockLine(const std::string& line, std::size_t lineNumber) const;
    std::vector<ParsedBlockData> readBlockDataFromFile(const std::string& filepath) const;
    
    bool LoadCubeStateFromFile(const std::string& filepath);
private:
    bool IsOccupied(const glm::vec3& worldPos) const;
    //ファイルの処理

    CollisionMesh& collisionMesh;
    float cubeSize;
    std::vector<PlacedBlock> blocks;
};
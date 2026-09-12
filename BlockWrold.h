#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "Shapes/Cube.h"
#include "Shapes/Plane.h"
#include "Entity/Goal.h"
#include "Collision/CollisionMesh.h"
#include "Entity/Object.h"
#include <fstream>
#include <sstream>
#include <iostream>
class Shader;
class Object;

// 配置済みブロック1個分の情報(位置、種類、サイズ、Objectの実体をまとめて保持)
struct PlacedBlock {
    glm::vec3 position;
    std::string objectType;
    glm::vec3 objectSize;
    std::unique_ptr<Object> object;
    bool isShown;
};


class BlockWorld
{
public:
    explicit BlockWorld(CollisionMesh& collisionMesh, float cubeSize = 1.0f);

    void SetCubeTextures(GLuint cubeTexture, GLuint wallTexture);

    // worldPosにオブジェクトを配置する（objectTypeとsizeで種類とサイズを指定）
    // @param worldPos ワールド座標
    // @param objectType オブジェクト種類（"Cube"、"Plane"など）
    // @param objectSize オブジェクトのサイズ（デフォルト: 1.0f）
    // @param isShown 描画するかどうかのフラグ（デフォルト: true）
    void PlaceBlock(
        const glm::vec3& worldPos,
        const std::string& objectType = "Cube",
        const glm::vec3& objectSize = {1.0,1.0,1.0},
        bool isShown = true
    );


    void DrawAll(Shader& shader,float deltaTime);
    ObjectEvent CheckPlayerEnter(const AABB& playerAABB);

    //ワールド座標をセル座標に
    glm::vec3 SnapToGrid(const glm::vec3& worldPos)const;
    //選択した座標にブロックが存在するか
    bool CheckBlockSelectedPos(const glm::vec3& Pos)const;
    //ブロックの座標を取得
    bool GetBlockPosition(const glm::vec3& pos,glm::vec3& blockPosition)const;
    //座標のブロックを削除
    bool DeleteBlockSelected(const glm::vec3& Pos,const std::string& objectName);
    //全てのブロックを削除
    bool DeleteAllBlocks();
    //座標のブロックを取得
    PlacedBlock* GetBlockSelected(const glm::vec3& Pos);
    //ファイルにブロックを収納
    bool SaveToFile(const std::string& filepath,const std::string& filename) const;
    //キューブのサイズを取得
    float Getcubesize() {return cubeSize;};
    //オブジェクトをセットする


    glm::vec3 parseLine (const std::string& line, std::size_t lineNumber) const;
    std::vector<glm::vec3> readCoordinatesFromFile(const std::string& filepath) const;
    
    // 拡張フォーマット: "x,y,z,objectType,size" をパースする
    struct ParsedBlockData {
        glm::vec3 position;
        std::string objectType;
        glm::vec3 objectSize;
        bool isShown = true;
    };
    ParsedBlockData parseBlockLine(const std::string& line, std::size_t lineNumber) const;
    std::vector<ParsedBlockData> readBlockDataFromFile(const std::string& filepath) const;

    bool LoadCubeStateFromFile(const std::string& filepath);
private:
    void SetObj(const glm::vec3& worldPos,const std::string& objectType,const glm::vec3& objectSize,bool isShown);
    bool IsOccupied(const glm::vec3& worldPos) const;
    //ファイルの処理
    CollisionMesh& collisionMesh;
    float cubeSize;
    std::vector<PlacedBlock> blocks;
    GLuint cubeTexture = 0;
    GLuint wallTexture = 0;
    //ファイルの列数
    int expectedColumnCount = 8; // PlacedBlockの列数: x, y, z, objectType, size, isShown
};
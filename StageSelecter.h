#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include "Shapes/Cube.h"
#include "glm/glm.hpp"

namespace fs = std::filesystem;
class Shader;
class StageSelecter {
public:
    StageSelecter();
    // ステージの検出と管理
    void detectStages(const std::string& folderPath);
    void handleInput();
    void Draw(  Shader& shader);  
    int GetMaxStageIndex();
    int GetSelectedStageIndex();
    glm::vec3 GetStageCubePosition();
private:
    int selectedStageIndex = 0; // 現在選択中のステージ番号
    int maxStageIndex = 0;       // 検出された最大ステージ番号
    std::vector<std::unique_ptr<Cube>>stageCubes; // ステージを表すキューブのリスト
    glm::vec3 stageCubePosition = {0.0f, 0.0f, 0.0f}; // ステージキューブの位置
};
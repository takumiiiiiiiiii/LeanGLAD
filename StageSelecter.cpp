#include "StageSelecter.h"
#include "Core/Input.h"
#include "Shader.h"
// #include "Game.h"

StageSelecter::StageSelecter() {
    selectedStageIndex = 0;
    maxStageIndex = 0;
}

void StageSelecter::detectStages(const std::string& folderPath) {
    maxStageIndex = -1;
    for (int i = 0; ; ++i) {
        std::string filePath = folderPath + "/" + std::to_string(i) + ".txt";
        if (fs::exists(filePath)) {
            maxStageIndex = i;
            stageCubes.push_back(std::make_unique<Cube>());
            stageCubes.back()->SetTranformPosition(
            {static_cast<float>(i) * -2.0f, 0.0f, 0.0f}
);
        } else {
            break; // 番号が連続しなくなったら終了
        }
    }
}

// キーボード入力処理（GLUT/GLFW等の入力コールバック内で呼び出す）
void StageSelecter::handleInput() {
    // 右キーで次のステージへ
    if (Input::GetJustMoveInput().x>0) {
        if (selectedStageIndex < maxStageIndex) {
            selectedStageIndex++;
        }
    }
    // 左キーで前のステージへ
    if (Input::GetJustMoveInput().x<0) {
        if (selectedStageIndex > 0) {
            selectedStageIndex--;
        }
    }
    stageCubePosition = stageCubes[selectedStageIndex]->GetTransform().GetPosition();
    // Enterキーで決定
    if (Input::IsJumpPressed()) {
        std::string filePath = "stages/" + std::to_string(selectedStageIndex) + ".txt";
    }
}

// 描画処理
void  StageSelecter::Draw(  Shader& shader) {
    // RenderStage();
    for(const auto& cube : stageCubes) {
       cube->Draw(shader);
    }
}
// ステージ番号の取得
int StageSelecter::GetMaxStageIndex(){
    return maxStageIndex;
}
int StageSelecter::GetSelectedStageIndex(){
    return selectedStageIndex;
}

glm::vec3 StageSelecter::GetStageCubePosition() {
    return stageCubePosition;
}


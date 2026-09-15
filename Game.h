#pragma once
#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Entity/Player.h"
#include "Camera.h"
#include "Shader.h"
#include "Shapes/Plane.h"
#include "Collision/CollisionMesh.h"
#include "Shapes/Cube.h"
#include "BlockWrold.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "global.h"
#include "StageEditor.h"
#include "Texture.h"
#include "Entity/Object.h"
#include "StageSelecter.h"
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xpos, double ypos);
enum class GameState
{
    Title,
    StageSelect,
    Playing,
    Goal,
    Result,
    Editor
};

// 画面の初期サイズ（幅と高さ）
class Game{
    public:
    bool isBlocksSlect=false;
    bool isBlocksSlectPrev = false;
    glm::vec3 selectCube;
    GameState state;
    GameState previousState = GameState::Title; // 前フレームの状態を保持
    Player player;
    glm::vec3 playerStartPos = glm::vec3{0.0,2.0,0.0};
    Camera camera;
    Shader shader;
    BlockWorld blockWorld;
    //画像のミックス具合
    float mix;
    //テクスチャ
    std::unique_ptr<Texture> cubeTexture;
    std::unique_ptr<Texture> wallTexture;
    std::unique_ptr<Texture> logoTexture;
    

    //マトリクス
    glm::mat4 projection;
    glm::mat4 view;
    //エディター
    StageEditor stageEditor;
    //ステージセレクター
    StageSelecter stageSelector;
    //コリジョン
    CollisionMesh collisionmesh;
    Game();
    void Initialize();
    void Update(float dt);
    void UpdateTitle(float dt);
    void UpdateStageSelect(float dt);
    void UpdatePlaying(float dt);
    void UpdateEditor(float dt);

    private:
    bool isGoal;
};

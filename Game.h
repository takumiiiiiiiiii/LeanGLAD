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

enum class GameState
{
    Title,
    Playing,
    Goal,
    Result,
    Editor
};
// エディター側の状態(グローバル or Editorクラスのメンバ)
static const char* objectTypes[] = { "Cube", "Plane", "Sphere" }; // 実際の種類名に置き換え
static int currentTypeIndex = 0;
static int value = 1;

class Game{
    public:
    bool isBlocksSlect=false;
    bool isBlocksSlectPrev = false;
    glm::vec3 selectCube;
    GameState state;
    Player player;

    Camera camera;
    Shader shader;
    BlockWorld blockWorld;
    //画像のミックス具合
    float mix;
    //テクスチャ
    unsigned int texture1,texture2;
    // 画面の初期サイズ（幅と高さ）
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;
    //マトリクス
    glm::mat4 projection;
    glm::mat4 view;
    //コリジョン
    CollisionMesh collisionmesh;
    //エディター
    Game();
    void Initialize();
    void Update(float dt);
    void UpdateTitle(float dt);
    void UpdatePlaying(float dt);
    void UpdateEditor(float dt);

    private:
    void PlaceBlockByMouse();
    void RemoveBlockByMouse();
};

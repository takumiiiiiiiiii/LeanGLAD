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
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xpos, double ypos);
enum class GameState
{
    Title,
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
    Player player;

    Camera camera;
    Shader shader;
    BlockWorld blockWorld;
    //画像のミックス具合
    float mix;
    //テクスチャ
    unsigned int texture1,texture2;

    //マトリクス
    glm::mat4 projection;
    glm::mat4 view;
    //エディター
    StageEditor stageEditor;
    //コリジョン
    CollisionMesh collisionmesh;
    Game();
    void Initialize();
    void Update(float dt);
    void UpdateTitle(float dt);
    void UpdatePlaying(float dt);
    void UpdateEditor(float dt);

    private:
    void PlaceBlockByMouse();
    void RemoveBlockByMouse();
    void SelectBlockByMouse();
};

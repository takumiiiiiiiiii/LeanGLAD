#pragma once
#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Entity/Player.h"
#include "Camera.h"
#include "Shader.h"

enum class GameState
{
    Title,
    Playing,
    Goal,
    Result
};

class Game{
    public:
    GameState state;
    Player player;
    Camera camera;
    Shader shader;
    //画像のミックス具合
    float mix;
    //テクスチャ
    unsigned int texture1,texture2;
    // 画面の初期サイズ（幅と高さ）
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;

    Game();
    void Initialize();
    void Update(float dt);
    void UpdatePlaying(float dt);
};
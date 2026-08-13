#define GL_SILENCE_DEPRECATION
#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include "Shader.h"
#include "stb_image.h"
#include "Camera.h"
#include "Entity/Player.h"
#include "Objects.h"
#include "Shapes/Plane.h"
#include "Shapes/Cube.h"
#include "Core/Input.h"
#include "Entity/Transform.h"
#include "Entity/Player.h"
#include "Game.h"

// ウィンドウサイズ変更時に呼ばれるコールバック関数（ウィンドウサイズが変わっても描画範囲を追従させる）
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// キーボード入力を処理する関数（特定のキーが押されたかチェックする）
void processInput(GLFWwindow *window);

void cursor_enter_callback(GLFWwindow* window, int entered);

//マウスがどんな入力をされたか
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

// 画面の初期サイズ（幅と高さ）
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;



//カメラ場所
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

//カメラ移動
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//時間
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame



int main()
{
    // GLFWライブラリの初期化処理
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // OpenGLのバージョンとプロファイルの設定（ここではOpenGL 3.3 Core Profileを指定）
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // macOS向けの互換性設定（Mac環境の場合のみ実行される）
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // ウィンドウの生成（サイズ、タイトル、モニター指定などを設定）
    GLFWwindow* window = glfwCreateWindow(
        SCR_WIDTH,
        SCR_HEIGHT,
        "LearnOpenGL",
        nullptr,
        nullptr
    );

    // ウィンドウの生成に失敗した場合のエラー処理
    if (window == nullptr)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate(); // 初期化したGLFWの終了処理
        return -1;
    }

    // 作成したウィンドウを現在のOpenGL描画対象（コンテキスト）に設定
    glfwMakeContextCurrent(window);
    //ウィンドウのカーソル入退室記録
    glfwSetCursorEnterCallback(window, cursor_enter_callback);
    //カーソル座標をコールバック
    glfwSetCursorPosCallback(window, mouse_callback);
    Input::SetWindow(window);
    // GLADの初期化。これを行わないと最初のOpenGL関数呼び出しでクラッシュする
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }
    
    // ウィンドウサイズが変更されたときに呼び出す関数（コールバック）を登録
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glEnable(GL_DEPTH_TEST);

    Game game;
    game.Initialize();

    // ウィンドウが閉じられる指示が出るまで、メインループを繰り返す
    while (!glfwWindowShouldClose(window))
    {
        // エスケープキーなどの入力入力を監視・処理
        processInput(window);
        //デルタタイムを取得
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;  
        //メインゲーム
        game.UpdatePlaying(deltaTime);
        // カラーバッファを入れ替えて、描画した内容を実際に画面に表示（ダブルバッファリング）
        glfwSwapBuffers(window);
        // キーボードやマウスの操作などのイベントを検知・処理
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    // glDeleteVertexArrays(1, &VAO[0]);
    // glDeleteBuffers(1, &VBO[0]);
    // glDeleteBuffers(1, &EBO);
    // glDeleteVertexArrays(1, &VAO[1]);
    // glDeleteBuffers(1, &VBO[1]);

    // メインループ終了後、ウィンドウを破棄
    glfwDestroyWindow(window);
    
    // GLFWの残りのリソースを解放して終了
    glfwTerminate();
    return 0;
}

// キーボード入力を処理する関数の本体
void processInput(GLFWwindow *window)
{

    // エスケープキー（ESCAPE）が押された場合、ウィンドウを閉じるフラグを立てる
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window,GLFW_KEY_K) == GLFW_PRESS)
        //mix +=0.1f;
    if (glfwGetKey(window,GLFW_KEY_M) == GLFW_PRESS)
        //mix -=0.1f;
    const float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
        
}


bool cursorInsideWindow = false;

void cursor_enter_callback(GLFWwindow* window, int entered)
{
    cursorInsideWindow = entered == GLFW_TRUE;
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    if (!cursorInsideWindow)
        return;
    std::cout << "Mouse" << std::endl;
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    //camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}



// ウィンドウサイズ変更時に呼ばれる関数の本体
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // 新しいウィンドウサイズに合わせて、OpenGLが描画する領域（ビューポート）を更新
    glViewport(0, 0, width, height);
}

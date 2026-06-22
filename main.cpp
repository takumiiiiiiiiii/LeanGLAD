#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

// ウィンドウサイズ変更時に呼ばれるコールバック関数
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// キーボード入力を処理する関数
void processInput(GLFWwindow *window);

// ウィンドウサイズ
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // GLFWライブラリを初期化
    glfwInit();

    // 使用するOpenGLのバージョンを3.3に設定
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Core Profile（モダンOpenGL）を使用
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    // macOSではForward Compatibleを有効にする必要がある
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    // ウィンドウを生成
    GLFWwindow* window = glfwCreateWindow(
        SCR_WIDTH,
        SCR_HEIGHT,
        "LearnOpenGL",
        NULL,
        NULL
    );

    // ウィンドウ生成に失敗した場合
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // 作成したウィンドウのOpenGLコンテキストを現在のスレッドに設定
    glfwMakeContextCurrent(window);

    // ウィンドウサイズ変更時のコールバック関数を登録
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // GLADを初期化してOpenGL関数のアドレスを取得
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // メインループ
    while (!glfwWindowShouldClose(window))
    {
        // キーボード入力を処理
        processInput(window);
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // バックバッファとフロントバッファを入れ替えて画面を更新
        glfwSwapBuffers(window);

        // イベント（キー入力・マウス操作など）を処理
        glfwPollEvents();
    }

    // GLFWで確保したリソースを解放
    glfwTerminate();

    return 0;
}

// キーボード入力を処理する関数
void processInput(GLFWwindow *window)
{
    // ESCキーが押されたらウィンドウを閉じる
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// ウィンドウサイズ変更時に自動的に呼ばれる関数
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // OpenGLの描画領域（ビューポート）を新しいサイズに合わせる
    // Retinaディスプレイではwidth, heightが実際のピクセル数になる
    glViewport(0, 0, width, height);
}
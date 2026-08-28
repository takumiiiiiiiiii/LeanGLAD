#pragma once
#include <GLFW/glfw3.h>

class Input
{
public:
    static void SetWindow(GLFWwindow* w);
    static void Update(); // ★毎フレーム呼ぶ更新処理を追加

    // Key
    static bool IsKeyPressed(int key);
    static bool IsKeyJustPressed(int key);

    // Mouse
    static bool IsMouseButtonPressed(int button);
    static bool IsMouseButtonJustPressed(int button);
    static void GetMousePosition(double& x, double& y);

private:
    static GLFWwindow* window;

    // 前フレームと現フレームの状態を保持して比較する
    static bool currentKeys[GLFW_KEY_LAST + 1];
    static bool previousKeys[GLFW_KEY_LAST + 1];

    static bool currentMouseButtons[GLFW_MOUSE_BUTTON_LAST + 1];
    static bool previousMouseButtons[GLFW_MOUSE_BUTTON_LAST + 1];
};

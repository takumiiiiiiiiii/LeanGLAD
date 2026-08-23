#pragma once
#include <GLFW/glfw3.h>

class Input
{
public:
    static void SetWindow(GLFWwindow* w);

    static bool IsKeyPressed(int key);

    // マウス
    static bool IsMouseButtonPressed(int button);     // 押されている間ずっとtrue
    static bool IsMouseButtonJustPressed(int button);  // 押された瞬間だけtrue(1回読むとリセット)
    static void GetMousePosition(double& x, double& y);

private:
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

    static GLFWwindow* window;
    static bool mouseJustPressed[GLFW_MOUSE_BUTTON_LAST + 1];
};
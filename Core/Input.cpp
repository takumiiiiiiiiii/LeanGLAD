#include "Input.h"

GLFWwindow* Input::window = nullptr;

bool Input::currentKeys[GLFW_KEY_LAST + 1] = { false };
bool Input::previousKeys[GLFW_KEY_LAST + 1] = { false };

bool Input::currentMouseButtons[GLFW_MOUSE_BUTTON_LAST + 1] = { false };
bool Input::previousMouseButtons[GLFW_MOUSE_BUTTON_LAST + 1] = { false };

void Input::SetWindow(GLFWwindow* w)
{
    window = w;
}

// ★毎フレームのループ先頭（glfwPollEventsの直後）で必ず呼び出す
void Input::Update()
{
    if (!window) return;

    // キーボードの状態を更新
    for (int i = 0; i <= GLFW_KEY_LAST; ++i)
    {
        previousKeys[i] = currentKeys[i];
        currentKeys[i] = (glfwGetKey(window, i) == GLFW_PRESS);
    }

    // マウスボタンの状態を更新
    for (int i = 0; i <= GLFW_MOUSE_BUTTON_LAST; ++i)
    {
        previousMouseButtons[i] = currentMouseButtons[i];
        currentMouseButtons[i] = (glfwGetMouseButton(window, i) == GLFW_PRESS);
    }
}

// 押しっぱなし判定
bool Input::IsKeyPressed(int key)
{
    if (key < 0 || key > GLFW_KEY_LAST) return false;
    return currentKeys[key];
}

// ★押した瞬間のみ判定 (前フレーム=離していて、現フレーム=押されている)
bool Input::IsKeyJustPressed(int key)
{
    if (key < 0 || key > GLFW_KEY_LAST) return false;
    return currentKeys[key] && !previousKeys[key];
}

// マウス押しっぱなし判定
bool Input::IsMouseButtonPressed(int button)
{
    if (button < 0 || button > GLFW_MOUSE_BUTTON_LAST) return false;
    return currentMouseButtons[button];
}

// ★マウスを押した瞬間のみ判定
bool Input::IsMouseButtonJustPressed(int button)
{
    if (button < 0 || button > GLFW_MOUSE_BUTTON_LAST) return false;
    return currentMouseButtons[button] && !previousMouseButtons[button];
}

void Input::GetMousePosition(double& x, double& y)
{
    if (window) {
        glfwGetCursorPos(window, &x, &y);
    }
}
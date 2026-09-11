#include "Input.h"
#include <algorithm>
#include <iostream>
GLFWwindow* Input::window = nullptr;

bool Input::currentKeys[GLFW_KEY_LAST + 1] = { false };
bool Input::previousKeys[GLFW_KEY_LAST + 1] = { false };

bool Input::currentMouseButtons[GLFW_MOUSE_BUTTON_LAST + 1] = { false };
bool Input::previousMouseButtons[GLFW_MOUSE_BUTTON_LAST + 1] = { false };

unsigned char Input::currentJoystickButtons[15] = { 0 };
unsigned char Input::previousJoystickButtons[15] = { 0 };
int Input::joystickButtonCount = 0;

//ゲームパッドの入力
GLFWgamepadstate Input::currentGamepadState = {};
GLFWgamepadstate Input::previousGamepadState = {};
bool Input::gamepadConnected = false;
StickInput Input::currentLeftStick{0.0f, 0.0f};
StickInput Input::previousLeftStick{0.0f, 0.0f};

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

    for (int i = 0; i < 15; ++i)
    {
        previousJoystickButtons[i] = currentJoystickButtons[i];
        currentJoystickButtons[i] = 0;
    }
    joystickButtonCount = 0;

    // ゲームパッド更新
    previousGamepadState = currentGamepadState;
    gamepadConnected = false;
    //ジョイスティック更新
    previousLeftStick = currentLeftStick;
    currentLeftStick = GetLeftStickInput();

    for (int i = GLFW_JOYSTICK_1; i <= GLFW_JOYSTICK_LAST; ++i)
    {
        if (!glfwJoystickPresent(i))
            continue;

        int buttonCount = 0;
        const unsigned char* buttons = glfwGetJoystickButtons(i, &buttonCount);
        if (buttons && buttonCount > 0)
        {
            std::cout << "Joystick " << i << " buttons:";
            joystickButtonCount = std::min(buttonCount, 15);
            for (int j = 0; j < joystickButtonCount; ++j)
            {
                currentJoystickButtons[j] = buttons[j];
            }
            for (int b = 0; b < std::min(buttonCount, 10); ++b)
            {
                std::cout << " [" << b << "]=" << (int)buttons[b];
            }
            std::cout << std::endl;
        }

        if (glfwJoystickIsGamepad(i) && glfwGetGamepadState(i, &currentGamepadState))
        {
            gamepadConnected = true;
            return;
        }
    }

    gamepadConnected = false;
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

bool Input::IsJumpPressed()
{
    bool gamepadJump = false;
    if (gamepadConnected)
    {
        gamepadJump = IsGamepadButtonPressed(GLFW_GAMEPAD_BUTTON_A);
    }
    else if (joystickButtonCount > 0)
    {
        gamepadJump = (currentJoystickButtons[0] == GLFW_PRESS);
    }

    return IsKeyPressed(GLFW_KEY_SPACE) || gamepadJump;
}

bool Input::IsJumpJustPressed()
{
    bool gamepadJump = false;
    if (gamepadConnected)
    {
        gamepadJump = IsGamepadButtonJustPressed(GLFW_GAMEPAD_BUTTON_A);
    }
    else if (joystickButtonCount > 0)
    {
        gamepadJump = (currentJoystickButtons[0] == GLFW_PRESS) &&
                      (previousJoystickButtons[0] != GLFW_PRESS);
    }

    return IsKeyJustPressed(GLFW_KEY_SPACE) || gamepadJump;
}

bool Input::IsBecomeBlockJustPressed()
{
    bool gamepadTrigger = false;

    if (gamepadConnected)
    {
        gamepadTrigger = IsGamepadButtonJustPressed(GLFW_GAMEPAD_BUTTON_X);
    }
    else if (joystickButtonCount > 0)
    {
        gamepadTrigger = (currentJoystickButtons[3] == GLFW_PRESS) &&
                         (previousJoystickButtons[3] != GLFW_PRESS);
    }

    return IsKeyJustPressed(GLFW_KEY_B) || gamepadTrigger;
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

void Input::GetMousePosition(double& x, double& y){
    if (!window)
    {
        x = 0.0;
        y = 0.0;
        return;
    }
    
    glfwGetCursorPos(window, &x, &y);

    int windowWidth, windowHeight;
    int framebufferWidth, framebufferHeight;

    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);

    if (windowWidth > 0 && windowHeight > 0)
    {
        x *= static_cast<double>(framebufferWidth) / windowWidth;
        y *= static_cast<double>(framebufferHeight) / windowHeight;
    }
}
//ゲームパッド

bool Input::IsGamepadConnected()
{
    return gamepadConnected;
}

bool Input::IsGamepadButtonPressed(int button)
{
    return gamepadConnected && currentGamepadState.buttons[button] == GLFW_PRESS;
}

bool Input::IsGamepadButtonJustPressed(int button)
{
    return gamepadConnected &&
           currentGamepadState.buttons[button] == GLFW_PRESS &&
           previousGamepadState.buttons[button] != GLFW_PRESS;
}

float Input::GetGamepadAxis(int axis)
{
    if (!gamepadConnected) return 0.0f;
    return currentGamepadState.axes[axis];
}

StickInput Input::GetLeftStickInput()
{
    StickInput result{0.0f, 0.0f};

    if (gamepadConnected)
    {
        result.x = GetGamepadAxis(GLFW_GAMEPAD_AXIS_LEFT_X);
        result.y = -GetGamepadAxis(GLFW_GAMEPAD_AXIS_LEFT_Y);
    }
    else
    {
        for (int jid = GLFW_JOYSTICK_1; jid <= GLFW_JOYSTICK_LAST; ++jid)
        {
            if (!glfwJoystickPresent(jid)) continue;

            int axisCount = 0;
            const float* axes = glfwGetJoystickAxes(jid, &axisCount);
            if (axes && axisCount >= 2)
            {
                result.x = axes[0];
                result.y = -axes[1];
                break;
            }
        }
    }

    if (std::abs(result.x) < 0.15f) result.x = 0.0f;
    if (std::abs(result.y) < 0.15f) result.y = 0.0f;

    return result;
}

StickInput Input::GetLeftStickJustInput()
{
   const float deadzone = 0.68f;

    StickInput current = currentLeftStick;
    float currentLen = std::sqrt(current.x * current.x + current.y * current.y);

    float previousLen = std::sqrt(previousLeftStick.x * previousLeftStick.x +
                                 previousLeftStick.y * previousLeftStick.y);

    if (previousLen <= deadzone && currentLen > deadzone)
    {
        return current;
    }

    return StickInput{0.0f, 0.0f};
}

StickInput Input::GetRightStickInput()
{
    StickInput result{0.0f, 0.0f};

    if (gamepadConnected)
    {
        result.x = GetGamepadAxis(GLFW_GAMEPAD_AXIS_RIGHT_X);
        result.y = -GetGamepadAxis(GLFW_GAMEPAD_AXIS_RIGHT_Y);
    }
    else
    {
        for (int jid = GLFW_JOYSTICK_1; jid <= GLFW_JOYSTICK_LAST; ++jid)
        {
            if (!glfwJoystickPresent(jid)) continue;

            int axisCount = 0;
            const float* axes = glfwGetJoystickAxes(jid, &axisCount);
            if (axes && axisCount >= 4)
            {
                result.x = axes[2];
                result.y = -axes[3];
                break;
            }
        }
    }

    if (std::abs(result.x) < 0.15f) result.x = 0.0f;
    if (std::abs(result.y) < 0.15f) result.y = 0.0f;

    return result;
}

MoveInput Input::GetMoveInput()
{
    MoveInput result{0.0f, 0.0f};

    // キーボード
    if (IsKeyPressed(GLFW_KEY_A)) result.x -= 1.0f;
    if (IsKeyPressed(GLFW_KEY_D)) result.x += 1.0f;
    if (IsKeyPressed(GLFW_KEY_W)) result.y += 1.0f;
    if (IsKeyPressed(GLFW_KEY_S)) result.y -= 1.0f;

    // 左スティックが使われていない時だけゲームパッド入力を採用
    StickInput leftStick = GetLeftStickInput();
    if (std::abs(result.x) < 0.01f && std::abs(result.y) < 0.01f)
    {
        result.x = leftStick.x;
        result.y = leftStick.y;
    }

    if (std::abs(result.x) < 0.15f) result.x = 0.0f;
    if (std::abs(result.y) < 0.15f) result.y = 0.0f;

    return result;
}

MoveInput Input::GetJustMoveInput()
{
     MoveInput result{0.0f, 0.0f};

    // キーボードの押した瞬間
    if (IsKeyJustPressed(GLFW_KEY_A)) result.x -= 1.0f;
    if (IsKeyJustPressed(GLFW_KEY_D)) result.x += 1.0f;
    if (IsKeyJustPressed(GLFW_KEY_W)) result.y += 1.0f;
    if (IsKeyJustPressed(GLFW_KEY_S)) result.y -= 1.0f;

    StickInput stick = GetLeftStickJustInput();
    if (std::abs(stick.x) > 0.18f || std::abs(stick.y) > 0.18f)
    {
        result.x += stick.x;
        result.y += stick.y;
    }

    return result;
}
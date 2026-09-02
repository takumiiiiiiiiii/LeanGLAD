#pragma once
#include <GLFW/glfw3.h>

struct MoveInput
{
    float x = 0.0f;
    float y = 0.0f;
};

struct StickInput
{
    float x = 0.0f;
    float y = 0.0f;
};

class Input
{
public:
    static void SetWindow(GLFWwindow* w);
    static void Update(); // ★毎フレーム呼ぶ更新処理を追加

    // キーボード
    static bool IsKeyPressed(int key);
    static bool IsKeyJustPressed(int key);

    // アクション入力
    static bool IsJumpPressed();
    static bool IsJumpJustPressed();
    static bool IsBecomeBlockJustPressed();
    // マウス
    static bool IsMouseButtonPressed(int button);
    static bool IsMouseButtonJustPressed(int button);
    static void GetMousePosition(double& x, double& y);

    //ゲームパッド
    static bool IsGamepadConnected();
    static bool IsGamepadButtonPressed(int button);
    static bool IsGamepadButtonJustPressed(int button);
    static float GetGamepadAxis(int axis);
    static StickInput GetLeftStickInput();
    static StickInput GetLeftStickJustInput();
    static StickInput GetRightStickInput();

    //キーボードゲームパッド共通
    static MoveInput GetMoveInput();
    static MoveInput GetJustMoveInput();


private:
    static GLFWwindow* window;

    // 前フレームと現フレームの状態を保持して比較する
    static bool currentKeys[GLFW_KEY_LAST + 1];
    static bool previousKeys[GLFW_KEY_LAST + 1];

    static bool currentMouseButtons[GLFW_MOUSE_BUTTON_LAST + 1];
    static bool previousMouseButtons[GLFW_MOUSE_BUTTON_LAST + 1];

    static unsigned char currentJoystickButtons[15];
    static unsigned char previousJoystickButtons[15];
    static int joystickButtonCount;

    //ゲームパッドの入力
    static GLFWgamepadstate currentGamepadState;
    static GLFWgamepadstate previousGamepadState;
    static bool gamepadConnected;
    static StickInput currentLeftStick;
    static StickInput previousLeftStick;
};

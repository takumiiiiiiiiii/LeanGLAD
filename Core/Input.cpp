#include "Input.h"
#include <iostream>
GLFWwindow* Input::window = nullptr;
bool Input::mouseJustPressed[GLFW_MOUSE_BUTTON_LAST + 1] = { false };

void Input::SetWindow(GLFWwindow* w)
{
    window = w;
    glfwSetMouseButtonCallback(window, Input::MouseButtonCallback);
}

bool Input::IsKeyPressed(int key)
{
    return glfwGetKey(window, key) == GLFW_PRESS;
}

bool Input::IsMouseButtonPressed(int button)
{
    return glfwGetMouseButton(window, button) == GLFW_PRESS;
}

bool Input::IsMouseButtonJustPressed(int button)
{
    if (button < 0 || button > GLFW_MOUSE_BUTTON_LAST)
        return false;

    bool result = mouseJustPressed[button];
    mouseJustPressed[button] = false; // 読んだら消費する
    return result;
}

void Input::GetMousePosition(double& x, double& y)
{
    glfwGetCursorPos(window, &x, &y);
        std::cout<<"Clicked screen pos: ("
                  << x<< ", " << y << ")" << std::endl;
}

void Input::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS && button <= GLFW_MOUSE_BUTTON_LAST)
    {
        mouseJustPressed[button] = true;
    }
}
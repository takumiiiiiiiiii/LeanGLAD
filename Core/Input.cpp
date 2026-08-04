#include "Input.h"
#include <iostream>
GLFWwindow* Input::window = nullptr;

void Input::SetWindow(GLFWwindow* w)
{
    window = w;
}

bool Input::IsKeyPressed(int key)
{
    
    return glfwGetKey(window, key) == GLFW_PRESS;
}
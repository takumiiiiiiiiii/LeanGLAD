#pragma once
#include <GLFW/glfw3.h>
class Input
{
public:
    static bool IsKeyPressed(int key);
    static void SetWindow(GLFWwindow*window);
private:
    static GLFWwindow* window;
};
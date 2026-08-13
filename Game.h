#pragma once
#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Entity/Player.h"
#include "Camera.h"

class Game{
    public:
    Player player;
    Camera camera;

    void Initialize();
    void Update(float dt);
};
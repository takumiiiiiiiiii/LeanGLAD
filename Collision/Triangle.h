#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Triangle{
    glm::vec3 v0,v1,v2;
    glm::vec3 normal;
};
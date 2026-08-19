#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Triangle.h"
bool intersectRayTriangle(const glm::vec3& orig, const glm::vec3& dir, const Triangle& tri, float& t);
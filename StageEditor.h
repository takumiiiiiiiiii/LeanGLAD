#pragma once
#include "Collision/CollisionMesh.h"
#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Core/Input.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "BlockWrold.h"
#include "global.h"

static const char* objectTypes[] = { "Cube", "Plane", "Wall" }; // 実際の種類名に置き換え
static const char* editorStates[] = { "Place", "Remove", "Select" };

struct RayCrossInformation{
    bool ishit;
    glm::vec3 hitPos;
    glm::vec3 normal;
    float dist;
};

// エディター側の状態(グローバル or Editorクラスのメンバ)
class StageEditor {
    public:
    StageEditor();
    void UpdateEditor(float dt);
    void PlaceBlockByMouse(CollisionMesh& collisionmesh, BlockWorld& blockWorld, const glm::mat4& view, const glm::mat4& projection);
    void RemoveBlockByMouse(CollisionMesh& collisionmesh, BlockWorld& blockWorld, const glm::mat4& view, const glm::mat4& projection);
    void SelectBlockByMouse(CollisionMesh& collisionmesh, BlockWorld& blockWorld, const glm::mat4& view, const glm::mat4& projection);
    RayCrossInformation GetClickedWorldPosInformation(CollisionMesh& collisionmesh,const glm::mat4& view, const glm::mat4& projection);
    PlacedBlock* selectBlock;
    private:
    int currentTypeIndex = 0;
    int currentStateIndex = 0;
    glm::vec3 value = {1.0f,1.0f,1.0f};
};
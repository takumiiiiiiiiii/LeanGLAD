#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "../Entity/Object.h"
#include "../Collision/CollisionMesh.h"
class Shader;
class Cube : public Object
{
public:
    explicit Cube(float size = 1.0f);
    ~Cube();
    // コピーを禁止して二重解放を防ぐ
    Cube(const Cube&) = delete;
    Cube& operator=(const Cube&) = delete;
    void SetTexture(GLuint textureID);

    void Draw(Shader& shader) override;
    void RegisterCollision(CollisionMesh& collision) const; 
    void SetTranformPosition(glm::vec3 transformPosition);
private:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    std::vector<glm::vec3> localPositions; // 衝突判定用:positionのみ
    std::vector<unsigned int> indices;     // 描画にもRay判定にも使う
};
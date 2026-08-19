#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "../Entity/Object.h"
#include "../Collision/CollisionMesh.h"
class Shader;

class Plane : public Object
{
public:
    explicit Plane(float size = 10.0f, const Transform& initialTransform = Transform());
    ~Plane();
    // コピーを禁止して二重解放を防ぐ
    Plane(const Plane&) = delete;
    Plane& operator=(const Plane&) = delete;

    void SetTexture(GLuint textureID);
    void Draw(Shader& shader) override;
    void RegisterCollision(CollisionMesh& collision) const; 
private:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    std::vector<glm::vec3> localPositions; // 衝突判定用:positionのみ
    std::vector<unsigned int> indices;     // 描画にもRay判定にも使う
};

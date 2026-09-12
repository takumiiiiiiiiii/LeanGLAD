#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "../Entity/Object.h"
#include "../Collision/CollisionMesh.h"
class Shader;
class Cube : public Object
{
public:
    explicit Cube(float input_size = 1.0f, GLuint textureID = 0);
    ~Cube();
    // 静的なCube ID カウンター
    static uint32_t nextCubeId;
    uint32_t GetCubeId() const { return cubeId; }
    // コピーを禁止して二重解放を防ぐ
    Cube(const Cube&) = delete;
    Cube& operator=(const Cube&) = delete;
    void SetTexture(GLuint textureID);

    void CubeInit(float input_size = 1.0f);
    void Draw(Shader& shader) override;
    void RegisterCollision(CollisionMesh& collision) const;
    void RegisterCollisionAndStoreIndices(CollisionMesh& collision);
    void SetScale(glm::vec3 scale);
    void SetTranformPosition(glm::vec3 transformPosition);
    glm::vec3 GetScale();
private:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    GLuint textureID = 0;

    uint32_t cubeId;                       // このCubeの一意なID
    float size = 0;
    std::vector<glm::vec3> localPositions; // 衝突判定用:positionのみ
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;     // 描画にもRay判定にも使う
};
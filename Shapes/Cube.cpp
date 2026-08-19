#include "Cube.h"
#include "../Shader.h"
Cube::Cube(float size)
{
    float s = size * 0.5f;

    // Position(x,y,z)      Normal(x,y,z)      TexCoord(u,v)
    float vertices[] = {
        // Position(x,y,z)      Normal(x,y,z)      TexCoord(u,v)
        //
      // Front (-Z)
    -s, -s, -s,  0.0f, 0.0f,-1.0f, 0.0f,0.0f,
    s, -s, -s,  0.0f, 0.0f,-1.0f, 1.0f,0.0f,
    s,  s, -s,  0.0f, 0.0f,-1.0f, 1.0f,1.0f,
    s,  s, -s,  0.0f, 0.0f,-1.0f, 1.0f,1.0f,
    -s,  s, -s,  0.0f, 0.0f,-1.0f, 0.0f,1.0f,
    -s, -s, -s,  0.0f, 0.0f,-1.0f, 0.0f,0.0f,

    // Back (+Z)
    -s, -s,  s,  0.0f, 0.0f, 1.0f, 0.0f,0.0f,
    s, -s,  s,  0.0f, 0.0f, 1.0f, 1.0f,0.0f,
    s,  s,  s,  0.0f, 0.0f, 1.0f, 1.0f,1.0f,
    s,  s,  s,  0.0f, 0.0f, 1.0f, 1.0f,1.0f,
    -s,  s,  s,  0.0f, 0.0f, 1.0f, 0.0f,1.0f,
    -s, -s,  s,  0.0f, 0.0f, 1.0f, 0.0f,0.0f,

    // Left (-X)
    -s,  s,  s, -1.0f, 0.0f, 0.0f, 1.0f,0.0f,
    -s,  s, -s, -1.0f, 0.0f, 0.0f, 1.0f,1.0f,
    -s, -s, -s, -1.0f, 0.0f, 0.0f, 0.0f,1.0f,
    -s, -s, -s, -1.0f, 0.0f, 0.0f, 0.0f,1.0f,
    -s, -s,  s, -1.0f, 0.0f, 0.0f, 0.0f,0.0f,
    -s,  s,  s, -1.0f, 0.0f, 0.0f, 1.0f,0.0f,

    // Right (+X)
    s,  s,  s,  1.0f, 0.0f, 0.0f, 1.0f,0.0f,
    s,  s, -s,  1.0f, 0.0f, 0.0f, 1.0f,1.0f,
    s, -s, -s,  1.0f, 0.0f, 0.0f, 0.0f,1.0f,
    s, -s, -s,  1.0f, 0.0f, 0.0f, 0.0f,1.0f,
    s, -s,  s,  1.0f, 0.0f, 0.0f, 0.0f,0.0f,
    s,  s,  s,  1.0f, 0.0f, 0.0f, 1.0f,0.0f,

    // Bottom (-Y)
    -s, -s, -s,  0.0f,-1.0f, 0.0f, 0.0f,1.0f,
    s, -s, -s,  0.0f,-1.0f, 0.0f, 1.0f,1.0f,
    s, -s,  s,  0.0f,-1.0f, 0.0f, 1.0f,0.0f,
    s, -s,  s,  0.0f,-1.0f, 0.0f, 1.0f,0.0f,
    -s, -s,  s,  0.0f,-1.0f, 0.0f, 0.0f,0.0f,
    -s, -s, -s,  0.0f,-1.0f, 0.0f, 0.0f,1.0f,

    // Top (+Y)
    -s,  s, -s,  0.0f, 1.0f, 0.0f, 0.0f,1.0f,
    s,  s, -s,  0.0f, 1.0f, 0.0f, 1.0f,1.0f,
    s,  s,  s,  0.0f, 1.0f, 0.0f, 1.0f,0.0f,
    s,  s,  s,  0.0f, 1.0f, 0.0f, 1.0f,0.0f,
    -s,  s,  s,  0.0f, 1.0f, 0.0f, 0.0f,0.0f,
    -s,  s, -s,  0.0f, 1.0f, 0.0f, 0.0f,1.0f,
    };

    // 衝突判定用にpositionだけ抜き出して保持(描画用配列とは別物)
    localPositions.reserve(sizeof(vertices) / (8 * sizeof(float)));
    for (size_t i = 0; i < sizeof(vertices) / sizeof(float); i += 8) {
        localPositions.emplace_back(vertices[i], vertices[i + 1], vertices[i + 2]);
    }
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(vertices),
                 vertices,
                 GL_STATIC_DRAW);


    // Position
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    // Normal
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8 * sizeof(float),(void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8 * sizeof(float),(void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

Cube::~Cube()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Cube::Draw(Shader& shader)
{
    shader.setMat4("model", transform.GetModelMatrix());
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void Cube::SetTexture(GLuint textureID)
{
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Cube::SetTranformPosition(glm::vec3 position){
    this->transform.SetPosition(position);
}

void Cube::RegisterCollision(CollisionMesh& collision) const
{
    glm::mat4 model = transform.GetModelMatrix();

    // ローカル座標をワールド座標へ変換
    std::vector<glm::vec3> worldPositions;
    worldPositions.reserve(localPositions.size());
    for (const auto& p : localPositions) {
        worldPositions.push_back(glm::vec3(model * glm::vec4(p, 1.0f)));
    }

    // // indicesを3つずつ読んで三角形として登録
    for (size_t i = 0; i + 2 < localPositions.size(); i += 3) {
        collision.addTriangle(
            worldPositions[i],
            worldPositions[i+1],
            worldPositions[i+2]
        );
    }
}
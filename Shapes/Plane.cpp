#include "Plane.h"
#include "../Shader.h"
#include <iostream>

Plane::Plane(float size, const Transform& initialTransform)
    : Object(initialTransform)
{
    float s = size * 0.5f;

    // Position(x,y,z)      Normal(x,y,z)      TexCoord(u,v)
    float vertices[] =
    {
        -s, 0.0f, -s,       0,1,0,            0,0,
         s, 0.0f, -s,       0,1,0,            1,0,
         s, 0.0f,  s,       0,1,0,            1,1,
        -s, 0.0f,  s,       0,1,0,            0,1
    };

    // unsigned int indices[] =
    // {
    //     0,1,2,
    //     2,3,0
    // };
    indices.assign({
    0, 1, 2,
    2, 3, 0
    });

    // 衝突判定用にpositionだけ抜き出して保持(描画用配列とは別物)
    localPositions = {
        {-s, 0.0f, -s},
        { s, 0.0f, -s},
        { s, 0.0f,  s},
        {-s, 0.0f,  s}
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(vertices),
                 vertices,
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                indices.size() * sizeof(unsigned int), // データ全体のバイトサイズ
                indices.data(),                        // メモリ先頭へのポインタ
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

Plane::~Plane()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Plane::Draw(Shader& shader)
{
    shader.setMat4("model", transform.GetModelMatrix());
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Plane::SetTexture(GLuint textureID)
{
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Plane::RegisterCollision(CollisionMesh& collision) const
{
    glm::mat4 model = transform.GetModelMatrix();

    // ローカル座標をワールド座標へ変換
    std::vector<glm::vec3> worldPositions;
    worldPositions.reserve(localPositions.size());
    for (const auto& p : localPositions) {
        worldPositions.push_back(glm::vec3(model * glm::vec4(p, 1.0f)));
    }
    // indicesを3つずつ読んで三角形として登録
    for (size_t i = 0; i + 2 < indices.size(); i += 3) {

        collision.addTriangle(
            worldPositions[indices[i]],
            worldPositions[indices[i + 1]],
            worldPositions[indices[i + 2]]
        );
    }
}
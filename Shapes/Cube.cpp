#include "Cube.h"
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

void Cube::Draw()
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void Cube::SetTexture(GLuint textureID)
{
    glBindTexture(GL_TEXTURE_2D, textureID);
}
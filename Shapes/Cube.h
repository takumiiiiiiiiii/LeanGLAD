#include <glad/glad.h>
#include <glm/glm.hpp>

class Cube
{
public:
    Cube(float size = 1.0f);
    ~Cube();
    void SetTexture(GLuint textureID);
    void Draw();

private:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
};
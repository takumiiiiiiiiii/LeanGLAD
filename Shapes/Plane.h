#include <glad/glad.h>
#include <glm/glm.hpp>

class Plane
{
public:
    Plane(float size = 10.0f);
    ~Plane();
    void SetTexture(GLuint textureID);
    void Draw();

private:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
};
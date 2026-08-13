#include <glad/glad.h>
#include <glm/glm.hpp>

class Plane
{
public:
    Plane(float size = 10.0f);
    ~Plane();
    // コピーを禁止して二重解放を防ぐ
    Plane(const Plane&) = delete;
    Plane& operator=(const Plane&) = delete;

    void SetTexture(GLuint textureID);
    void Draw();

private:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
};
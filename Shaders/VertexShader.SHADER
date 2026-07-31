#version 330 core
layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aColor;
layout(location = 1) in vec2 aTexCoord;
out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
//out vec3 vertexPos

void main()
{
    //順番に気をつける
    gl_Position = projection * view * model *vec4(aPos, 1.0);
    // ourColor = aColor;
    TexCoord = aTexCoord;
    //vertexPos = gl_Position;
}
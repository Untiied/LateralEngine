#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform vec3 inColor;
out vec3 outColor;
void main()
{
    gl_Position = Projection * View * Model * vec4((aPos), 1.0f);
    outColor = inColor;
}
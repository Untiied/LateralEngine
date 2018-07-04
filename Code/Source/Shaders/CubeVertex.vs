#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec3 inColor;
layout (location = 3) in mat4 instanceModel;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

out vec3 outColor;
void main()
{
    gl_Position = Projection * View * instanceModel * vec4(aPos, 1.0f);
    outColor = inColor;
}
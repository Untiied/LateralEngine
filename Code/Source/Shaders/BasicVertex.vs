#version 330 core

layout (location = 0) in struct Vertex{
    vec3 aPos;
    vec3 Normal;
    vec2 TexCoords;
} vertex;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform vec3 inColor;
out vec3 outColor;
out vec2 TextureCords;
void main()
{
    gl_Position = Projection * View * Model * vec4((vertex.aPos), 1.0f);
    outColor = vec3(1.0,1.0,1.0);
    TextureCords = vertex.TexCoords;
}
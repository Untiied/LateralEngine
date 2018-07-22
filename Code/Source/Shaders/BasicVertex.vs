#version 330 core

layout (location = 0) in struct Vertex{
    vec3 aPos;
    vec3 Normal;
    vec2 TexCoords;
} vertex;

struct Material{
    vec3 Ambient;
};

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform Material material;

uniform vec3 inColor;
out vec3 outColor;
out vec2 TextureCords;
void main()
{
    gl_Position = Projection * View * Model * vec4((vertex.aPos), 1.0f);
    outColor = material.Ambient;
    TextureCords = vertex.TexCoords;
}
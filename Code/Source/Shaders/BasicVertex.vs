#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoords;

struct Material{
    vec3 Emission;
    vec3 Ambient;
    vec3 Diffuse;
    float shininess;
    vec3 specular;
};

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform Material material;

out vec3 outColor;
out vec2 TextureCords;
void main()
{
    gl_Position = Projection * View * Model * vec4((aPos), 1.0f);
    outColor = material.Emission + material.Ambient;
    TextureCords = vec2(TexCoords.x, TexCoords.y);
}
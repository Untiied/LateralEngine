#version 330 core

out vec4 FragColor;
in vec3 outColor;
in vec2 TextureCords;

uniform sampler2D tex;

void main()
{
    FragColor = texture(tex, TextureCords) * vec4(outColor, 1.0f);
}
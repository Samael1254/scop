#version 460 core

in vec3 normal;
in vec2 texture;

out vec4 FragColor;

void main()
{
    FragColor = vec4(normal, 1.);
}

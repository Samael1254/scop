#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexture;
layout(location = 2) in vec3 aNormal;

uniform mat4x4 proj;
uniform mat4x4 view;
uniform mat4x4 model;

out vec3 normal;
out vec2 texture;

void main()
{
    gl_Position = proj * view * model * vec4(aPos, 1.0);
    normal = aNormal;
    texture = aTexture;
}

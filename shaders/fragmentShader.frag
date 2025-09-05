#version 460 core

in vec3 normal;
in vec2 texture;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform float lightBrightness;
uniform vec3 diffuseColor;

void main()
{
    vec3 lightDir = normalize(lightPosition - FragPos);
    float incidence = max(dot(lightDir, normal), 0.);
    FragColor = vec4(incidence * lightBrightness * lightColor * diffuseColor, 1.0);
}

#version 460 core

in vec3 normalVec;
in vec2 texture;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 lightColor;
uniform float lightBrightness;
uniform vec3 lightPos;
uniform vec3 ambiantColor;
uniform float ambiantBrightness;
uniform vec3 diffuseColor;

void main()
{
    vec3 lightDir = normalize(lightPos - FragPos);
    float incidence = max(dot(lightDir, normalVec), 0.);
    vec3 ambiant = ambiantBrightness * ambiantColor;
    vec3 diffuse = incidence * lightBrightness * lightColor;
    FragColor = vec4((ambiant + diffuse) * diffuseColor, 1.0);
    // FragColor = vec4(normalVec, 1.0);
}

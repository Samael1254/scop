#version 460 core

in vec3 normalVec;
in vec2 texture;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 lightColor;
uniform float lightBrightness;
uniform vec3 lightPos;
uniform vec3 ambiantLightColor;
uniform float ambiantLightBrightness;
uniform vec3 diffuseColor;
uniform bool showTriangles;

float hash(float x)
{
    return fract(sin(x) * 43758.5453123);
}

void main()
{
    vec3 lightDir = normalize(lightPos - FragPos);
    float incidence = max(dot(lightDir, normalVec), 0.);
    vec3 ambiant = ambiantLightBrightness * ambiantLightColor;
    vec3 diffuse = incidence * lightBrightness * lightColor;

    float randomnessSpan = 0.3 * float(showTriangles);
    float idFactor = 1 - randomnessSpan + randomnessSpan * hash(float(gl_PrimitiveID));

    FragColor = vec4((idFactor) * (ambiant + diffuse) * diffuseColor, 1.0);
    // FragColor = vec4(normalVec, 1.0);
}

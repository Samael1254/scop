#version 460 core

in vec3 normalVec;
in vec2 textureCoords;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 lightColor;
uniform float lightBrightness;
uniform vec3 lightPos;
uniform vec3 ambiantLightColor;
uniform float ambiantLightBrightness;

uniform vec3 cameraPos;

uniform vec3 diffuseColor;
uniform vec3 ambiantColor;
uniform vec3 specularColor;
uniform float specularExponent;
uniform sampler2D diffuseTexture;

uniform int displayMode;

float rand(float x)
{
    return fract(sin(x) * 43758.5453123);
}

void main()
{
    vec3 lightDir = normalize(lightPos - FragPos);
    float incidence = max(dot(lightDir, normalVec), 0.);
    vec3 ambiant = ambiantLightBrightness * ambiantLightColor;
    vec3 diffuse = incidence * lightBrightness * lightColor;
    vec3 reflectionDir = normalize(2 * dot(lightDir, normalVec) * normalVec - lightDir);
    vec3 specular = pow(max(dot(reflectionDir, -normalize(cameraPos - FragPos)), 0.0), specularExponent) * lightBrightness * lightColor * specularColor;

    if (displayMode == 0)
        FragColor = vec4(ambiant * ambiantColor + diffuse * diffuseColor + specular * specularColor, 1.0);
    else if (displayMode == 1)
    {
        float randomnessSpan = 0.2;
        float idFactor = 1 - randomnessSpan + randomnessSpan * rand(float(gl_PrimitiveID));
        FragColor = vec4(idFactor * (ambiant * ambiantColor + diffuse * diffuseColor), 1.0);
    }
    else if (displayMode == 2)
    {
        vec4 textureColor = texture(diffuseTexture, textureCoords);
        FragColor = vec4((ambiant * ambiantColor + diffuse * diffuseColor) * vec3(textureColor) + specular * specularColor, 1.0);
    }
    else if (displayMode == 3)
        FragColor = vec4(normalVec, 1.0);
}

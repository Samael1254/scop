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

uniform vec3 diffuseColor;
uniform vec3 ambiantColor;
uniform sampler2D diffuseTexture;

uniform bool showTriangles;
uniform bool hasTexture;

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

    float randomnessSpan = 0.2 * float(showTriangles);
    float idFactor = 1 - randomnessSpan + randomnessSpan * rand(float(gl_PrimitiveID));
    vec4 textureColor = texture(diffuseTexture, textureCoords);

    if (hasTexture)
        FragColor = vec4(idFactor * (ambiant * ambiantColor + diffuse * diffuseColor), 1.0) * textureColor;
    else
        FragColor = vec4(idFactor * (ambiant * ambiantColor + diffuse * diffuseColor), 1.0);
    // FragColor = vec4(normalVec, 1.0);
}

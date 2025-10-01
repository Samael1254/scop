#version 460 core

in VS_OUT {
    vec2 textureCoords;
    vec3 FragPos;
    mat3 TBN;
    vec3 normalVec;
} fs_in;

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
uniform sampler2D normalMap;
uniform bool hasNormalMap;

uniform int displayMode;

float rand(float x)
{
    return fract(sin(x) * 43758.5453123);
}

void main()
{
    vec3 normal;
    if (hasNormalMap)
    {
        normal = texture(normalMap, fs_in.textureCoords).rgb;
        normal = normalize(normal * 2.0 - 1.0);
        normal = normalize(fs_in.TBN * normal);
    }
    else
        normal = fs_in.normalVec;
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float incidence = max(dot(lightDir, normal), 0.);
    vec3 ambiant = ambiantLightBrightness * ambiantLightColor;
    vec3 diffuse = incidence * lightBrightness * lightColor;
    vec3 reflectionDir = normalize(2 * dot(lightDir, normal) * normal - lightDir);
    vec3 specular = pow(max(dot(reflectionDir, -normalize(cameraPos - fs_in.FragPos)), 0.0), specularExponent) * lightBrightness * lightColor * specularColor;

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
        vec4 textureColor = texture(diffuseTexture, fs_in.textureCoords);
        FragColor = vec4((ambiant * ambiantColor + diffuse * diffuseColor) * vec3(textureColor) + specular * specularColor, 1.0);
    }
    else if (displayMode == 3)
        FragColor = vec4(normal, 1.0);
}

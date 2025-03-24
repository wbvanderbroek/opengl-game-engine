#version 330 core
#define MAX_LIGHTS 8

out vec4 FragColor;

in vec3 crntPos;
in vec3 Normal;
in vec3 color;
in vec2 texCoord;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform vec3 lightPos;
uniform vec3 camPos;


struct Light {
    vec4 color;
    vec3 pos;
};

uniform int numLights;
uniform Light lights[MAX_LIGHTS];

vec4 pointLight(Light light)
{
    vec3 lightVec = light.pos - crntPos;
    float dist = length(lightVec);
    float a = 0.05f;
    float b = 0.7f;
    float inten = 1.0f / (a * dist + b * dist + 1.0f);

    float ambient = 0.20f;

    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(lightVec);
    float diffuse = dot(normal, lightDirection) * 0.5 + 0.5;

    float specularLight = 0.50f;
    vec3 viewDirection = normalize(camPos - crntPos);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
    float specular = specAmount * specularLight;

    return (texture(diffuse0, texCoord) * (diffuse * inten + ambient) +
            texture(specular0, texCoord).r * specular * inten) * light.color;
}

vec4 directLight(Light light)
{
    float ambient = 0.20f;

    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 1.0f));
    float diffuse = dot(normal, lightDirection) * 0.5 + 0.5;

    float specularLight = 0.50f;
    vec3 viewDirection = normalize(camPos - crntPos);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
    float specular = specAmount * specularLight;

    return (texture(diffuse0, texCoord) * (diffuse + ambient) +
            texture(specular0, texCoord).r * specular) * light.color;
}

vec4 spotLight(Light light)
{
    float outerCone = 0.90f;
    float innerCone = 0.95f;

    float ambient = 0.20f;

    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(light.pos - crntPos);
    float diffuse = dot(normal, lightDirection) * 0.5 + 0.5;

    float specularLight = 0.50f;
    vec3 viewDirection = normalize(camPos - crntPos);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
    float specular = specAmount * specularLight;

    float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
    float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

    return (texture(diffuse0, texCoord) * (diffuse * inten + ambient) +
            texture(specular0, texCoord).r * specular * inten) * light.color;
}

void main()
{
    vec4 finalColor = vec4(0.0);
    for (int i = 0; i < numLights; i++) {
        finalColor += pointLight(lights[i]);
    }
    FragColor = finalColor;
}
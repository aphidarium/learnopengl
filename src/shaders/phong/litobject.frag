#version 330 core

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  sampler2D emission;
  float shininess;
};

struct DirectionalLight {
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct PointLight {
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  /* attentuation */
  float constant;
  float linear;
  float quadratic;
};

struct SpotLight {
  vec3 position;
  vec3 direction;
  float innerCone;
  float outerCone;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  /* attentuation */
  float constant;
  float linear;
  float quadratic;
};

uniform vec3       viewPos;
uniform Material   material;

uniform DirectionalLight dirLight;

uniform bool usingFlashlight;
uniform SpotLight flashlight;

uniform int spotLightAmount; 
uniform SpotLight spotLights[16];

uniform int pointLightAmount;
uniform PointLight pointLights[16];

in vec3 normal;
in vec3 fragPos;
in vec2 texCoords;

out vec4 fragColor;

vec3 calcDirectionalLighting(DirectionalLight light, vec3 normal, vec3 viewDir) {
    vec3 surfaceToLight = normalize(-light.direction);

    float diff = max(dot(normal, surfaceToLight), 0.0);

    vec3 reflectDir = reflect(-surfaceToLight, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient  = light.ambient * vec3((texture(material.diffuse, texCoords)));
    vec3 diffuse  = light.diffuse * diff * vec3((texture(material.diffuse, texCoords)));
    vec3 specular = light.specular * spec * vec3((texture(material.specular, texCoords)));

    return (diffuse + specular);
}

vec3 calcPointLighting(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 surfaceToLight = normalize(light.position - fragPos);

    float diff = max(dot(normal, surfaceToLight), 0.0);

    vec3 reflectDir = reflect(-surfaceToLight, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient  = light.ambient * vec3((texture(material.diffuse, texCoords)));
    vec3 diffuse  = light.diffuse * diff * vec3((texture(material.diffuse, texCoords)));
    vec3 specular = light.specular * spec * vec3((texture(material.specular, texCoords)));

    float dist  = length(light.position - fragPos);
    float atten = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    ambient  *= atten;
    diffuse  *= atten;
    specular *= atten;

    return (ambient + diffuse + specular);
}

vec3 calcSpotLighting(SpotLight light, vec3 normal, vec3 viewDir) {
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));
    vec3 surfaceToLight = normalize(light.position - fragPos);

    float theta = dot(surfaceToLight, normalize(-light.direction));
    if (theta > light.outerCone) {
        vec3 diffuse = (max(dot(normal, surfaceToLight), 0.0)) * light.diffuse * vec3(texture(material.diffuse, texCoords));

        vec3 reflectDir = reflect(-surfaceToLight, normal);

        vec3 specularMap = vec3(texture(material.specular, texCoords));
        float grayscale = (specularMap.x + specularMap.y + specularMap.z) / 3;

        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = (vec3(texture(material.specular, texCoords)) * grayscale * spec) * light.specular;

        vec3 emission = vec3(texture(material.emission, texCoords));

        float dist  = length(light.position - fragPos);
        float atten = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

        ambient  *= atten;
        diffuse  *= atten;
        specular *= atten;

        float epsilon   = light.innerCone - light.outerCone;
        float intensity = clamp((theta - light.outerCone) / epsilon, 0.0, 1.0);

        diffuse  *= intensity;
        specular *= intensity;

        return (ambient + diffuse + specular);
    } else {
        return ambient * vec3(texture(material.diffuse, texCoords));
    }
}

void main() {
    vec3 viewDir = normalize(viewPos - fragPos);

    vec3 result = vec3(0);

    result += calcDirectionalLighting(dirLight, normal, viewDir);

//    for (int i = 0; i < spotLightAmount; i++)
//      result += calcSpotLighting(spotLights[i], normal, viewDir);
//
//    for (int i = 0; i < pointLightAmount; i++)
//      result += calcPointLighting(pointLights[i], normal, fragPos, viewDir);

    if (usingFlashlight) result += calcSpotLighting(flashlight, normal, viewDir);

    gl_FragColor = vec4(result, 1.0f);
}

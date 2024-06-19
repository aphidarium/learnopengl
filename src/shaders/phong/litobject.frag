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

uniform vec3       lightColor;
uniform vec3       lightPos;
uniform vec3       viewPos;
uniform Material   material;
uniform PointLight light;

in vec3 normal;
in vec3 fragPos;
in vec2 texCoords;

out vec4 fragColor;

void main() {
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));

    vec3 surfaceToLight = normalize(light.position - fragPos);
    vec3 diffuse = (max(dot(normal, surfaceToLight), 0.0)) * light.diffuse * vec3(texture(material.diffuse, texCoords));

    vec3 viewDir = normalize(viewPos - fragPos);
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
    fragColor = vec4(ambient + diffuse + specular, 1.0);
}

#version 330 core

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 normal;
in vec3 fragPos;

out vec4 fragColor;

void main() {
    vec3 ambient = lightColor * 0.2;

    vec3 surfaceToLight = normalize(lightPos - fragPos);
    vec3 diffuse = (max(dot(normal, surfaceToLight), 0.0)) * lightColor;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightPos, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 2);
    vec3 specular = 0.5 * spec * lightColor;

    fragColor = vec4((ambient + diffuse + specular) * objectColor, 1.0);
}

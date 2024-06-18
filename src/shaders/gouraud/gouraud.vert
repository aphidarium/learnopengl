#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

out vec4 ourColor;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    vec3 normal = mat3(transpose(inverse(model))) * normalize(aNormal);
    vec3 fragPos = vec3(model * vec4(aPos, 1.0));

    vec3 ambient = lightColor * 0.2;

    vec3 surfaceToLight = normalize(lightPos - fragPos);
    vec3 diffuse = (max(dot(normal, surfaceToLight), 0.0)) * lightColor;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightPos, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 2);
    vec3 specular = 0.5 * spec * lightColor;

    ourColor = vec4((ambient + diffuse + specular) * objectColor, 1.0);
}

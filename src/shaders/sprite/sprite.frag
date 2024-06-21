#version 330 core

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  sampler2D emission;
  float shininess;
};

uniform Material material;
uniform vec3     color;

in vec2 texCoords;

out vec4 fragColor;

void main() {
  vec4 result = texture(material.diffuse, texCoords);

  if (result.w == 0.0) discard;
  if (result.x + result.y + result.z == 0.0) result = vec4(color, 1.0);

  fragColor = result;
}

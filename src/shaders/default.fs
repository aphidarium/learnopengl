#version 330 core

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float mixAmount;

in vec2 texCoords;

out vec4 fragColor;

void main() {
    fragColor = mix(texture(texture1, texCoords), texture(texture2, vec2(-texCoords.x, texCoords.y)), mixAmount);
}
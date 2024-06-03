#version 330 core

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float mixAmount;

in vec3 ourColor;
in vec2 texCoords;

out vec4 fragColor;

void main() {
    // fragColor = texture(ourTexture, texCoords) * vec4(ourColor, 1.0);

    vec4 tex1 = texture(texture1, texCoords);
    vec4 tex2 = texture(texture2, vec2(-texCoords.x, texCoords.y));

    fragColor = mix(tex1, tex2, mixAmount);
    // fragColor = texture(texture1, texCoords);
}
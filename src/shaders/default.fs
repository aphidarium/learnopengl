#version 330 core

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float mixAmount;

in vec3 ourColor;
in vec2 texCoords;

out vec4 fragColor;

void main() {
    // fragColor = texture(ourTexture, texCoords) * vec4(ourColor, 1.0);

    //vec4 tex1 = texture(texture1, texCoords);
    //vec4 tex2 = texture(texture2, vec2(1.0-texCoords.x, texCoords.y));

    vec4 col = mix(texture(texture1, texCoords), texture(texture2, vec2(-texCoords.x, texCoords.y)), mixAmount);

    // inverted colours
    //fragColor = vec4(1.0-col.x, 1.0-col.y, 1.0-col.z, col.w);

    // grayscale
    // float avg = (col.x + col.y + col.z) / 3;
    // fragColor = vec4(avg, avg, avg, col.w);
    fragColor = col;

    // fragColor = texture(texture1, texCoords);
}
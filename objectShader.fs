#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D uTexture;
uniform vec3 uObjectColor;
uniform vec3 uLightColor;

void main() {
    vec4 texColor = texture(uTexture, TexCoord);
    vec3 result = uLightColor * uObjectColor;
    FragColor = vec4(result, 1.0);
}
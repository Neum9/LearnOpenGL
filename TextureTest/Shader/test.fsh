#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sample2D ourTexture;

void main() {
	FragColor = texture(ourTexture,TexCoord);
}
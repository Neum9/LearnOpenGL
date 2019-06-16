#version 330 core

out vec4 FragColor;

vec3 ourColor;
in vec2 TexCoord;

uniform float m_coe;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
	FragColor = mix( texture(texture1,TexCoord), texture(texture2,TexCoord) ,m_coe);
	//FragColor = mix( texture(texture2,TexCoord), texture(texture1,vec2(1.0 - TexCoord.x,TexCoord.y)) ,0.5);
}
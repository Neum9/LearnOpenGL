#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 lightPos;

uniform vec3 viewPos;

uniform float specularStrength;

void main() {

	//环境光
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;
	
	//漫反射
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm,lightDir),0.0);
	vec3 diffuse = diff * lightColor;

	//镜面反射
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir,norm);

	float spec = pow( max(dot(viewDir,reflectDir),0.0),32 );
	vec3 specular = specularStrength * spec * lightColor;

	//vec3 result = objectColor;
	//vec3 result = ambient * objectColor;
	//vec3 result = (ambient + diffuse) * objectColor;
	vec3 result = (ambient + diffuse + specular) * objectColor;

	FragColor = vec4(result,1.0);
	//FragColor = vec4(lightColor * objectColor,1.0);
}
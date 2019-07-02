#version 330 core
struct Material {
	sampler2D diffuse;	
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material material;
uniform Light light;

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 lightPos;

uniform vec3 viewPos;

uniform float specularStrength;

int vec2 TexCoords;

void main() {

	
	//漫反射
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm,lightDir),0.0);
	vec3 diffuse = vec3(texture(material.diffuse,TexCoords)) * light.diffuse;

	//环境光
	vec3 ambient =vec3(texture(material.diffuse,TexCoords)) * light.ambient;

	//镜面反射
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir,norm);
	float spec = pow( max(dot(viewDir,reflectDir),0.0),32 );
	vec3 specular =  (spec * material.specular) * lightColor;

	vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result,1.0);
}
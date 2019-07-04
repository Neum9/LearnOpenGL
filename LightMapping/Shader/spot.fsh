/*
	聚光灯
*/

#version 330 core

out vec4 FragColor;

struct Material {
	sampler2D diffuse;	
	sampler2D specular;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main() {

	//计算光强度
	vec3 lightDir = normalize(light.position - FragPos);

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp(( theta - light.outerCutOff) / epsilon,0.0,1.0  );

	//执行光照计算
 	//环境光
	vec3 ambient =vec3(texture(material.diffuse,TexCoords)) * light.ambient;
	//漫反射
	vec3 norm = normalize(Normal);
	float diff = max(dot(norm,lightDir),0.0);
	vec3 diffuse = vec3(texture(material.diffuse,TexCoords)) * light.diffuse * diff;

	//镜面反射
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir,norm);
	float spec = pow( max(dot(viewDir,reflectDir),0.0),material.shininess );
	vec3 specular =  spec * light.specular *  vec3(texture( material.specular,TexCoords)) ;

    // 计算衰减
	//计算衰减
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic *(distance * distance));

	vec3 result = (ambient + ( diffuse + specular) * intensity )* attenuation;
	FragColor = vec4(result,1.0);	
}
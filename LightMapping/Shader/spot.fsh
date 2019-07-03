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
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	// float constant;
	// float linear;
	// float quadratic;
};


in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main() {


	vec3 lightDir = normalize(light.position - FragPos);

	float theta = dot(lightDir, normalize(-light.direction));

	if(theta > light.cutOff) 
	{   
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

		vec3 result = (ambient + diffuse + specular) ;
		FragColor = vec4(result,1.0);
	}
	else  // 否则，使用环境光，让场景在聚光之外时不至于完全黑暗
		FragColor = vec4(light.ambient * vec3(texture(material.diffuse, TexCoords)), 1.0);
}
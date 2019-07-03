
/*
	平行光
*/

#version 330 core

out vec4 FragColor;

struct Material {
	sampler2D diffuse;	
	sampler2D specular;
	float shininess;
};

struct Light {
	//vec3 position;
	vec3 direction; //使用定向光，方向取代
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};


in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main() {

	//环境光
	vec3 ambient =vec3(texture(material.diffuse,TexCoords)) * light.ambient;
	
	//漫反射
	vec3 norm = normalize(Normal);
	//vec3 lightDir = normalize(light.position - FragPos);
	//方向只要取光照方向相反即可
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(norm,lightDir),0.0);
	vec3 diffuse = vec3(texture(material.diffuse,TexCoords)) * light.diffuse * diff;


	//镜面反射
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir,norm);
	float spec = pow( max(dot(viewDir,reflectDir),0.0),material.shininess );
	vec3 specular =  spec * light.specular *  vec3(texture( material.specular,TexCoords)) ;

	vec3 result = (ambient + diffuse + specular) ;
	FragColor = vec4(result,1.0);

	//FragColor = vec4(1.0F,0.0F,0.0F,1.0F);
}
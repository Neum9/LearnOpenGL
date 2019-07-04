#version 330 core

#define COUNT_POINT_LIGHTS 4

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

//定向光结构
struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

//点光源结构
struct PointLight {
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

//聚光灯结构
struct SpotLight {
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

//物体材质结构
struct Material {
	sampler2D diffuse;	
	sampler2D specular;
	float shininess;
};

//函数声明
//计算定向光
vec3 CalcDirLight(DirLight light,vec3 normal,vec3 viewDir);
//计算点光源
vec3 CalcPointLight(PointLight light,vec3 normal,vec3 fragPos,vec3 viewDir);
//计算聚光灯
vec3 CalcSPotLight(SpotLight light,vec3 normal,vec3 fragPos,vec3 viewDir);


uniform DirLight dirLight;
uniform PointLight pointLightList[COUNT_POINT_LIGHTS];
uniform SpotLight spotLight;

uniform Material material;

uniform vec3 viewPos;

void main() {
	//定义一个输出颜色值
	vec3 outputColor;
	//视线方向,是指从物体到摄像机，因为计算镜面反射时需要计算这个向量和反射向量的点乘
	vec3 viewDir = normalize(viewPos - FragPos);
	//标准化法向量
	vec3 norm = normalize(Normal);
	//定向光贡献
	outputColor += CalcDirLight(dirLight,norm, viewDir);
	//点光源贡献
	for (int i = 0;i < COUNT_POINT_LIGHTS;i++) {
		outputColor += CalcPointLight(pointLightList[i],norm,FragPos,viewDir);
	}
	//聚光贡献
	outputColor += CalcSPotLight(spotLight,norm,FragPos,viewDir);

	FragColor = vec4(outputColor,1.0);
}

//函数实现
vec3 CalcDirLight(DirLight light,vec3 normal,vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction);
	//diffuse
	float diff = max( dot(normal,lightDir),0.0 );
	//specular
	vec3 reflectDir = reflect(-lightDir,normal);
	float spec = pow( max(dot(viewDir,reflectDir),0.0),material.shininess );
	//add
	vec3 ambient = light.ambient * vec3(texture(material.diffuse,TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse,TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular,TexCoords));
	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light,vec3 normal,vec3 fragPos,vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);
	//漫反射着色
	float diff = max(dot(normal,lightDir),0.0);
	//镜面管着色
	vec3 reflectDir = reflect(-lightDir,normal);
	float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
	//衰减
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
	//合并结果
	vec3 ambient = light.ambient * vec3(texture(material.diffuse,TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse,TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular,TexCoords));
	return (ambient + diffuse + specular) * attenuation;
}

vec3 CalcSPotLight(SpotLight light,vec3 normal,vec3 fragPos,vec3 viewDir) {
	//计算光强度
	vec3 lightDir = normalize(light.position - fragPos);

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp(( theta - light.outerCutOff) / epsilon,0.0,1.0  );

	//执行光照计算
	//环境光
	vec3 ambient =vec3(texture(material.diffuse,TexCoords)) * light.ambient;
	//漫反射
	vec3 norm = normalize(normal);
	float diff = max(dot(norm,lightDir),0.0);
	vec3 diffuse = vec3(texture(material.diffuse,TexCoords)) * light.diffuse * diff;

	//镜面反射
	vec3 reflectDir = reflect(-lightDir,norm);
	float spec = pow( max(dot(viewDir,reflectDir),0.0),material.shininess );
	vec3 specular =  spec * light.specular *  vec3(texture( material.specular,TexCoords));

	//计算衰减
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic *(distance * distance));

	vec3 result = (ambient + ( diffuse + specular) * intensity )* attenuation;
	return result;
}
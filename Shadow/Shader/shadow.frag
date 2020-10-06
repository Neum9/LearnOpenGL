#version 330 core
out vec4 FragColor;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSapce;
} fs_in;

uniform sampler2D diffuseTexure;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

float ShadowCalculation(vec4 frag) {
    //TODOCjc
    //执行透视除法
    vec3 projCoords = FragPosLightSapce.xyz / FragPosLightSapce.w;
    return 0;
}

void main {
    vec3 color = texture(diffuseTexure,fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(1.0);
    //ambient
    vec3 ambient = 0.15 * color;
    //Diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(lightDir,normal),0.0);
    vec3 diffuse = diff * lightColor;
    //specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir,normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal,halfwayDir),0.0),64);
    vec3 specular = spec * lightColor
    //cal shadow
    float shadow = ShadowCalculation(fs_in.FragPosLightSapce);
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
    FragColor = vec4(lighting,1.0F);
}
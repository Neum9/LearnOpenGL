#version 330 core
out vec4 FragColor;

in VS_OUT{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
}fs_in;

uniform sampler2D diffuseTexure;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

float ShadowCalculation(vec4 fragPosLightSpace,float bias){
    //执行透视除法
    vec3 projCoords=fragPosLightSpace.xyz/fragPosLightSpace.w;
    //变换到[0,1]的范围
    projCoords=projCoords*.5+.5;
    if(projCoords.z>1.){
        return 0.;
    }
    // //取得最近点的深度
    // float closestDepth=texture(shadowMap,projCoords.xy).r;
    //取得当前片段在光源视角下的深度
    float currentDepth=projCoords.z;
    // //检查当前片段是否在阴影中
    // float shadow=currentDepth-bias>closestDepth?1.:0.;
    //return shadow;
    
    float shadow=0.;
    vec2 texelSize=1./textureSize(shadowMap,0);
    for(int x=-1;x<=1;++x)
    {
        for(int y=-1;y<=1;++y)
        {
            float pcfDepth=texture(shadowMap,projCoords.xy+vec2(x,y)*texelSize).r;
            shadow+=currentDepth-bias>pcfDepth?1.:0.;
        }
    }
    shadow/=9.;
    return shadow;
}

void main(){
    vec3 color=texture(diffuseTexure,fs_in.TexCoords).rgb;
    vec3 normal=normalize(fs_in.Normal);
    vec3 lightColor=vec3(1.);
    //ambient
    vec3 ambient=.15*color;
    //Diffuse
    vec3 lightDir=normalize(lightPos-fs_in.FragPos);
    float diff=max(dot(lightDir,normal),0.);
    vec3 diffuse=diff*lightColor;
    //specular
    vec3 viewDir=normalize(viewPos-fs_in.FragPos);
    vec3 reflectDir=reflect(-lightDir,normal);
    float spec=0.;
    vec3 halfwayDir=normalize(lightDir+viewDir);
    spec=pow(max(dot(normal,halfwayDir),0.),64);
    vec3 specular=spec*lightColor;
    //cal shadow
    //偏移
    float bias=max(.05*(1.-dot(normal,lightDir)),.005);
    float shadow=ShadowCalculation(fs_in.FragPosLightSpace,bias);
    vec3 lighting=(ambient+(1.-shadow)*(diffuse+specular))*color;
    FragColor=vec4(lighting,1.F);
}
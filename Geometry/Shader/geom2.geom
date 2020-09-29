#version 330 core
layout(triangles)in;
layout(triangle_strip,max_vertices=3)out;

in VS_OUT{
    vec2 texCoords;
}gs_in[];

out vec2 texCoords;
uniform float time;

vec3 GetNormal(){
    vec3 a=vec3(gl_in[0].gl_Position)-vec3(gl_in[1].gl_Position);
    vec3 b=vec3(gl_in[2].gl_Position)-vec3(gl_in[1].gl_Position);
    return normalize(cross(a,b));
}

vec4 explode(vec4 position,vec3 normal){
    float mag=2.;
    vec3 dir=normal*((sin(time)+1.)/2.)*mag;
    return position+vec4(dir,0.);
}

void main(){
    vec3 normal=GetNormal();
    
    gl_Position=explode(gl_in[0].gl_Position,normal);
    texCoords=gs_in[0].texCoords;
    EmitVertex();
    gl_Position=explode(gl_in[1].gl_Position,normal);
    EmitVertex();
    gl_Position=explode(gl_in[2].gl_Position,normal);
    EmitVertex();

    EndPrimitive();
}
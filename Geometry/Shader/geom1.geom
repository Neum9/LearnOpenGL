#version 330 core
layout(points)in;
layout(triangle_strip,max_vertices=5)out;

in VS_OUT{
    vec3 color;
}gs_in[];

out vec3 fColor;

void build_house(vec4 position){
    fColor=gs_in[0].color;
    gl_Position=position+vec4(-.2,-.2,0.,0.);
    EmitVertex();
    gl_Position=position+vec4(.2,-.2,0.,0.);
    EmitVertex();
    gl_Position=position+vec4(-.2,.2,0.,0.);
    EmitVertex();
    gl_Position=position+vec4(.2,.2,0.,0.);
    EmitVertex();
    gl_Position=position+vec4(0.,.4,0.,0.);
    fColor=vec3(1,1,1);
    EmitVertex();
    EndPrimitive();
}

// vec3 GetNormal(){
    //     vec3 a=vec3(gl_in[0].gl_Position)-vec3(gl_in[1].gl_Position);
    //     vec3 b=vec3(gl_in[2].gl_Position)-vec3(gl_in[1].gl_Position);
    //     return normalize(cross(a,b));
// }

// vec3 explore(vec4 position,vec3 normal){
//     float mag=2.;
//     vec3 dir=normal*((sin(time)+1.)/2.)*mag;
//     return position+vec4(dir,0.);
// }

void main(){
    build_house(gl_in[0].gl_Position);
}
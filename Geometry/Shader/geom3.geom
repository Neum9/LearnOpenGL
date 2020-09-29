#version 330 core
layout(triangles)in;
layout(line_strip,max_vertices=6)out;

in VS_OUT{
    vec3 normal;
}gs_in[];

const float MAG=.4;

void GenLine(int index){
    gl_Position=gl_in[index].gl_Position;
    EmitVertex();
    gl_Position=gl_in[index].gl_Position+vec4(gs_in[index].normal,0.)*MAG;
    EmitVertex();
    EndPrimitive();
}

void main(){
    GenLine(0);
    GenLine(1);
    GenLine(2);
}
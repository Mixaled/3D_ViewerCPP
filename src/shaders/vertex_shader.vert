#version 330
uniform mat4 MV;
uniform mat4 P;
uniform vec4 C;

in vec3 position;
in vec3 normal;

out vec4 fcolor;

void main(){
    gl_Position = P * MV * vec4(position, 1.0);
    fcolor = C;
}
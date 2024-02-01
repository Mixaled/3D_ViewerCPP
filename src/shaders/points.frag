#version 330

in vec4 fcolor;

out vec4 FragColor;

uniform int u_is_circle;

void main() {
    if (u_is_circle > 0 && length(gl_PointCoord - 0.5) > 0.5)
        discard;
    FragColor = vec4(fcolor);
}
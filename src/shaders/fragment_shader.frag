#version 330

uniform int is_stripple;

in vec4 fcolor;
out vec4 FragColor;

uniform float time;


void main() {
    if (is_stripple == 1) {
        float stipplePattern = mod(gl_FragCoord.x * 0.1 + gl_FragCoord.y * 0.1 + time * 10.0, 1.0);
        if (stipplePattern > 0.5) discard;
    }
    FragColor = vec4(fcolor);
}
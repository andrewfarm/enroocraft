#version 330 core

in vec3 v_Pos;

out vec4 color;

void main() {
    color = vec4(v_Pos, 1.0);
}

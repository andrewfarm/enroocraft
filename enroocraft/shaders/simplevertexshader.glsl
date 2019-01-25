#version 330 core

layout(location = 0) in vec3 a_Pos;

uniform mat4 u_MvpMatrix;

void main() {
    gl_Position = u_MvpMatrix * vec4(a_Pos, 1.0f);
}

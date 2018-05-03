#version 330 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec3 a_Normal;

out vec3 v_Normal;

uniform mat4 u_MvpMatrix;

void main() {
    v_Normal = a_Normal;
    gl_Position = u_MvpMatrix * vec4(a_Pos, 1.0);
}

#version 330 core

layout(location = 0) in vec3 pos;

uniform mat4 u_MvpMatrix;

void main() {
    gl_Position = u_MvpMatrix * vec4(pos, 1.0);
}

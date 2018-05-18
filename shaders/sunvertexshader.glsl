#version 330 core

layout(location=0) in vec3 a_Pos;

uniform mat4 u_VpRotationMatrix;

void main() {
    gl_Position = (u_VpRotationMatrix * vec4(a_Pos, 1.0)).xyww;
}


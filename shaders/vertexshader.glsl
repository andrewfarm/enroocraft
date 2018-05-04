#version 330 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexUV;

out vec3 v_Normal;
out vec2 v_TexUV;

uniform mat4 u_MvpMatrix;

void main() {
    v_Normal = a_Normal;
    v_TexUV = a_TexUV;
    gl_Position = u_MvpMatrix * vec4(a_Pos, 1.0);
}

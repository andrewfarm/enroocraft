#version 330 core

layout(location=0) in vec3 a_Pos;

out vec3 v_Pos;

uniform mat4 u_VpRotationMatrix;

void main() {
    v_Pos = a_Pos;
    v_Pos.z = -v_Pos.z;
    
    gl_Position = (u_VpRotationMatrix * vec4(a_Pos, 1.0)).xyww;
}

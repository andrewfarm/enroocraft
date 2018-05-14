#version 330 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexUV;
layout(location = 3) in float a_AmbientLight;

out vec3 v_Normal;
out vec2 v_TexUV;
out float v_AmbientLight;
out vec3 v_PosLightSpace;

uniform mat4 u_MvpMatrix;
uniform mat4 u_LightBiasMvpMatrix;

void main() {
    v_PosLightSpace = (u_LightBiasMvpMatrix * vec4(a_Pos, 1.0)).xyz;
    
    v_Normal = a_Normal;
    v_TexUV = a_TexUV;
    v_AmbientLight = a_AmbientLight;
    gl_Position = u_MvpMatrix * vec4(a_Pos, 1.0);
}

#version 330 core

in vec2 v_UV;

uniform sampler2D u_ColorTexture;
uniform sampler2D u_DepthTexture;

out vec3 color;

void main() {
    color = vec3(v_UV, 0.0f);//texture(u_DepthTexture, v_UV).rgb;
}

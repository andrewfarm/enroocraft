#version 330 core

in vec2 v_UV;

uniform sampler2D u_ColorTexture;
uniform sampler2D u_DepthTexture;

out vec3 color;

void main() {
    color = vec3(pow(texture(u_DepthTexture, v_UV).r, 50.0));
}

#version 330 core

in vec2 v_UV;

uniform sampler2D u_ColorTexture;
uniform sampler2D u_DepthTexture;
uniform vec4 u_ColorOverlay;

out vec3 color;

void main() {
    float depth = texture(u_DepthTexture, v_UV).x;
//    float fog = depth == 1.0 ? 0.0 : pow(depth, 250.0);
//    vec3 fogColor = vec3(0.8, 0.9, 1.0);
    vec3 textureColor = texture(u_ColorTexture, v_UV).rgb;
    color = (textureColor * (1.0 - u_ColorOverlay.a)) + (u_ColorOverlay.rgb * u_ColorOverlay.a);
}

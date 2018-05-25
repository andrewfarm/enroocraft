#version 330 core

uniform sampler2D u_Texture;
uniform float u_ScreenWidth;
uniform float u_ScreenHeight;

out vec4 color;

void main() {
    vec2 texUV = vec2(gl_FragCoord.x / u_ScreenWidth, gl_FragCoord.y / u_ScreenHeight);
    color = texture(u_Texture, texUV);
}

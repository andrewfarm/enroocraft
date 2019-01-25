#version 330 core

uniform sampler2D u_Texture;
uniform vec2 u_StartUV;
uniform float u_TextureSize; // a portion of the texture atlas' dimensions

out vec3 color;

void main() {
    vec2 uv = u_StartUV + (gl_PointCoord * u_TextureSize);
    color = texture(u_Texture, uv).rgb;
}

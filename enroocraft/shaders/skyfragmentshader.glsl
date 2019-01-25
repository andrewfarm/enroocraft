#version 330 core

in vec3 v_Pos;

out vec4 color;

uniform samplerCube u_Texture;

void main() {
    color = texture(u_Texture, v_Pos);
}

#version 330 core

in vec3 v_Normal;
in vec2 v_TexUV;

out vec3 color;

uniform sampler2D u_Texture;

void main() {
    vec4 texColor = texture(u_Texture, v_TexUV);
    float ambientLight = 0.5;
    float directionalLight = clamp(dot(v_Normal, normalize(vec3(0.3f, 1.0f, 0.4f))), 0.0f, 1.0f);
    color = texColor.rgb * clamp(ambientLight + directionalLight, 0.0f, 1.0f);
}

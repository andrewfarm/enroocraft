#version 330 core

in vec3 v_Normal;
in vec2 v_TexUV;

uniform sampler2D u_Texture;

layout(location = 0) out vec3 color;

void main() {
    vec4 texColor = texture(u_Texture, v_TexUV);
    float ambientLight = 0.6;
    float directionalLight = clamp(dot(v_Normal, normalize(vec3(0.1f, 1.0f, 0.2f))), 0.0f, 1.0f);
    color = texColor.rgb * clamp(ambientLight + directionalLight, 0.0f, 1.0f);
}

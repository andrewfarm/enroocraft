#version 330 core

in vec3 v_Normal;
in vec2 v_TexUV;

uniform sampler2D u_Texture;

layout(location = 0) out vec4 color;

const vec3 fogColor = vec3(0.8, 0.9, 1.0);

void main() {
    vec4 texColor = texture(u_Texture, v_TexUV);
    float ambientLight = 0.6;
    float directionalLight = clamp(dot(v_Normal, normalize(vec3(0.1f, 1.0f, 0.2f))), 0.0f, 1.0f);
    float fog = pow(gl_FragCoord.z, 500.0);
    vec3 colorWithoutFog = texColor.rgb * clamp(ambientLight + directionalLight, 0.0f, 1.0f);
    vec3 colorWithFog = (colorWithoutFog * (1.0f - fog)) + (fogColor * fog);
    color = vec4(colorWithFog, texColor.a);
}

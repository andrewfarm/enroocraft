#version 330 core

in vec3 v_Normal;
in vec2 v_TexUV;
in float v_AmbientLight;
in vec3 v_PosLightSpace;

uniform sampler2D u_Texture;
uniform sampler2D u_ShadowMap;
uniform vec3 u_LightDirection;
uniform float u_AmbientLight;

layout(location = 0) out vec4 color;

const vec3 fogColor = vec3(0.8, 0.9, 1.0);

void main() {
    vec4 texColor = texture(u_Texture, v_TexUV);
    
    float ambientLight = (0.5 * v_AmbientLight + 0.2) * u_AmbientLight;
    float cosLightAngle = max(dot(normalize(v_Normal), normalize(u_LightDirection)), 0.0f);
    float shadowBias = 0.0001 * tan(acos(cosLightAngle));
    bool inShadow = (texture(u_ShadowMap, v_PosLightSpace.xy).r < v_PosLightSpace.z - shadowBias);
    float directionalLight = inShadow ? 0.0 : (0.4 * cosLightAngle);
    float totalLight = clamp(ambientLight + directionalLight, 0.0f, 1.0f);
    
    vec3 colorWithoutFog = texColor.rgb * totalLight;
    
    float fog = pow(gl_FragCoord.z, 5000.0);
    vec3 colorWithFog = (colorWithoutFog * (1.0f - fog)) + (fogColor * u_AmbientLight * fog);
    color = vec4(colorWithFog, texColor.a);
}

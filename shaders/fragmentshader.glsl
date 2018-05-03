#version 330 core

in vec3 v_Normal;

out vec3 color;

void main() {
    float ambientLight = 0.5;
    float directionalLight = clamp(dot(v_Normal, normalize(vec3(0.3f, 1.0f, 0.4f))), 0.0f, 1.0f);
    color = vec3(0.0f, 0.8f, 0.0f) * clamp(ambientLight + directionalLight, 0.0f, 1.0f);
}

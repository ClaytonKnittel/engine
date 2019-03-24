#version 330 core

uniform vec3 lightDir;

in vec3 norm;
in vec2 texCord;

out vec4 color;

uniform sampler2D tex;

void main() {
    vec4 col = texture(tex, texCord);
    float dim = max(dot(norm, lightDir), .2);
    color = vec4(dim * col.xyz, col.w);
}
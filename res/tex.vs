#version 330 core

uniform mat4 projection;
uniform mat4 cam;
uniform mat4 model;

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

out vec2 texCord;

void main() {
	gl_Position = projection * cam * model * vec4(position, 1.0);
    texCord = texCoord;
}

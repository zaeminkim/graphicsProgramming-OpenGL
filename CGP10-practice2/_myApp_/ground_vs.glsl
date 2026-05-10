#version 430 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoord;
out vec2 vsTexCoord;

uniform mat4 viewMat;
uniform mat4 projMat;

void main(void) {
	gl_Position = projMat * viewMat * vec4(pos.x, pos.y, pos.z, 1.0);

	vsTexCoord = texCoord;
}
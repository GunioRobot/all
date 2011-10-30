#version 330

in vec3 vVertex;

uniform mat4 MVP;

void main() {
	vec4 vPosition= MVP * vec4(vVertex, 1);
	gl_Position = vPosition;
}

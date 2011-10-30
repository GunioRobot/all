#version 330
smooth in vec4 vSmoothColor;

out vec4 vFragColor;

void main() {
	vFragColor = vSmoothColor;
}

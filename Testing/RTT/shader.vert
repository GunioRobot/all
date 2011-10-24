#version 330

in vec3 vColor;
in vec3 vVertex;
smooth out vec4 vSmoothColor;

vec4 vPosition;

uniform mat4 MVP;

void main() {
	float al = vColor[2] / vColor[2];
	if (al != 1) al = 1;

	vPosition = MVP * vec4(vVertex, 1);

	float z = abs(10 / vPosition.z);
	vSmoothColor = vec4(z, z, z, al);

	gl_Position = vPosition;
}

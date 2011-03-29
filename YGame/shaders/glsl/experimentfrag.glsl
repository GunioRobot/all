varying vec4 rawpos;

// This file is just a place for me to do quick experimentation.
// It is a place where i can try things out, and not worry about
// canging the other, actually useful shaders.

float noise(vec3);
float snoise(vec3);
vec4 spline(float x, int y, vec4 z[]);
float spline(float x, int y, float z[]);
float unsign(float);

void main() {
	float scale=1.10;
	vec4 tp = gl_TexCoord[0] * scale;
	vec3 rp = rawpos.xyz * scale;
	

	gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}


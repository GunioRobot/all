// Based on the wood shader in the Orange Book

varying vec3 normal;
varying vec4 pos;
varying vec4 rawpos;

uniform float scale;

float noise(vec4);
float snoise(vec4);
float noise(vec3);
float snoise(vec3);
float unsign(float x);


void main() {

	vec3 LightWood = vec3(0.6, 0.3, 0.1);
	vec3 DarkWood = vec3(0.4, 0.2, 0.07);
	//vec3 LightWood = vec3(0.46, 0.35, 0.19);
	//vec3 DarkWood = vec3(0.29, 0.27, 0.06);
	float RingFreq = 4.0;
	//float RingFreq = 0.30;
	float LightGrains = 1.0;
	float DarkGrains = 0.0;
	float GrainThreshold = 0.8;
	vec3 NoiseScale= vec3(0.5, 0.1, 0.1);
	float Noisiness = 3.0;
	float GrainScale = 17.0;

	vec4 color;
	vec4 matspec = gl_FrontMaterial.specular;
	float shininess = gl_FrontMaterial.shininess;
	vec4 lightspec = gl_LightSource[0].specular;
	vec4 lpos = gl_LightSource[0].position;
	vec4 s = -normalize(pos-lpos); 	//not sure why this needs to 
									// be negated, but it does.
	vec3 light = s.xyz;
	vec3 n = normalize(normal);
	vec3 r = -reflect(light, n);
	r = normalize(r);
	vec3 v = -pos.xyz; // We are in eye coordinates,
					   // so the viewing vector is
					   // [0.0 0.0 0.0] - pos
	v = normalize(v);

	float scalelocal;
	if (scale == 0.0) {
		scalelocal = 2.0; //default value
	} else {
		scalelocal = scale;
	}

	vec4 tp = gl_TexCoord[0] * scalelocal;
	//vec3 rp = rawpos.xyz * scalelocal* 0.8;
	vec3 rp = rawpos.xyz * scalelocal * NoiseScale;
	
	vec3 noisevec = vec3(noise(rp), noise(rp + 3.33), noise(rp + 7.77)) * Noisiness;
	vec3 location = rp + noisevec;
	
	float dist = sqrt(location.x * location.x + location.z * location.z);
	dist *= RingFreq;
	
	float rf = fract(dist + unsign(noisevec[0])/256.0 + unsign(noisevec[1])/32.0 + unsign(noisevec[2])/16.0) * 2.0;
	//float rf = fract(dist)*2.0;
	if (rf > 1.0) {
		rf = 2.0 - rf;
	}
	
	color = vec4(mix(LightWood, DarkWood, rf), 1.0);
	
	rf = fract((rp.x + rp.z) * GrainScale +0.5);
	noisevec[2] *= rf;
	
	if( rf < GrainThreshold) {
		color.xyz += LightWood * LightGrains * unsign(noisevec[2]);
	} else {
		color.xyz -= LightWood * DarkGrains * unsign(noisevec[2]);
	}
	
	//float intensity = min(1.0, noise(rp) * 5.0);
	//color = vec4(intensity, intensity, intensity, 1.0);
	
	vec4 diffuse  = color * max(0.0, dot(n, s.xyz)) * gl_LightSource[0].diffuse;
	vec4 specular;
	if (shininess != 0.0) {
		specular = lightspec * matspec * pow(max(0.0, dot(r, v)), shininess);
	} else {
		specular = vec4(0.0, 0.0, 0.0, 0.0);
	}
	
	gl_FragColor = diffuse + specular;


}


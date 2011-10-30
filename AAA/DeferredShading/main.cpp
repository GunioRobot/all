#include <cstdio>
#include "blas.hpp"

#include <algorithm>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include "GLSLShader.h"
#include <cassert>

#include "accumulator.hpp"
#include "timer.hpp"

#include "cube.h"
#include "initgl.h"

double Timer::getTime() { return (double)glutGet(GLUT_ELAPSED_TIME) / 1000.0; }

Timer framet;
Accumulator<double> fps(1.0), input(1.0 / 30.0), render(1.0 / 60.0), spinner(1.0 / 120.0);
float rotation_x = 0, rotation_y = 0, rotation_z = 0;
uint dframes = 0;

class Viewport {
	uint m_width, m_height;
public:
	Viewport(uint width, uint height) : m_width(width), m_height(height) {}

	void bind() { glViewport(0, 0, m_width, m_height); }
	void resize(uint width, uint height) { m_width = width; m_height = height; }

	uint width() const { return m_width; }
	uint height() const { return m_height; }
	float aspect() const { return (float)m_width / (float)m_height; }
};

class Renderer {
	unsigned int fbo_id;
	unsigned int depth_buffer;
	int m_FBOWidth, m_FBOHeight;
	unsigned int m_FragLayers[4];

	Viewport m_FBOViewport, m_ScreenViewport;
public:
	Renderer(int sw, int sh, int fw, int fh) : m_FBOWidth(fw), m_FBOHeight(fh), m_FBOViewport(fw, fh), m_ScreenViewport(sw, sh) {}

	void resize(int sw, int sh) {
		m_ScreenViewport.resize(sw, sh);
	}

	void initFBO() {
		glGenRenderbuffers(1, &depth_buffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_FBOWidth, m_FBOHeight);

		glGenTextures(4, m_FragLayers);
		for (uint i = 0; i < 4; ++i) {
			glBindTexture(GL_TEXTURE_2D, m_FragLayers[i]);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_FBOWidth, m_FBOHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		glGenFramebuffers(1, &fbo_id);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo_id);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FragLayers[0], 0);
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_FragLayers[1], 0);
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_FragLayers[2], 0);
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_FragLayers[3], 0);

		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void render() {
		Matrix4 M(1), V(1), P(1), MVP(1);

		////////////////////////////////////////////////////////////////

		glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);
		m_FBOViewport.bind();
		P = perspective(45.0f, m_FBOViewport.aspect(), 1.f, 100.f);

		glClearColor(0, 0.1f, 0,0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		colorshader.Use();

		/////////////////////////////

		M = Matrix4(1);
		scale(M, Vector3(10, 10, 10));
		M[12] = 0;
		M[13] = 0;
		M[14] = -10;
		MVP = M * V * P;

		const uint csmvp = colorshader("MVP");
		glUniformMatrix4fv(csmvp, 1, GL_FALSE, &MVP[0]);

		glBindVertexArray(vaoID);
		glDrawElements(GL_TRIANGLES, n_indices, GL_UNSIGNED_INT, 0);

		////////////////////////////////////////////////////////////////

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		m_ScreenViewport.bind();
		P = perspective(45.0f, m_ScreenViewport.aspect(), 1.f, 100.f);
		//P = orthographic(-1, 1 * m_ScreenViewport.aspect(), -1 * m_ScreenViewport.aspect(), 1, -1, 1);

		glClearColor(0.1f, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		texshader.Use();

		/////////////////////////////

		glBindTexture(GL_TEXTURE_2D, m_FragLayers[0]);
		glUniform1i(texshader("textureMap"), 0);

		M = Matrix4(1);
		MVP = M * V * P;

		const uint tsmvp = texshader("MVP");
		glUniformMatrix4fv(tsmvp, 1, GL_FALSE, &MVP[0]);

		glBindVertexArray(vaoID);
		glDrawElements(GL_TRIANGLES, n_indices, GL_UNSIGNED_INT, 0);

		////////////////////////////////////////////////////////////////

		GL_CHECK_ERRORS

		glutSwapBuffers();
	}
};

Renderer renderer(800, 600, 320, 240);

void OnRender() {
	renderer.render();
}


void OnIdle() {
	GL_CHECK_ERRORS

	framet.update();
	const double dt = framet.delta();
	fps.add(dt); input.add(dt); render.add(dt); spinner.add(dt);

	++dframes;
	if (fps.ready()) {
		printf("%i %f\n", dframes, dt);
		dframes = 0;
		fps.reset();
	}
	if (render.ready()) {
		glutPostRedisplay(); // call onRender()
		render.reset();
	}
	if (input.ready()) {
		input.reset();
	}
	if (spinner.ready()) {
		//rotation_x = rotation_x + 0.000f;
		rotation_y = rotation_y + 0.002f;
		//rotation_z = rotation_z + 0.000f;
		if (rotation_x > 359) rotation_x = 0;
		if (rotation_y > 359) rotation_y = 0;
		if (rotation_z > 359) rotation_z = 0;

		spinner.reset();
	}

	GL_CHECK_ERRORS
}

void InitGL() {
	glGetError();
	GL_CHECK_ERRORS

	InitShaders();
	InitVAOs();

	renderer.initFBO();
	GL_CHECK_ERRORS

	glEnable(GL_DEPTH_TEST);
	GL_CHECK_ERRORS
}

void OnResize(int w, int h) {
	printf("Resized to %i, %i\n", w, h);
	renderer.resize(w, h); }

uint rid = 1;
void OnKey(unsigned char key, int, int) {
	switch (key) {
		case 'w': case 'W': break;
		case 's': case 'S': break;
		case 'r': case 'R': {
			switch (rid) {
				case 0: glutReshapeWindow(640, 576); rid = 1; break;
				case 1: glutReshapeWindow(1024, 576); rid = 2; break;
				case 2: glutReshapeWindow(1920, 576); rid = 0; break;
			}
		}
	}
}

int main(int argc, char** argv) {
	//GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_CORE_PROFILE | GLUT_DEBUG);
	glutInitWindowSize(640, 576);
	glutCreateWindow("RTT");

	// GLEW
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
	else if (GLEW_VERSION_3_2) std::cout << "Driver supports OpenGL 3.2\nDetails:" << std::endl;

	std::cout << "Using GLEW "<< glewGetString(GLEW_VERSION) << std::endl;
	std::cout << "Vendor: "<< glGetString (GL_VENDOR) << std::endl;
	std::cout << "Renderer: "<< glGetString (GL_RENDERER) << std::endl;
	std::cout << "Version: "<< glGetString (GL_VERSION) << std::endl;
	std::cout << "GLSL: "<< glGetString (GL_SHADING_LANGUAGE_VERSION) << std::endl;

	InitGL();
	glutDisplayFunc(OnRender);
	glutReshapeFunc(OnResize);
	glutKeyboardFunc(OnKey);
	glutIdleFunc(OnIdle);
	glutMainLoop();

	return 0;
}

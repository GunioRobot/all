#include <cstdio>
#include "blas.hpp"

#include <algorithm>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include "GLSLShader.h"
#include <cassert>

#define GL_CHECK_ERRORS assert(glGetError() == GL_NO_ERROR);

typedef struct{
	float r,g,b;
}color_type;

// The polygon (triangle), 3 numbers that aim 3 vertices
typedef struct{
	uint a,b,c;
}polygon_type;

struct vertex_type {
	float x,y,z;
};

// The mapcoord type, 2 texture coordinates for each vertex
typedef struct{
	float u,v;
}mapcoord_type;

#define MAX_VERTICES 2000
#define MAX_POLYGONS 2000

// The object type
typedef struct {
    vertex_type vertex[MAX_VERTICES];
    polygon_type polygon[MAX_POLYGONS];
    mapcoord_type mapcoord[MAX_VERTICES];
    color_type colors[MAX_VERTICES];
} obj_type;

obj_type cube = {
	{
		{-1, -1, -1},
		{1, -1, -1},
		{-1, 1, -1},
		{1, 1, -1},

		{-1, -1, 1},
		{1, -1, 1},
		{-1, 1, 1},
		{1, 1, 1}
	},
	{
		{0, 1, 3}, // back
		{0, 2, 3},

		{0, 4, 5}, // bottom
		{0, 1, 5},

		{0, 4, 6}, // left
		{0, 2, 6},

		{1, 5, 7}, // right
		{1, 3, 7},

		{2, 6, 7}, // top
		{2, 3, 7},

		{4, 5, 7}, // front
		{4, 6, 7}
	},
	{
		{0, 0},
		{1, 0},
		{0, 1},
		{1, 1},

		{0, 0},
		{1, 0},
		{0, 1},
		{1, 1}
	},
	{
		{1, 0, 0},
		{0, 1, 0},
		{0, 0, 1},
		{1, 0, 0},
		{0, 1, 0},
		{0, 0, 1},
		{1, 0, 0},
		{0, 1, 0}
	}
};

uint n_indices = sizeof(cube.polygon) / sizeof(cube.polygon[0]);

GLuint vboVerticesID, vboColorsID, vboIndicesID, vaoID;
GLuint sqvboTexCoordID, sqvaoID;

GLSLShader colorshader, texshader;

int filling = 1;

// Projection and View matrices
Matrix4 P, V(1);

void InitShaders() {
	colorshader.LoadFromFile(GL_VERTEX_SHADER, "shader.vert");
	colorshader.LoadFromFile(GL_FRAGMENT_SHADER, "shader.frag");
	colorshader.CreateAndLinkProgram();
	colorshader.Use();
		colorshader.AddAttribute("vVertex");
		colorshader.AddAttribute("vColor");
		colorshader.AddUniform("MVP");
	colorshader.UnUse();

	GL_CHECK_ERRORS

	texshader.LoadFromFile(GL_VERTEX_SHADER, "tex_shader.vert");
	texshader.LoadFromFile(GL_FRAGMENT_SHADER, "tex_shader.frag");
	texshader.CreateAndLinkProgram();
	texshader.Use();
		texshader.AddAttribute("vVertex");
		texshader.AddAttribute("vUV");
		texshader.AddUniform("MVP");
		texshader.AddUniform("textureMap");
		glUniform1i(texshader("textureMap"),0);
	texshader.UnUse();

	GL_CHECK_ERRORS
}
void InitVAO() {
	glGenVertexArrays(1, &vaoID);
	glGenBuffers(1, &vboVerticesID);
	glGenBuffers(1, &vboColorsID);
	glGenBuffers(1, &vboIndicesID);
	GL_CHECK_ERRORS

	glBindVertexArray(vaoID);
		glBindBuffer (GL_ARRAY_BUFFER, vboVerticesID);
		glBufferData (GL_ARRAY_BUFFER, sizeof(cube.vertex), &cube.vertex[0], GL_STATIC_DRAW);
		GL_CHECK_ERRORS
		glEnableVertexAttribArray(colorshader["vVertex"]);
		glVertexAttribPointer (colorshader["vVertex"], 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*3 ,0);
		GL_CHECK_ERRORS

		glBindBuffer (GL_ARRAY_BUFFER, vboColorsID);
		glBufferData (GL_ARRAY_BUFFER, sizeof(cube.colors), &cube.colors[0], GL_STATIC_DRAW);
		GL_CHECK_ERRORS
		glEnableVertexAttribArray(colorshader["vColor"]);
		glVertexAttribPointer (colorshader["vColor"], 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*3 ,0);
		GL_CHECK_ERRORS

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube.polygon), &cube.polygon[0], GL_STATIC_DRAW);
		GL_CHECK_ERRORS
	glBindVertexArray(0);

	GL_CHECK_ERRORS
}
void InitSQVAO() {
	glGenVertexArrays(1, &sqvaoID);
	glGenBuffers(1, &sqvboTexCoordID);
	GL_CHECK_ERRORS

	glBindVertexArray(sqvaoID);
		glBindBuffer(GL_ARRAY_BUFFER, vboVerticesID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube.vertex), &cube.vertex[0], GL_STATIC_DRAW);
		GL_CHECK_ERRORS
		glEnableVertexAttribArray(colorshader["vVertex"]);
		glVertexAttribPointer(colorshader["vVertex"], 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*3 ,0);
		GL_CHECK_ERRORS

		glBindBuffer(GL_ARRAY_BUFFER, sqvboTexCoordID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube.mapcoord), &cube.mapcoord[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(texshader["vUV"]);
		glVertexAttribPointer(texshader["vUV"], 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*2 ,0);
		GL_CHECK_ERRORS

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube.polygon), &cube.polygon[0], GL_STATIC_DRAW);
		GL_CHECK_ERRORS
	glBindVertexArray(0);

	GL_CHECK_ERRORS
}

class Timer {
	double m_previous, m_delta;
public:
	Timer() : m_previous(0), m_delta(0) {}

	double delta() const { return m_delta; }
	void update() {
		const double current = (double)glutGet(GLUT_ELAPSED_TIME) / 1000.0;

		m_delta = current - m_previous;
		m_previous = current;
	}
};

class Accumulator {
	double m_accum, m_resolution;
public:
	Accumulator(double resolution) : m_accum(0), m_resolution(resolution) {}
	bool ready() const { return m_accum > m_resolution; }

	void add(double dt) { m_accum += dt; }
	void reset() { m_accum = 0; }
};

struct RenderDetail {
	Matrix4 MVP;
	uint vao, indices;

	RenderDetail(const Matrix4& mvp, uint v, uint i) : MVP(mvp), vao(v), indices(i) {}
};

class Viewport {
	uint m_width, m_height;
public:
	Viewport(uint width, uint height) : m_width(width), m_height(height) {}

	void bind() { glViewport(0, 0, m_width, m_height); }
	void resize(uint width, uint height) { m_width = width; m_height = height; }
};

class Renderer {

	std::vector<RenderDetail> queue;
	std::vector<RenderDetail> texqueue;

	unsigned int fbo_id;
	unsigned int depth_buffer;
	int m_FBOWidth, m_FBOHeight;
	unsigned int m_TextureID;

	Viewport m_FBOViewport, m_ScreenViewport;
public:
	Renderer(int sw, int sh, int fw, int fh) : m_FBOWidth(fw), m_FBOHeight(fh), m_FBOViewport(fw, fh), m_ScreenViewport(sw, sh) {}
	~Renderer() {
		glDeleteFramebuffers(1, &fbo_id);
	}

	void enqueue(const RenderDetail& rd) { queue.push_back(rd); }
	void texenqueue(const RenderDetail& rd) { texqueue.push_back(rd); }
	void resize(int sw, int sh) {
		m_ScreenViewport.resize(sw, sh);
	}

	void initFBO() {
		//const GLenum fbo_buffers[] = { GL_COLOR_ATTACHMENT0 };

		glGenRenderbuffers(1, &depth_buffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_FBOWidth, m_FBOHeight);

		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_FBOWidth, m_FBOHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glGenFramebuffers(1, &fbo_id);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo_id);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureID, 0);

		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void render() {
		uint prevVAO = 0;

		glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);
		m_FBOViewport.bind();

		glClearColor(0, 0, 0.2f,0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		colorshader.Use();
		const uint csmvp = colorshader("MVP");
		for (size_t i = 0; i < queue.size(); ++i) {
			const RenderDetail& rd = queue[i];

			glUniformMatrix4fv(csmvp, 1, GL_FALSE, &rd.MVP[0]);
			if (prevVAO != rd.vao) {
				prevVAO = rd.vao;
				glBindVertexArray(rd.vao);
			}
			glDrawElements(GL_TRIANGLES, rd.indices, GL_UNSIGNED_INT, 0);
		}
		//colorshader.UnUse();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		m_ScreenViewport.bind();

		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		texshader.Use();
		glBindTexture(GL_TEXTURE_2D, m_TextureID);

		const uint tsmvp = texshader("MVP");
		for (size_t i = 0; i < texqueue.size(); ++i) {
			const RenderDetail& rd = texqueue[i];

			glUniformMatrix4fv(tsmvp, 1, GL_FALSE, &rd.MVP[0]);
			if (prevVAO != rd.vao) {
				prevVAO = rd.vao;
				glBindVertexArray(rd.vao);
			}
			glDrawElements(GL_TRIANGLES, rd.indices, GL_UNSIGNED_INT, 0);
		}
		//texshader.UnUse();

		queue.clear();
		texqueue.clear();

		glutSwapBuffers();
	}
};

Renderer renderer(800, 600, 320, 240);

void drawBox(const Matrix4& M) {
	const Matrix4 MVP = M * V * P;
	renderer.enqueue(RenderDetail(MVP, vaoID, n_indices));
}

void drawTexBox(const Matrix4& M) {
	const Matrix4 MVP = M * V * P;
	renderer.texenqueue(RenderDetail(MVP, sqvaoID, n_indices));
}

// Absolute rotation values (0-359 degrees) and rotiation increments for each frame
float rotation_x = 0;
float rotation_y = 0;
float rotation_z = 0;

float drawdist = 40.f;

void drawColouredRotatingBox() {
	Matrix4 M(1);
	scale(M, Vector3(10,10,10));
	rotate(M, rotation(Vector3(1, 0, 0), rotation_x));
	rotate(M, rotation(Vector3(0, 1, 0), rotation_y));
	rotate(M, rotation(Vector3(0, 0, 1), rotation_z));

	drawdist += 0.2f;
	translate(M, Vector3(0,0, -drawdist));
	if (drawdist > 100) drawdist = 40;

	drawBox(M);
}

void drawTexturedRotatingBox() {
	Matrix4 M(1);
	scale(M, Vector3(10,10,10));
	rotate(M, rotation(Vector3(1, 0, 0), rotation_x));
	translate(M, Vector3(0,0,-50));

	drawTexBox(M);
}

void OnRender() {
	for (int i = 0; i < 2; ++i) {
		drawTexturedRotatingBox();
		drawColouredRotatingBox();
	}
	renderer.render();
}

void OnResize(int w, int h) {
	renderer.resize(w, h);
	P = perspective(45.0f, (GLfloat)w / (GLfloat)h, 1.f, 200.f);
}

void OnShutdown() {
	glDeleteBuffers(1, &vboVerticesID);
	glDeleteBuffers(1, &vboColorsID);
	glDeleteBuffers(1, &sqvboTexCoordID);
	glDeleteBuffers(1, &vboIndicesID);

	glDeleteVertexArrays(1, &vaoID);
	glDeleteVertexArrays(1, &sqvaoID);
}

void OnKey(unsigned char key, int, int) {
	switch (key) {
		case 'w': case 'W': relative_translate(V, Vector3(0,0,1)); break;
		case 's': case 'S': relative_translate(V, Vector3(0,0,-1)); break;
	}
}

Timer framet;
Accumulator fps(1.0), input(1.0 / 30.0), render(1.0 / 60.0), spinner(1.0 / 120.0);

uint frames;

void OnIdle() {
	framet.update();
	const double dt = framet.delta();
	fps.add(dt); input.add(dt); render.add(dt); spinner.add(dt);

	++frames;
	if (fps.ready()) {
		printf("%i %f\n", frames, dt);
		frames = 0;
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
		rotation_x = rotation_x + 0.000f;
		rotation_y = rotation_y + 0.002f;
		rotation_z = rotation_z + 0.000f;
		if (rotation_x > 359) rotation_x = 0;
		if (rotation_y > 359) rotation_y = 0;
		if (rotation_z > 359) rotation_z = 0;

		spinner.reset();
	}
}

void InitGL() {
	glGetError();
	GL_CHECK_ERRORS

	InitShaders();
	InitVAO();
	InitSQVAO();
	renderer.initFBO();
	GL_CHECK_ERRORS

	glEnable(GL_DEPTH_TEST);
	GL_CHECK_ERRORS
}

int main(int argc, char** argv) {
	//GLUT
	atexit(OnShutdown);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_CORE_PROFILE | GLUT_DEBUG);
	glutInitWindowSize(800, 600);
	glutCreateWindow("RTT");

	// GLEW
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
	else if (GLEW_VERSION_3_2) std::cout << "Driver supports OpenGL 3.2\nDetails:" << std::endl;

	std::cout << "Using GLEW "<<glewGetString(GLEW_VERSION) << std::endl;
	std::cout << "Vendor: "<<glGetString (GL_VENDOR) << std::endl;
	std::cout << "Renderer: "<<glGetString (GL_RENDERER) << std::endl;
	std::cout << "Version: "<<glGetString (GL_VERSION) << std::endl;
	std::cout << "GLSL: "<<glGetString (GL_SHADING_LANGUAGE_VERSION) << std::endl;

	InitGL();
	glutDisplayFunc(OnRender);
	glutReshapeFunc(OnResize);
	glutKeyboardFunc(OnKey);
	glutIdleFunc(OnIdle);
	glutMainLoop();

	return 0;
}

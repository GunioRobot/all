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


double Timer::getTime() { return (double)glutGet(GLUT_ELAPSED_TIME) / 1000.0; }

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
};

class Renderer {
	int screen_width;
	int screen_height;

	std::vector<RenderDetail> queue;

	unsigned int fbo_id;
	unsigned int depth_buffer;
	int m_FBOWidth, m_FBOHeight;
	unsigned int m_TextureID, m_TextureID2;

	Viewport m_FBOViewport, m_ScreenViewport;
public:
	Renderer(int sw, int sh, int fw, int fh) : screen_width(sw), screen_height(sh), m_FBOWidth(fw), m_FBOHeight(fh), m_FBOViewport(fw, fh), m_ScreenViewport(sw, sh) {}
	~Renderer() {
		glDeleteFramebuffers(1, &fbo_id);
	}

	void enqueue(const RenderDetail& rd) { queue.push_back(rd); }
	void resize(int sw, int sh) { screen_width = sw; screen_height = sh; }

	void initFBO() {
		glGenRenderbuffers(1, &depth_buffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_FBOWidth, m_FBOHeight);

		// (4 bytes * m_FBOWidth * m_FBOHeight) Data Array (Texture)
		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_FBOWidth, m_FBOHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glBindTexture(GL_TEXTURE_2D, 0);

		// (4 bytes * m_FBOWidth * m_FBOHeight) Data Array (Texture)
		glGenTextures(1, &m_TextureID2);
		glBindTexture(GL_TEXTURE_2D, m_TextureID2);
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
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_TextureID2, 0);

		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void render() {

	}
};

Renderer renderer(800, 600, 320, 240);
float rotation_x = 0, rotation_y = 0, rotation_z = 0;

void OnRender() {
	Matrix4 M(1);
	scale(M, Vector3(10,10,10));
	rotate(M, rotation(Vector3(1, 0, 0), rotation_x));
	rotate(M, rotation(Vector3(0, 1, 0), rotation_y));
	rotate(M, rotation(Vector3(0, 0, 1), rotation_z));
	translate(M, Vector3(0,0,-50));

	const Matrix4 MVP = M * V * P;
	renderer.enqueue(RenderDetail(MVP, vaoID, n_indices));

	renderer.render();
}


// INPUT BASED OPERATIONS
void OnResize(int w, int h) {
	renderer.resize(w, h);
	P = perspective(45.0f, (GLfloat)w / (GLfloat)h, 1.f, 100.f);
}

void OnKey(unsigned char key, int, int) {
	switch (key) {
		case 'w': case 'W': break;
		case 's': case 'S': break;
	}
}

Timer framet;
Accumulator<double> fps(1.0), input(1.0 / 30.0), render(1.0 / 60.0), spinner(1.0 / 120.0);

uint frames;

void OnIdle() {
	framet.update();
	const double dt = framet.delta();
	fps.add(dt); input.add(dt); render.add(dt); spinner.add(dt);

	++frames;
	if (fps.ready()) {
		printf("%i %f\n", frames, dt);
		printf("%f %f %f\n", rotation_x, rotation_y, rotation_z);
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
		rotation_x = rotation_x + 0.0000f;
		rotation_y = rotation_y + 0.0002f;
		rotation_z = rotation_z + 0.0000f;
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

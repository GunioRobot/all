#include "blas.hpp"

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include "GLSLShader.h"
#include "obj_type.h"
#include <cassert>

#define GL_CHECK_ERRORS assert(glGetError() == GL_NO_ERROR);

int screen_width = 800;
int screen_height = 600;

const GLfloat colors[] = {
	1,0,0,	0,1,0, 0,0,1,
	1,0,0,	0,1,0, 0,0,1,
	1,0,0,	0,1,0,
};

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
		{0, 1, 2},
		{1, 2, 3},

		{0, 4, 2},
		{2,

		{4, 5, 6},
		{5, 6, 7}
	},
	{
		{1, 1},
		{0, 1},
		{1, 0},
		{0, 0},
		{0, 0},
		{1, 0},
		{0, 1},
		{1, 1}
	}
};

obj_type square = {
	{
		{-1, -1, 0},
		{1, -1, 0},
		{-1, 1, 0},
		{1, 1, 0}
	},
	{
		{0, 1, 2},
		{1, 2, 3}
	},
	{
		{0, 0},
		{1, 0},
		{0, 1},
		{1, 1}
	}
};

GLuint sqvboVerticesID, sqvboColorsID, sqvboTexCoordID, sqvboIndicesID, sqvaoID;
GLuint vboVerticesID, vboColorsID, vboTexCoordID, vboIndicesID, vaoID;
GLSLShader colorshader, texshader;
GLSLShader* boundshader = 0;

int filling=1;

// Absolute rotation values (0-359 degrees) and rotiation increments for each frame
float rotation_x=0, rotation_x_increment = 0.005f;
float rotation_y=0, rotation_y_increment = 0.003f;
float rotation_z=0, rotation_z_increment = 0.001f;

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
	glGenBuffers(1, &vboTexCoordID);
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

		glBindBuffer (GL_ARRAY_BUFFER, vboTexCoordID);
		glBufferData (GL_ARRAY_BUFFER, sizeof(cube.mapcoord), &cube.mapcoord[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(colorshader["vUV"]);
		glVertexAttribPointer (colorshader["vUV"], 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*2 ,0);
		GL_CHECK_ERRORS

		glBindBuffer (GL_ARRAY_BUFFER, vboColorsID);
		glBufferData (GL_ARRAY_BUFFER, sizeof(colors), &colors[0], GL_STATIC_DRAW);
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
	glGenBuffers(1, &sqvboVerticesID);
	glGenBuffers(1, &sqvboTexCoordID);
	glGenBuffers(1, &sqvboIndicesID);
	GL_CHECK_ERRORS

	glBindVertexArray(sqvaoID);
		glBindBuffer(GL_ARRAY_BUFFER, sqvboVerticesID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(square.vertex), &square.vertex[0], GL_STATIC_DRAW);
		GL_CHECK_ERRORS
		glEnableVertexAttribArray(colorshader["vVertex"]);
		glVertexAttribPointer(colorshader["vVertex"], 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*3 ,0);
		GL_CHECK_ERRORS

		glBindBuffer(GL_ARRAY_BUFFER, sqvboTexCoordID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(square.mapcoord), &square.mapcoord[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(colorshader["vUV"]);
		glVertexAttribPointer(colorshader["vUV"], 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*2 ,0);
		GL_CHECK_ERRORS

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sqvboIndicesID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(square.polygon), &square.polygon[0], GL_STATIC_DRAW);
		GL_CHECK_ERRORS
	glBindVertexArray(0);

	GL_CHECK_ERRORS
}

unsigned int fbo_id;
unsigned int depth_buffer;
int m_FBOWidth, m_FBOHeight;
unsigned int m_TextureID;

GLenum fbo_buffers[] = { GL_COLOR_ATTACHMENT0 };

void initFBO() {
	m_FBOWidth = screen_width;
	m_FBOHeight = screen_height;

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

void InitGL() {
	glGetError();
	GL_CHECK_ERRORS

	InitShaders();
	InitVAO();
	InitSQVAO();
	initFBO();
	GL_CHECK_ERRORS

	glEnable(GL_DEPTH_TEST);
	GL_CHECK_ERRORS
}

void drawBox(const Matrix4& M) {
	const Matrix4 MVP = M * V * P;

	if (boundshader) {
		glUniformMatrix4fv((*boundshader)("MVP"), 1, GL_FALSE, &MVP[0]);
	}

	glBindVertexArray(vaoID);
	glDrawElements(GL_TRIANGLES, sizeof(cube.polygon)/sizeof(cube.polygon[0]), GL_UNSIGNED_INT, 0);
}

void drawSquare(const Matrix4& M) {
	const Matrix4 MVP = M * V * P;

	if (boundshader) {
		glUniformMatrix4fv((*boundshader)("MVP"), 1, GL_FALSE, &MVP[0]);
	}

	glBindVertexArray(sqvaoID);
	glDrawElements(GL_TRIANGLES, sizeof(square.polygon)/sizeof(square.polygon[0]), GL_UNSIGNED_INT, 0);
}

void drawStaticSquare() {
	Matrix4 M(1);
	scale(M, Vector3(10,10,10));
	translate(M, Vector3(0,0,-50));

	drawSquare(M);
}

void drawRotatingSquare() {
	Matrix4 M(1);
	scale(M, Vector3(10,10,10));
	rotate(M, rotation(Vector3(1, 0, 0), rotation_x));
	rotate(M, rotation(Vector3(0, 1, 0), rotation_y));
	rotate(M, rotation(Vector3(0, 0, 1), rotation_z));
	translate(M, Vector3(0,0,-50));

	drawSquare(M);
}

void drawStaticBox() {
	Matrix4 M(1);
	translate(M, Vector3(0,0,-50));

	drawBox(M);
}

void drawRotatingBox() {
	Matrix4 M(1);
	rotate(M, rotation(Vector3(1, 0, 0), rotation_x));
	rotate(M, rotation(Vector3(0, 1, 0), rotation_y));
	rotate(M, rotation(Vector3(0, 0, 1), rotation_z));
	translate(M, Vector3(0,0,-50));

	drawBox(M);
}

void OnRender() {
	/////////////////////////////////////////
	// Render to FBO

	glClearColor(0, 0, 0.2f,0);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);
	glViewport(0, 0, m_FBOWidth, m_FBOHeight);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	GL_CHECK_ERRORS

	colorshader.Use();
	boundshader = &colorshader;

	drawRotatingBox();

	colorshader.UnUse();

	/////////////////////////////////////////
	// Render to Window

	glClearColor(0, 0, 0, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, screen_width, screen_height);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	GL_CHECK_ERRORS

	texshader.Use();
	boundshader = &texshader;

	glBindTexture(GL_TEXTURE_2D, m_TextureID);
	//drawStaticSquare();
	drawRotatingSquare();

	texshader.UnUse();

	// Swap le buffers

	glutSwapBuffers();
}

void OnResize(int w, int h) {
	screen_width = w;
	screen_height = h;
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	P = perspective(45.0f, (GLfloat)w / (GLfloat)h, 1.f, 2000.f);
}

void OnShutdown() {
	glDeleteBuffers(1, &vboVerticesID);
	glDeleteBuffers(1, &vboColorsID);
	glDeleteBuffers(1, &vboTexCoordID);
	glDeleteBuffers(1, &vboIndicesID);
	glDeleteVertexArrays(1, &vaoID);

	// cleanup the FBO and that too... (cbf for now)
	glDeleteFramebuffers(1, &fbo_id);
}

void OnKey(unsigned char key, int, int) {
	switch (key) {
		case 'w': case 'W': relative_translate(V, Vector3(0,0,1)); break;
		case 's': case 'S': relative_translate(V, Vector3(0,0,-1)); break;
	}
}

void OnIdle() {
	rotation_x = rotation_x + rotation_x_increment;
	rotation_y = rotation_y + rotation_y_increment;
	rotation_z = rotation_z + rotation_z_increment;

	if (rotation_x > 359) rotation_x = 0;
	if (rotation_y > 359) rotation_y = 0;
	if (rotation_z > 359) rotation_z = 0;

	glutPostRedisplay();
}

int main(int argc, char** argv) {
	//GLUT
	atexit(OnShutdown);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_CORE_PROFILE | GLUT_DEBUG);
	glutInitWindowSize(screen_width, screen_height);
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

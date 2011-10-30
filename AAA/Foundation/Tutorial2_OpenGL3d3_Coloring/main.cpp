/*
 * ---------------- www.spacesimulator.net --------------
 *   ---- Space simulators and 3d engine tutorials ----
 *
 * Original Author: Damiano Vitulli
 * Porting to OpenGL3.3: Movania Muhammad Mobeen
 * Shaders Functions: Movania Muhammad Mobeen
 * Standards Compliance + GLM Compliant : Daniel Cousens
 *
 * This program is released under the BSD licence
 * By using this program you agree to licence terms on spacesimulator.net copyright page
 *
 */

#include "blas.hpp"

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include "GLSLShader.h"
#include "obj_type.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cassert>

#define GL_CHECK_ERRORS assert(glGetError() == GL_NO_ERROR);

using namespace std;

int screen_width=640;
int screen_height=480;

const GLfloat colors[] = {
	1,0,0,	0,1,0, 0,0,1,
	1,0,0,	0,1,0, 0,0,1,
	1,0,0,	0,1,0,
};

obj_type cube =
{
	{
		{-10, -10, 10},   // vertex v0
		{10,  -10, 10},   // vertex v1
		{10,  -10, -10},  // vertex v2
		{-10, -10, -10},  // vertex v3
		{-10, 10,  10},   // vertex v4
		{10,  10,  10},   // vertex v5
		{10,  10,  -10},  // vertex v6
		{-10, 10,  -10}   // vertex v7
	},
	{
		{0, 1, 4},  // polygon v0,v1,v4
		{4, 1, 5},  // polygon v1,v5,v4
		{1, 2, 5},  // polygon v1,v2,v5
		{2, 6, 5},  // polygon v2,v6,v5
		{2, 3, 6},  // polygon v2,v3,v6
		{3, 7, 6},  // polygon v3,v7,v6
		{3, 0, 7},  // polygon v3,v0,v7
		{0, 4, 7},  // polygon v0,v4,v7
		{4, 5, 7},  // polygon v4,v5,v7
		{5, 6, 7},  // polygon v5,v6,v7
		{3, 2, 0},  // polygon v3,v2,v0
		{2, 1, 0},  // polygon v2,v1,v0
	}
};

GLuint vboVerticesID, vboColorsID, vboIndicesID, vaoID;
GLint vVertexLoc, vColorLoc;
GLsizei stride = sizeof(GLfloat)*3;
GLSLShader shader;

int filling=1;
// Absolute rotation values (0-359 degrees) and rotiation increments for each frame

//#define USE_GLM

#ifdef USE_GLM
	float rotation_x=0, rotation_x_increment=0.005f;
	float rotation_y=0, rotation_y_increment=0.003f;
	float rotation_z=0, rotation_z_increment=0.001f;
	glm::mat4 P;	//projection matrix;
#else
	float rotation_x=0, rotation_x_increment=0.005f;
	float rotation_y=0, rotation_y_increment=0.003f;
	float rotation_z=0, rotation_z_increment=0.001f;
	Matrix4 P;
#endif

void InitShaders(void)
{
	shader.LoadFromFile(GL_VERTEX_SHADER, "shader.vert");
	shader.LoadFromFile(GL_FRAGMENT_SHADER, "shader.frag");
	shader.CreateAndLinkProgram();
	shader.Use();
		shader.AddAttribute("vVertex");
		shader.AddUniform("MVP");
	shader.UnUse();

	GL_CHECK_ERRORS
}
void InitVAO() {
	GL_CHECK_ERRORS
	//Create vao and vbo stuff
	glGenVertexArrays(1, &vaoID);
	glGenBuffers (1, &vboVerticesID);
	glGenBuffers (1, &vboColorsID);
	glGenBuffers (1, &vboIndicesID);

	GL_CHECK_ERRORS

	glBindVertexArray(vaoID);
		glBindBuffer (GL_ARRAY_BUFFER, vboVerticesID);
		glBufferData (GL_ARRAY_BUFFER, sizeof(cube.vertex), &cube.vertex[0], GL_STATIC_DRAW);
		GL_CHECK_ERRORS
		glEnableVertexAttribArray(shader["vVertex"]);
		glVertexAttribPointer (shader["vVertex"], 3, GL_FLOAT, GL_FALSE,stride,0);
		GL_CHECK_ERRORS
		glBindBuffer (GL_ARRAY_BUFFER, vboColorsID);
		glBufferData (GL_ARRAY_BUFFER, sizeof(colors), &colors[0], GL_STATIC_DRAW);
		GL_CHECK_ERRORS
		glEnableVertexAttribArray(shader["vColor"]);
		glVertexAttribPointer (shader["vColor"], 3, GL_FLOAT, GL_FALSE,stride,0);
		GL_CHECK_ERRORS
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube.polygon), &cube.polygon[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

	GL_CHECK_ERRORS
}
void InitGL() {
	glGetError();
	GL_CHECK_ERRORS
	glClearColor(0.0f,0.0f,0.2f,0.0f);
	GL_CHECK_ERRORS
	InitShaders();
	InitVAO();
	glEnable(GL_DEPTH_TEST); // We enable the depth test (also called z buffer)
	glPolygonMode (GL_FRONT_AND_BACK, GL_FILL); // Polygon rasterization mode (polygon filled)
	GL_CHECK_ERRORS
}

#ifdef USE_GLM
void OnRender() {
	GL_CHECK_ERRORS
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	//setup matrices
	glm::mat4 T	= glm::translate(glm::mat4(1.0f),glm::vec3(0.0f, 0.0f, -50));
	glm::mat4 Rx	= glm::rotate(T,  rotation_x, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 Ry	= glm::rotate(Rx, rotation_y, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 MV	= glm::rotate(Ry, rotation_z, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 MVP	= P*MV;

	glBindVertexArray(vaoID);
	shader.Use();
		glUniformMatrix4fv(shader("MVP"), 1, GL_FALSE, glm::value_ptr(MVP));
		glDrawElements(GL_TRIANGLES, sizeof(cube.polygon)/sizeof(cube.polygon[0]), GL_UNSIGNED_INT, 0);
	shader.UnUse();
	glBindVertexArray(0);

	glutSwapBuffers();
}
#else
void OnRender() {
	GL_CHECK_ERRORS
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	//setup matrices
	Matrix4 T(1);
	relative_translate(T, Vector3(0.0f, 0.0f, -50));

	Matrix4 Rx(T);
	relative_rotate(Rx, rotation(Vector3(1.0f, 0.0f, 0.0f), rotation_x));

	Matrix4 Ry(Rx);
	relative_rotate(Ry, rotation(Vector3(0.0f, 1.0f, 0.0f), rotation_y));

	Matrix4 MV(Ry);
	relative_rotate(MV, rotation(Vector3(0.0f, 0.0f, 1.0f), rotation_z));

	Matrix4 MVP = MV * P;

	glBindVertexArray(vaoID);
	shader.Use();
	glUniformMatrix4fv(shader("MVP"), 1, GL_FALSE, &MVP[0]);
	glDrawElements(GL_TRIANGLES, sizeof(cube.polygon)/sizeof(cube.polygon[0]), GL_UNSIGNED_INT, 0);
	shader.UnUse();
	glBindVertexArray(0);

	glutSwapBuffers();
}
#endif

void OnResize(int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	//setup the projection matrix
#ifdef USE_GLM
	P = glm::perspective(45.0f, (GLfloat)w / (GLfloat)h, 1.f, 1000.f);
#else
	P = perspective(45.0f, (GLfloat)w / (GLfloat)h, 1.f, 1000.f);
#endif
}
void OnShutdown() {
	glDeleteBuffers(1, &vboVerticesID);
	glDeleteBuffers(1, &vboColorsID);
	glDeleteBuffers(1, &vboIndicesID);
	glDeleteVertexArrays(1, &vaoID);
}
void OnKey(unsigned char key, int, int)
{
	switch (key)
	{
		case ' ':
			rotation_x_increment=0;
			rotation_y_increment=0;
			rotation_z_increment=0;
		break;
		case 'r': case 'R':
			if (filling==0)
			{
				glPolygonMode (GL_FRONT_AND_BACK, GL_FILL); // Polygon rasterization mode (polygon filled)
				filling=1;
			}
			else
			{
				glPolygonMode (GL_FRONT_AND_BACK, GL_LINE); // Polygon rasterization mode (polygon outlined)
				filling=0;
			}
		break;
	}
}
void OnSpecialKey(int key, int, int)
{
	switch (key)
	{
		case GLUT_KEY_UP:
			rotation_x_increment = rotation_x_increment +0.005f;
		break;
		case GLUT_KEY_DOWN:
			rotation_x_increment = rotation_x_increment -0.005f;
		break;
		case GLUT_KEY_LEFT:
			rotation_y_increment = rotation_y_increment +0.005f;
		break;
		case GLUT_KEY_RIGHT:
			rotation_y_increment = rotation_y_increment -0.005f;
		break;
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
	atexit(OnShutdown);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitContextVersion (3, 3);
	glutInitContextFlags (GLUT_CORE_PROFILE | GLUT_DEBUG);
	glutInitWindowSize(screen_width, screen_height);
	glutCreateWindow("Tutorial 2 - OpenGL 3.3");
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)	{
		cerr<<"Error: "<<glewGetErrorString(err)<<endl;
	} else {
		if (GLEW_VERSION_3_2)
		{
			cout<<"Driver supports OpenGL 3.2\nDetails:"<<endl;
		}
	}
	cout<<"Using GLEW "<<glewGetString(GLEW_VERSION)<<endl;
	cout<<"Vendor: "<<glGetString (GL_VENDOR)<<endl;
	cout<<"Renderer: "<<glGetString (GL_RENDERER)<<endl;
	cout<<"Version: "<<glGetString (GL_VERSION)<<endl;
	cout<<"GLSL: "<<glGetString (GL_SHADING_LANGUAGE_VERSION)<<endl;
	InitGL();
	glutDisplayFunc(OnRender);
	glutReshapeFunc(OnResize);
	glutKeyboardFunc(OnKey);
	glutSpecialFunc(OnSpecialKey);
	glutIdleFunc(OnIdle);
	glutMainLoop();

	return 0;
}

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
#include "tga.hpp"

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
	},
	{
		{0.0, 0.0},  // mapping coordinates for vertex v0
		{1.0, 0.0},  // mapping coordinates for vertex v1
		{1.0, 0.0},  // mapping coordinates for vertex v2
		{0.0, 0.0},  // mapping coordinates for vertex v3
		{0.0, 1.0},  // mapping coordinates for vertex v4
		{1.0, 1.0},  // mapping coordinates for vertex v5
		{1.0, 1.0},  // mapping coordinates for vertex v6
		{0.0, 1.0}   // mapping coordinates for vertex v7
	},
	0,
};

GLuint vboVerticesID, vboTexCoordID, vboIndicesID, vaoID;
GLint vVertexLoc, vColorLoc;
GLsizei stride = sizeof(GLfloat)*3;
GLSLShader shader;

int filling=1;

// Absolute rotation values (0-359 degrees) and rotiation increments for each frame
float rotation_x=0, rotation_x_increment=0.005f;
float rotation_y=0, rotation_y_increment=0.003f;
float rotation_z=0, rotation_z_increment=0.001f;
Matrix4 P, V(1);

void InitShaders(void)
{
	shader.LoadFromFile(GL_VERTEX_SHADER, "shader.vert");
	shader.LoadFromFile(GL_FRAGMENT_SHADER, "shader.frag");
	shader.CreateAndLinkProgram();
	shader.Use();
		shader.AddAttribute("vVertex");
		shader.AddAttribute("vUV");
		shader.AddUniform("MVP");
		shader.AddUniform("textureMap");
		glUniform1i(shader("textureMap"),0);
	shader.UnUse();

	GL_CHECK_ERRORS
}
void InitVAO() {
	GL_CHECK_ERRORS
	//Create vao and vbo stuff
	glGenVertexArrays(1, &vaoID);
	glGenBuffers (1, &vboVerticesID);
	glGenBuffers (1, &vboTexCoordID);
	glGenBuffers (1, &vboIndicesID);

	GL_CHECK_ERRORS

	glBindVertexArray(vaoID);
		glBindBuffer (GL_ARRAY_BUFFER, vboVerticesID);
		glBufferData (GL_ARRAY_BUFFER, sizeof(cube.vertex), &cube.vertex[0], GL_STATIC_DRAW);
		GL_CHECK_ERRORS
		glEnableVertexAttribArray(shader["vVertex"]);
		glVertexAttribPointer (shader["vVertex"], 3, GL_FLOAT, GL_FALSE,stride,0);
		GL_CHECK_ERRORS
		glBindBuffer (GL_ARRAY_BUFFER, vboTexCoordID);
		glBufferData (GL_ARRAY_BUFFER, sizeof(cube.mapcoord), &cube.mapcoord[0], GL_STATIC_DRAW);
		GL_CHECK_ERRORS
		glEnableVertexAttribArray(shader["vUV"]);
		glVertexAttribPointer (shader["vUV"], 2, GL_FLOAT, GL_FALSE,sizeof(GLfloat)*2,0);
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

	Image img; // auto destroys loaded resources after
	load_tga_image("texture1.tga", img);

	glBindTexture(GL_TEXTURE_2D, 0); // Bind the ID texture specified by the 2nd parameter
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width, img.height, 0, GL_BGR, GL_UNSIGNED_BYTE, &img.data[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);
	glGenerateMipmap(GL_TEXTURE_2D);
	cube.id_texture = 0;

	 // If the last function returns -1 it means the file was not found so we exit from the program
	if (cube.id_texture == (unsigned)-1) {
		cerr<<"Image file: texture1.bmp not found"<<endl;
		exit (EXIT_FAILURE);
	}
	GL_CHECK_ERRORS
}

void drawBox(const Matrix4& M) {
	const Matrix4 MVP = M * V * P;

	glUniformMatrix4fv(shader("MVP"), 1, GL_FALSE, &MVP[0]); // MVP correlates to same position in texshader... so dw for now lol

	glBindVertexArray(vaoID);
	glDrawElements(GL_TRIANGLES, sizeof(cube.polygon)/sizeof(cube.polygon[0]), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void drawSkybox() {
	float s = 80;

	Matrix4 M(1);
	scale(M, Vector3(s,s,s));
	rotate(M, rotation(Vector3(1, 0, 0), rotation_x));
	translate(M, Vector3(0,0,-50));

	drawBox(M);
}

void drawFlyingBox() {
	float s = 0.2f;

	Matrix4 M(1);
	scale(M, Vector3(s,s,s));
	rotate(M, rotation(Vector3(0, 0, 1), rotation_z));
	rotate(M, rotation(Vector3(0, 1, 0), rotation_y));
	rotate(M, rotation(Vector3(1, 0, 0), rotation_x));
	translate(M, Vector3(0,0,-50));
	relative_translate(M, Vector3(100,100,100));

	drawBox(M);
}

void drawDefaultBox() {
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
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	GL_CHECK_ERRORS

	shader.Use();

	drawDefaultBox();

	shader.UnUse();

	glutSwapBuffers();
}


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
	glDeleteTextures(1, &cube.id_texture);
	glDeleteBuffers(1, &vboVerticesID);
	glDeleteBuffers(1, &vboTexCoordID);
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

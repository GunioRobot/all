#include "ogl/shader.h"

#include <iostream>

uint Shader::s_id = 0;

Shader::Shader(const char* vsrc, const char* fsrc, const char* gsrc) : vshader(0), fshader(0), gshader(0), id(glCreateProgram()) {
	loadShaders(vsrc, fsrc, gsrc);
}

void Shader::loadShaders(const char* vsrc, const char* fsrc, const char* gsrc) {
	if (vshader) {
		glDetachShader(id, vshader);
		glDeleteShader(vshader);
		GL_CHECK_ERRORS
	}
	if (fshader) {
		glDetachShader(id, fshader);
		glDeleteShader(fshader);
		GL_CHECK_ERRORS
	}
	if (gshader) {
		glDetachShader(id, gshader);
		glDeleteShader(gshader);
		GL_CHECK_ERRORS
	}

	if (vsrc) {
		vshader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vshader, 1, &vsrc, 0);
		glCompileShader(vshader);
		glAttachShader(id, vshader);
		GL_CHECK_ERRORS
	}
	if (fsrc) {
		fshader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fshader, 1, &fsrc, 0);
		glCompileShader(fshader);
		glAttachShader(id, fshader);
		GL_CHECK_ERRORS
	}
	if (gsrc) {
		gshader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(gshader, 1, &gsrc, 0);
		glCompileShader(gshader);
		glAttachShader(id, gshader);
		GL_CHECK_ERRORS
	}

	glBindAttribLocation(id, 0, "in_Point");
	glBindAttribLocation(id, 1, "in_Normal");
	glBindAttribLocation(id, 2, "in_Texcoord");
	GL_CHECK_ERRORS

	glLinkProgram(id);
	glValidateProgram(id);
	GL_CHECK_ERRORS

	int status;
	glGetProgramiv(id, GL_VALIDATE_STATUS, &status);
	if (status == false) {
		const size_t length = 512;
		char buffer[4][length];
		int error = 0;

		glGetShaderInfoLog(vshader, length, &error, buffer[0]);
		glGetShaderInfoLog(fshader, length, &error, buffer[1]);
		glGetShaderInfoLog(gshader, length, &error, buffer[2]);
		glGetProgramInfoLog(id, length, &error, buffer[3]);

		std::cerr << buffer[0] << std::endl << buffer[1] << std::endl << buffer[2] << std::endl << buffer[3] << std::endl;
	}
	GL_CHECK_ERRORS
}

Shader::~Shader() {
	if (vshader) {
		glDetachShader(id, vshader);
		glDeleteShader(vshader);
		GL_CHECK_ERRORS
	}
	if (fshader) {
		glDetachShader(id, fshader);
		glDeleteShader(fshader);
		GL_CHECK_ERRORS
	}
	if (gshader) {
		glDetachShader(id, gshader);
		glDeleteShader(gshader);
		GL_CHECK_ERRORS
	}
	glDeleteProgram(id);
	GL_CHECK_ERRORS
}

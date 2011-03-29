#include <graphics/Shader.hpp>

namespace graphics {
	Shader::Shader(const GLchar* vsrc, const GLchar* fsrc, const GLchar* gsrc) {
		pid = glCreateProgram();

		if (!vsrc) {
			vshader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vshader, 1, &vsrc, 0);
			glCompileShader(vshader);
			glAttachShader(pid, vshader);
		}
		if (!fsrc) {
			fshader = glCre
			ateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fshader, 1, &fsrc, 0);
			glCompileShader(fshader);
			glAttachShader(pid, fshader);
		}
		if (!gsrc) {
			gshader = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(gshader, 1, &gsrc, 0);
			glCompileShader(gshader);
			glAttachShader(pid, gshader);
		}

		glLinkProgram(pid);
		glValidateProgram(pid);

		GLint status;
		glGetProgramiv(pid, GL_VALIDATE_STATUS, &status);
		if (status == GL_FALSE) {
			const GLsizei length = 512;
			GLchar buffer[4][length];
			GLsizei error;

			glGetShaderInfoLog(vshader, length, &error, buffer[0]);
			glGetShaderInfoLog(fshader, length, &error, buffer[1]);
			glGetShaderInfoLog(gshader, length, &error, buffer[2]);
			glGetProgramInfoLog(pid, length, &error, buffer[3]);

			std::cerr << buffer[0] << buffer[1] << buffer[2] << buffer[3];
		}
	}
	Shader::~Shader() {
		if (vshader) {
			glDetachShader(pid, vshader);
			glDeleteShader(vshader);
		}
		if (fshader) {
			glDetachShader(pid, fshader);
			glDeleteShader(fshader);
		}
		if (gshader) {
			glDetachShader(pid, gshader);
			glDeleteShader(gshader);
		}
		glDeleteProgram(pid);
	}

	GLuint Shader::id() const { return pid; }
	void Shader::bind() const { glUseProgram(pid); }
	void Shader::unbind() const { glUseProgram(0); }
};

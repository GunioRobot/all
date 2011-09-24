#include "glw.hpp"
#include "GL3/gl3w.h"

#include <std/io>
#include <std/containers>
#include <std/string>

#include "util.hpp"
/*
class Shader : public noncopyable {
	static uint s_id;

	uint id;
	uint vshader, fshader, gshader;
public:
	Shader(const char* vsrc, const char* fsrc, const char* gsrc = 0) : id(glCreateProgram()), vshader(0), fshader(0), gshader(0) { loadShaders(vsrc, fsrc, gsrc); }
	~Shader() {
		if (id == s_id) unbind();
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

	void loadShaders(const char* vsrc, const char* fsrc, const char* gsrc = 0) {
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

		//glBindAttribLocation(id, 0, "in_Point");
		//glBindAttribLocation(id, 1, "in_Normal");
		//glBindAttribLocation(id, 2, "in_Texcoord");
		GL_CHECK_ERRORS

		glLinkProgram(id);
		glValidateProgram(id);
		GL_CHECK_ERRORS

		int status;
		glGetProgramiv(id, GL_VALIDATE_STATUS, &status);
		//if (status == false) {
			const size_t length = 512;
			char buffer[4][length];

			glGetShaderInfoLog(vshader, length, 0, buffer[0]);
			glGetShaderInfoLog(fshader, length, 0, buffer[1]);
			glGetShaderInfoLog(gshader, length, 0, buffer[2]);
			glGetProgramInfoLog(id, length, 0, buffer[3]);

			fprintf(stderr, "%s\n%s\n%s\n%s\n", buffer[0], buffer[1], buffer[2], buffer[3]);
		//}
		GL_CHECK_ERRORS
	}

	inline void bind() const { if (s_id != id) { glUseProgram(id); s_id = id; GL_CHECK_ERRORS } }
	inline static void unbind() { if (s_id != 0) { glUseProgram(0); s_id = 0; GL_CHECK_ERRORS } }
};
uint Shader::s_id = 0;

static char* load_string_from_file(const char* file_name) {
	File file(file_name, "rb");
	assert(file.open());

	const size_t file_length = file.length();
	assert(file_length != 0);

	char* text = new char[file_length + 1]; // +1 for \0
	file.read(&text[0], 1, file_length);
	text[file_length] = '\0'; // null-terminated

	return text;
}

static void load_shader_files(const char* vs_file_name, const char* fs_file_name, Shader& shader) {
	ScopedArray<char> vs_src(load_string_from_file(vs_file_name));
	ScopedArray<char> fs_src(load_string_from_file(fs_file_name));

	shader.loadShaders(vs_src.ptr(), fs_src.ptr());
}

class OGLDemo {
	uint m_width, m_height;

	Shader* m_flatShader;
public:
	OGLDemo(uint width, uint height) {
		glClearColor(0,0.1f,0.2f,1);
		glClear(GL_COLOR_BUFFER_BIT);

		resize(width, height);

		ScopedArray<char> flat_vs_src(load_string_from_file("shaders\flat\vertex.glsl"));
		ScopedArray<char> flat_fs_src(load_string_from_file("shaders\flat\frag.glsl"));
		m_flatShader = new Shader(flat_vs_src.ptr(), flat_fs_src.ptr());
	}
	~OGLDemo() {
		delete m_flatShader;
	}

	void reload_shaders() {
		load_shader_files("shaders\flat\vertex.glsl", "shaders\flat\frag.glsl", *m_flatShader);
	}
	void resize(uint width, uint height) {
		m_width = width;
		m_height = height;
		glViewport(0,0, width, height);
	}
	void step() {
		glClear(GL_COLOR_BUFFER_BIT);
	}
};
*/
class Application : public glw::Listener {
	glw::Window window;
	//OGLDemo ogldemo;
	bool running;

public:
	Application() :
		window("Final", 1024, 768, false),
		running(true)
	{
		window.setKeyListener(*this);
		window.setMouseListener(*this);
		window.setWindowListener(*this);
	}

	void run() {
		double pt = 0.0;
		double accum = 0.0;
		double render_accum = 0.0, poll_accum = 0.0;
		while (running) {
			const double ct = window.getTime();
			const double dt = ct - pt;
			pt = ct; accum += dt;
			render_accum += dt;
			poll_accum += dt;

			if (accum > 1.0) {
				printf("%i\n", (int) (1.f / dt));
				accum = 0.0;
			}

			if (render_accum > (1.f / 60.f)) {
				render_accum = 0.0;
				//ogldemo.step();
				window.swapBuffers();
			}
			if (poll_accum > (1.f / 30.f)) {
				poll_accum = 0.0;
				window.pollEvents();
			}

			assert(glGetError() == GL_NO_ERROR);
		}
	}
private:
	void onError(int error, const char* message) { printf("err %i %s\n", error, message); }
	void onKey(int key, bool state) {
		printf("ok %i %i\n", key, state);

		if (key == glw::KEY_H && state) {
			static bool hidecursor = false;
			hidecursor = !hidecursor;
			window.hideCursor(hidecursor);
		}
		if (key == glw::KEY_ESCAPE && state) {
			running = false;
		}
	}
	void onMouseClick(int index, bool state) { printf("omc %i %i\n", index, state); }
	void onMouseMove(int x, int y) { printf("omm %i %i\n", x, y); }
	void onMouseScroll(int x, int y) { printf("oms %i %i\n", x, y); }
	void onWindowClose() {
		printf("owc\n");

		running = false;
	}
	void onWindowResize(int new_width, int new_height) {
		printf("owr %i %i\n", new_width, new_height);

		//ogldemo.resize(new_width, new_height);
	}
};

int main(/*int argc, const char* argv[]*/) {
	Application app;
	app.run();

	return 0;
}

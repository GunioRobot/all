#include <ogl3.h>

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <vector>

namespace crucial {
namespace ogl {


	typedef boost::shared_ptr<Shader> Shader_ptr;

	class Driver {
		int mvpMatrixLoc;

		std::vector<Shader_ptr> shaders;
		int shaderLoc, currShader;

		GLfloat width, height, fov, zoom, near, far;
	public:
		Driver(GLfloat width, GLfloat height) : width(width), height(height), fov(60), zoom(1), near(0.1), far(10e3) {
			ClearColour();
			ClearDepth();

			Viewport(width, height);
			EnableDepth();
			Clear();
		}
		void Clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); }
		void ClearColour(GLfloat r = 0.0f, GLfloat g = 0.0f, GLfloat b = 0.0f, GLfloat a = 1.0f) { glClearColor(r,g,b,a); }
		void ClearDepth(GLfloat z = 1.0f) { glClearDepth(z); }

		void EnableDepth() { glEnable(GL_DEPTH_TEST); glDepthMask(GL_TRUE); glDepthFunc(GL_LEQUAL); }
		void DisableDepth() { glDisable(GL_DEPTH_TEST); glDepthMask(GL_FALSE); glDepthFunc(GL_ALWAYS); }

		void Viewport(GLfloat width, GLfloat height) {
			glViewport(0, 0, width, height);
			if (this->width / this->height != width / height) {
				Perspective(fov, zoom, near, far);
			}
			this->width = width;
			this->height = height;
		}
		void Perspective(GLfloat fov, GLfloat zoom, GLfloat near, GLfloat far) {
			projectionMatrix = glm::perspective(fov * zoom, width / height, near, far);
		}
		void ViewMatrix(glm::mat4 viewMatrix) { this->viewMatrix = viewMatrix; }
		void LoadMatrice(glm::mat4 modelMatrix) {
			mvpMatrix = viewMatrix * modelMatrix * projectionMatrix;
			glUniformMatrix4fv(mvpMatrixLoc, 1, GL_FALSE, &mvpMatrix[0][0]);
		}

		void LoadShader(const char* vsrc, const char* fsrc, const char* gsrc) {
			Shader_ptr ashader(new Shader(vsrc, fsrc, gsrc));
			shaders.push_back(ashader);
		}

	/*	void VertexAttribute(shader_id, "inPosition", GL_FLOAT, 3) {
			struct va {
				const char* name;
				GLboolean normalised;
				GLenum type;
				GLuint count;
			}

			struct data {
				void* datapointer;
			}


			//ie
			inVLoc = glGetUniformLocation(va.shader_id, va.name);

			//glVertexAttribPointer(inVLoc, count, type, normalised, sizeof(glVertex), (GLvoid*) (voffset + offsetof(glVertex, colours)));
			//glEnableVertexAttribArray(inVLoc);
		}*/
	};
}
}

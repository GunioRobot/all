#include <crucial/C3DSLoader.h>
#include <crucial/CDevice.h>

#include <crucial/glDriver.h>
#include <crucial/glShader.h>
#include <crucial/glVAO.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <iostream>

int PrevX = 0, PrevY = 0;
float RotX = 0, RotY = 0, RotZ = 0, zoom = 1;

float cam[3];

void OnMouseMove(int mX, int mY, int relX, int relY, int Left, int Right, int Middle) {
	int combo = mX + mY + Middle;
	combo *= 1;
	if(Left) {}
		RotX = (float) -relY;
		RotY = (float) -relX;
	//}
	if (Right) {
		RotZ = (float) relX;
	}
}

void OnKeyDown(SDLKey sym) {
	const float inc = -5;
	switch (sym) {
		case 'w' : { cam[1] += inc; } break;
		case 's' : { cam[1] -= inc; } break;
		case 'a' : { cam[0] -= inc; } break;
		case 'd' : { cam[0] += inc; } break;
		case 'q' : { cam[2] += inc; } break;
		case 'z' : { cam[2] -= inc; } break;
		case 'r' : {} break;
	}
}
void OnWheelUp() { zoom += 1.f; }
void OnWheelDown() { zoom -= 1.f; }

using namespace crucial;
using namespace ogl;

class CRenderMesh {
	CMesh_ptr mesh;
	glVAO_ptr vao;
public:
	CRenderMesh(CMesh_ptr mesh) {
		this->mesh = mesh;
		this->vao.reset(
			new glVAO(
				&(mesh->vertices[0]),
				sizeof(float) * mesh->vertices.size(),
				&(mesh->indices[0]),
				sizeof(short) * mesh->indices.size(),
				mesh->indices.size()
			)
		);
	}
	void draw() {
		vao->draw();
	}
};

typedef boost::shared_ptr<CRenderMesh> CRenderMesh_ptr;

class CModel {
public:
	std::vector<CRenderMesh_ptr> meshes;

	CModel() {}
	~CModel() {}

	void draw() {
		for (unsigned int i = 0; i < meshes.size(); ++i) {
			meshes[i]->draw();
		}
	}
};

CModel load3DSModel(const char* file) {
	CModel amodel;
	C3DSLoader aloader(file);

	for (unsigned int i = 0; i < aloader.meshes.size(); ++i) {
		CRenderMesh_ptr rmesh(
			new CRenderMesh(aloader.meshes[i])
		);
		amodel.meshes.push_back(rmesh);
	}
	return amodel;
}

int main(int argc, char** argv) {
	for (int i = 1; i < argc; ++i)
		std::cout << argv[i] << ' ';

	const int width = 800;
	const int height = 600;

	CDevice device;
	device.createDevice("OpenGL3 Window", width, height);
	glDriver driver(width, height);
	glShader shader("shaders/default.vert","shaders/default.frag");

	CModel aknot = load3DSModel("models/woman.3ds");
	CModel aknot2 = load3DSModel("models/woman.3ds");
	CModel aknot3 = load3DSModel("models/woman.3ds");
	CModel aknot4 = load3DSModel("models/woman.3ds");

	{
		glm::mat4 projectionMatrix; // Store the projection matrix
		glm::mat4 viewMatrix; // Store the view matrix
		glm::mat4 modelMatrix; // Store the model matrix

		glClearColor(0.4f, 0.6f, 0.9f, 0.0f); // Set the clear color based on Microsofts CornflowerBlue (default in XNA)
		projectionMatrix = glm::perspective(60.0f, (float)width / (float)height, 0.1f, 10000.f); // Create our perspective matrix

		modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)); // Create our model matrix

		shader.bind(); // Bind our shader, required for uniform locations
		//GLint projectionMatrixLocation = glGetUniformLocation(shader.id(), "projectionMatrix"); // Get the location of our projection matrix in the shader
		//GLint viewMatrixLocation = glGetUniformLocation(shader.id(), "viewMatrix"); // Get the location of our view matrix in the shader
		GLint modelMatrixLocation = glGetUniformLocation(shader.id(), "modelMatrix"); // Get the location of our model matrix in the shader

		//glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]); // Send our projection matrix to the shader
		//glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader

		glDepthFunc(GL_LESS);
		glCullFace(GL_BACK);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //ENABLE WIREFRAME
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //DISABLE WIREFRAME
		//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); //Points

		//glEnable(GL_MULTISAMPLE); //AA

		glm::mat4 rotationMatrix;

		int frames = 0;

		SDL_Event event;
		unsigned int ticks = SDL_GetTicks();
		while (device.isRunning() && frames < 500000) {
			frames++;
			{
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // Clear required buffers

				// Camera Setup
				viewMatrix = projectionMatrix;
				if (RotX) rotationMatrix *= glm::rotate(RotX, 1.f, 0.f, 0.f); RotX = 0;
				if (RotY) rotationMatrix *= glm::rotate(RotY, 0.f, 1.f, 0.f); RotY = 0;
				if (RotZ) rotationMatrix *= glm::rotate(RotZ, 0.f, 0.f, 1.f); RotZ = 0;
				viewMatrix *= rotationMatrix;

				viewMatrix *= glm::translate(glm::mat4(1.0f), glm::vec3(cam[0],cam[1],cam[2]));
				//glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]); // Send our view matrix to the shader

				//Drawing Model 1
				for (int i = 0; i < 2; ++i) {
					modelMatrix = viewMatrix * glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 50.f * i, 0.f)); // Create our view matrix
					glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our view matrix to the shader
					aknot.draw();
				}

			}
			while (SDL_PollEvent(&event)) {
				switch (event.type) {
					case SDL_QUIT: device.closeDevice();
					case SDL_MOUSEMOTION: {
						OnMouseMove	(event.motion.x,event.motion.y,event.motion.xrel,event.motion.yrel,
							(event.motion.state&SDL_BUTTON(SDL_BUTTON_LEFT)),
							(event.motion.state&SDL_BUTTON(SDL_BUTTON_RIGHT)),
							(event.motion.state&SDL_BUTTON(SDL_BUTTON_MIDDLE)));
					} break;
					case SDL_KEYDOWN: {
						OnKeyDown(event.key.keysym.sym);
					} break;
					case SDL_MOUSEBUTTONDOWN: {
						switch(event.button.button) {
							case SDL_BUTTON_WHEELUP: OnWheelUp(); break;
							case SDL_BUTTON_WHEELDOWN: OnWheelDown(); break;
						} break;
					}
				}
			}
			device.swapBuffers(); // calls glFinish, so do as much as possible prior
		}
		shader.unbind();
				unsigned int ticksafter = SDL_GetTicks();
		std::cout << "frames passed: " << frames << " deltaTicks: " << ticksafter - ticks;
	}



	return 0;
}

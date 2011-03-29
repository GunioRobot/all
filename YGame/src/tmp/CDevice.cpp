#include <CWindow.h>

namespace crucial {
	int CDevice::devices = 0;

	CDevice::CDevice() {
		running = false;
		devices++;
	}
	CDevice::~CDevice() {
		closeDevice();
		devices--;

		if (!devices) SDL_Quit();
	}

	static CDevice* CDevice::createDevice(const char* name, int width, int height, int fsaa, bool vsync, bool fullscreen, int flags, const int* error) {
		if (SDL_Init(SDL_INIT_VIDEO) < 0) { if (error) {*error = 1;} return 0; }

		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, true);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);

		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, fsaa > 0);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, fsaa);

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

		device = new CDevice();
		if (!device) { if (error) {*error = 2;} return 0; }

		device->window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
		if (!device->window) { if (error) {*error = 3;} return 0; }

		SDL_GL_SetSwapInterval(vsync);
		SDL_SetWindowFullscreen(device->window, fullscreen);

		device->context = SDL_GL_CreateContext(device->window);
		if (!device->context) { if (error) {*error = 4;} return 0; }

		device->running = true;

		return device;
	}
	void CDevice::closeDevice() {
		if (context) SDL_GL_DeleteContext(context);
		if (window) SDL_DestroyWindow(window);
		running = false;
	}
	void CDevice::swapBuffers() const { SDL_GL_SwapWindow(window); }
}

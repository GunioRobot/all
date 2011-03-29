#ifndef _C_Device_H_
	#define _C_Device_H_

#include <boost/noncopyable.hpp>
#include <CSDL.h>

namespace crucial {
	class CDevice : boost::noncopyable {
		SDL_WindowID window;
		SDL_GLContext context;
		bool running;

		static int devices;

		CDevice();
	public:
		~CDevice();

		//Error codes | 0 Success | 1 SDL Init Failed | 2 Failed to allocate Device | 3 Failed Creating SDL Window | 4 Failed Creating OpenGL Context
		static CDevice* createDevice(
			const char* name,
			int width = 640,
			int height = 480,
			int fsaa = 0,
			bool vsync = false,
			bool fullscreen = false,
			int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN,
			const int* error = 0
		);
		void closeDevice();

		void swapBuffers() const;
		bool isRunning() const;
	};
}

#endif

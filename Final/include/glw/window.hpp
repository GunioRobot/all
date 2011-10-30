#pragma once

#include <std/types>
#include "listeners.hpp"

namespace glw {
	class Window {
		const char* m_title;

		bool m_windowed;
	public:
		Window(const char* title, uint width, uint height, bool windowed);
		~Window();

		void pollEvents();
		void swapBuffers();

		void setErrorListener(ErrorListener&);
		void setKeyListener(KeyListener&);
		void setMouseListener(MouseListener&);
		void setWindowListener(WindowListener&);

		double getTime() const;

		const char* getTitle() const;
		void getWindowSize(uint& width, uint& height) const;
		bool isWindowed() const;

		void setTitle(const char* title);
		void setWindowSize(uint width, uint height);

		void hideCursor(bool);
	};
}

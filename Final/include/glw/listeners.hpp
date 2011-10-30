#pragma once

namespace glw {
	class ErrorListener {
	public:
		virtual void onError(int, const char*) {}
	};

	class KeyListener {
	public:
		virtual void onKey(int, bool) {} // key, state
	};

	class MouseListener {
	public:
		virtual void onMouseClick(int, bool) {} // index, state
		virtual void onMouseMove(int, int) {} // x, y
		virtual void onMouseScroll(int, int) {} // x, y
	};

	class WindowListener {
	public:
		virtual void onWindowClose() {}
		virtual void onWindowResize(int, int) {} // w, h
	};
}

#include <crucial/IWindowListener.h>

namespace crucial {
	IWindowListener::IWindowListener() {}
	IWindowListener::~IWindowListener() {}
	void IWindowListener::OnEvent(SDL_Event* Event) {
		switch(Event->type) {
			case SDL_QUIT: OnExit(); break;

			//http://wiki.libsdl.org/moin.cgi/SDL_ActiveEvent
			case SDL_ACTIVEEVENT: {
				switch(Event->active.state) {
					case SDL_APPMOUSEFOCUS: if ( Event->active.gain ) {OnMouseFocus();} else {OnMouseBlur();} break;
					case SDL_APPINPUTFOCUS: if ( Event->active.gain ) {OnInputFocus();} else {OnInputBlur();} break;
					case SDL_APPACTIVE: if ( Event->active.gain ) {OnRestore();} else {OnMinimize();} break;
				}
				break;
			}

			case SDL_SYSWMEVENT: break;
			case SDL_VIDEORESIZE: OnResize(Event->resize.w,Event->resize.h); break;
			case SDL_VIDEOEXPOSE: OnExpose(); break;

			default: OnUser(Event->user.type,Event->user.code,Event->user.data1,Event->user.data2);	break;
		}
	}
	void IWindowListener::OnMouseFocus() {}
	void IWindowListener::OnMouseBlur() {}

	void IWindowListener::OnInputFocus() {}
	void IWindowListener::OnInputBlur() {}

	void IWindowListener::OnMinimize() {}
	void IWindowListener::OnRestore() {}
	void IWindowListener::OnResize(int width, int height) {}
	void IWindowListener::OnExpose() {}
	void IWindowListener::OnExit() {}

	void IWindowListener::OnUser(int type, int code, void* data1, void* data2) {}
}

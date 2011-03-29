#include <crucial/IKeyboardListener.h>

namespace crucial {
	IKeyboardListener::IKeyboardListener() {}
	IKeyboardListener::~IKeyboardListener() {}
	void IKeyboardListener::OnEvent(SDL_Event* Event) {
		switch(Event->type) {
			//http://wiki.libsdl.org/moin.cgi/SDL_KeyboardEvent
			case SDL_KEYDOWN: OnKeyDown(Event->key.keysym.sym/*,Event->key.keysym.unicode*/); break;
			case SDL_KEYUP: OnKeyUp(Event->key.keysym.sym/*,event->key.keysym.unicode*/); break;

			//http://wiki.libsdl.org/moin.cgi/SDL_MouseButtonEvent
			case SDL_MOUSEBUTTONDOWN: {
				switch(Event->button.button) {
					case SDL_BUTTON_LEFT: OnLButtonDown(Event->button.x,Event->button.y); break;
					case SDL_BUTTON_RIGHT: OnRButtonDown(Event->button.x,Event->button.y); break;
					case SDL_BUTTON_MIDDLE: OnMButtonDown(Event->button.x,Event->button.y); break;
					case SDL_BUTTON_WHEELUP: OnWheelUp(Event->button.x,Event->button.y); break;
					case SDL_BUTTON_WHEELDOWN: OnWheelDown(Event->button.x,Event->button.y); break;
				} break;
			}
			case SDL_MOUSEBUTTONUP: {
				switch(Event->button.button) {
					case SDL_BUTTON_LEFT: OnLButtonUp(Event->button.x,Event->button.y); break;
					case SDL_BUTTON_RIGHT: OnRButtonUp(Event->button.x,Event->button.y); break;
					case SDL_BUTTON_MIDDLE: OnMButtonUp(Event->button.x,Event->button.y); break;
				} break;
			}

			case SDL_JOYBUTTONDOWN: OnJoyButtonDown(Event->jbutton.which,Event->jbutton.button); break;
			case SDL_JOYBUTTONUP: OnJoyButtonUp(Event->jbutton.which,Event->jbutton.button); break;

			default: OnUser(Event->user.type,Event->user.code,Event->user.data1,Event->user.data2);	break;
		}
	}
	void IKeyboardListener::OnKeyDown(SDLKey sym/*, int unicode*/) {}
	void IKeyboardListener::OnKeyUp(SDLKey sym/*, int unicode*/) {}

	void IKeyboardListener::OnLButtonDown(int mX, int mY) {}
	void IKeyboardListener::OnLButtonUp(int mX, int mY) {}
	void IKeyboardListener::OnRButtonDown(int mX, int mY) {}
	void IKeyboardListener::OnRButtonUp(int mX, int mY) {}
	void IKeyboardListener::OnMButtonDown(int mX, int mY) {}
	void IKeyboardListener::OnMButtonUp(int mX, int mY) {}
	void IKeyboardListener::OnWheelUp(int mX, int mY) {}
	void IKeyboardListener::OnWheelDown(int mX, int mY) {}

	void IKeyboardListener::OnJoyButtonDown(int which, int button) {}
	void IKeyboardListener::OnJoyButtonUp(int which, int button) {}

	void IKeyboardListener::OnUser(int type, int code, void* data1, void* data2) {}
}

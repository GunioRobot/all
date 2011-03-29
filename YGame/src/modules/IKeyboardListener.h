#ifndef _Crucial_Keyboard_Listener_H_
	#define _Crucial_Keyboard_Listener_H_

#include <crucial/IEventReceiver.h>

namespace crucial {
	class IKeyboardListener : public IEventReceiver {
	protected:
		IKeyboardListener();
		~IKeyboardListener();
		void OnEvent(SDL_Event* Event);
		
		virtual void OnKeyDown(SDLKey sym/*, int unicode*/);
		virtual void OnKeyUp(SDLKey sym/*, int unicode*/);

		virtual void OnLButtonDown(int mX, int mY);
		virtual void OnLButtonUp(int mX, int mY);
		virtual void OnRButtonDown(int mX, int mY);
		virtual void OnRButtonUp(int mX, int mY);
		virtual void OnMButtonDown(int mX, int mY);
		virtual void OnMButtonUp(int mX, int mY);
		virtual void OnWheelUp(int mX, int mY);
		virtual void OnWheelDown(int mX, int mY);

		virtual void OnJoyButtonDown(int which, int button);
		virtual void OnJoyButtonUp(int which, int button);

		virtual void OnUser(int type, int code, void* data1, void* data2);
	};
}

#endif

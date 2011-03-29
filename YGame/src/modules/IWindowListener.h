#ifndef _Crucial_Window_Listener_H_
	#define _Crucial_Window_Listener_H_

#include <crucial/IEventReceiver.h>

namespace crucial {
	class IWindowListener : public IEventReceiver {
	protected:
		IWindowListener();
		~IWindowListener();
		void OnEvent(SDL_Event* Event);
		
		virtual void OnMouseFocus();
		virtual void OnMouseBlur();

		virtual void OnInputFocus();
		virtual void OnInputBlur();

		virtual void OnMinimize();
		virtual void OnRestore();
		virtual void OnResize(int width, int height);
		virtual void OnExpose();
		virtual void OnExit();

		virtual void OnUser(int type, int code, void* data1, void* data2);
	};
}

#endif

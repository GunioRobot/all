#ifndef _Crucial_Motion_Listener_H_
	#define _Crucial_Motion_Listener_H_

#include <crucial/IEventReceiver.h>

namespace crucial {
	class IMotionListener : public IEventReceiver {
	protected:
		IMotionListener();
		~IMotionListener();
		void OnEvent(SDL_Event* Event);
		
		virtual void OnMouseMove(int mX, int mY, int relX, int relY, bool Left, bool Right, bool Middle);
	
		virtual void OnJoyAxis(int which, int axis, int value);
		virtual void OnJoyHat(int which, int hat, int value);
		virtual void OnJoyBall(int which, int ball, int xrel, int yrel);
	
		virtual void OnUser(int type, int code, void* data1, void* data2);
	};
}

#endif

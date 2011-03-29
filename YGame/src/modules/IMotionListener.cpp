#include <crucial/IMotionListener.h>

namespace crucial {
	IMotionListener::IMotionListener() {}
	IMotionListener::~IMotionListener() {}
	void IMotionListener::OnEvent(SDL_Event* Event) {
		switch(Event->type) {
			//http://wiki.libsdl.org/moin.cgi/SDL_MouseMotionEvent
			case SDL_MOUSEMOTION: {
				OnMouseMove	(Event->motion.x,Event->motion.y,Event->motion.xrel,Event->motion.yrel,
						(Event->motion.state&SDL_BUTTON(SDL_BUTTON_LEFT)),
						(Event->motion.state&SDL_BUTTON(SDL_BUTTON_RIGHT)),
						(Event->motion.state&SDL_BUTTON(SDL_BUTTON_MIDDLE)));
				break;
			}

			case SDL_JOYAXISMOTION: OnJoyAxis(Event->jaxis.which,Event->jaxis.axis,Event->jaxis.value); break;
			case SDL_JOYBALLMOTION: OnJoyBall(Event->jball.which,Event->jball.ball,Event->jball.xrel,Event->jball.yrel); break;
			case SDL_JOYHATMOTION: OnJoyHat(Event->jhat.which,Event->jhat.hat,Event->jhat.value); break;

			default: OnUser(Event->user.type,Event->user.code,Event->user.data1,Event->user.data2);	break;
		}
	}
	void IMotionListener::OnMouseMove(int mX, int mY, int relX, int relY, bool Left, bool Right, bool Middle) {}

	void IMotionListener::OnJoyAxis(int which, int axis, int value) {}
	void IMotionListener::OnJoyHat(int which, int hat, int value) {}
	void IMotionListener::OnJoyBall(int which, int ball, int xrel, int yrel) {}

	void IMotionListener::OnUser(int type, int code, void* data1, void* data2) {}
}

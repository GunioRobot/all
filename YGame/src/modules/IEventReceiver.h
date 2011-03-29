#ifndef _Crucial_Event_Receiver_H_
	#define _Crucial_Event_Receiver_H_

#include <crucial/SDL.h>
#include <vector>

namespace crucial {
	class IEventReceiver {
		static std::vector<IEventReceiver*> listeners;
	public:
		static void propagate(SDL_Event* Event);
	protected:
		IEventReceiver();
		~IEventReceiver();
		virtual void OnEvent(SDL_Event* Event) =0;
	};
}

#endif

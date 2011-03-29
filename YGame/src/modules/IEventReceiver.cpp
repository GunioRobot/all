#include <crucial/IEventReceiver.h>

namespace crucial {
	std::vector<IEventReceiver*> IEventReceiver::listeners;

	IEventReceiver::IEventReceiver() {
		listeners.push_back(this);
	}
	IEventReceiver::~IEventReceiver() {
		for (std::vector<IEventReceiver*>::iterator it = listeners.begin(); it != listeners.end(); ++it) {
			if (*it == this) {
				listeners.erase(it);
				break;
			}
		}
	}
	void IEventReceiver::propagate(SDL_Event* Event) {
		for (unsigned int i=0; i < listeners.size(); ++i) {
			listeners[i]->OnEvent(Event);
		}
	}
}

class Movable : Component;

class Controller : Component, InputReceiver;
class MovableControllable : Component {
	Movable;
	Controller;
}

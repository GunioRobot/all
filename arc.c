--------------------



namespace physics {
	class World {
		btBroadphaseInterface;
		btDefaultCollisionConfiguration;
		btCollisionDispatcher;
		btSequentialImpulseConstraintSolver;
	}

	class CollisionBody {
		btCollisionShape;

	}

	class RigidBody {
		CollisionBody;


	}
	class SoftBody {
		CollisionBody;

	}

}

--------------------

namespace scene {
	class Component;
	class EComponent {
		Entity;
	}
	class SComponent {
		Scene;
	}

	class Entity {
		list<EComponent>;
	}

	class Scene {
		list<SComponent>;
		list<Entity>;
	}
	class Manager {
		list<Scene>;
	}

	class Node : Component {
		Vector3 Position, Rotation, Velocity;
	}

	class Mesh;
	class Loader {
		Mesh;
	}
	class MeshComponent : Component {
		Node;
		Mesh;
	}

	class Physics : Component {
		Node;
	}
	class Render : Component {
		Node;

		list<VAO>;
		list<Textures>;
		Shader;
	}
	class Movable : Component {
		Node;
	}
	class Controller : Component, InputReceiver;
}

--------------------

class Application {
	Scene::Manager;
}


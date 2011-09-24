namespace ogl {
	struct LightSource {
		Vector3f position;
		Quaternion direction;

		Vector3f ambient, diffuse, specular;
	};
}

/////////////////////////////////

namespace physics {
	typedef btVector3;
	inline btVector3 tobtVector3(const Vector3& v) { return btVector3(v.x, v.y, v.z); }

	struct CollisionShape {};
	struct RigidObject {
		btRigidBody* body;
		btDefaultMotionState motionState;
		const DynamicsWorld& simulation;

		RigidObject(btVector3 position, btQuaternion rotation, btScalar mass, btVector3 inertia, btCollisionShape& shape, DynamicsWorld& simulation) : motionState(btTransform(rotation, position)), simulation(simulation) {
			shape.calculateLocalInertia(mass, inertia);

			body = new btRigidBody(mass, &motionState, &shape, inertia);
			simulation.world->addRigidBody(body);
		}
		~RigidObject() {
			simulation.world->removeRigidBody(body);
		}
	};

	struct Motion {
		Vector3f acceleration, torque; // torque quaternion
	};

	struct DynamicsWorld {
		btBroadphaseInterface* broadphase;
		btDefaultCollisionConfiguration* collisionConfiguration;
		btCollisionDispatcher* dispatcher;
		btSequentialImpulseConstraintSolver* solver;
		btDiscreteDynamicsWorld* world;

		DynamicsWorld() {
			broadphase = new btDbvtBroadphase();
			collisionConfiguration = new btDefaultCollisionConfiguration();
			dispatcher = new btCollisionDispatcher(collisionConfiguration);
			solver = new btSequentialImpulseConstraintSolver;
			world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
		}
		~DynamicsWorld() {
			delete broadphase;
			delete collisionConfiguration;
			delete dispatcher;
			delete solver;
			delete world;
		}
	};
}

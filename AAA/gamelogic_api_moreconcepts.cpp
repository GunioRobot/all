typedef deque,vector,list;

struct Character {
	void step(double dt) {}
};

struct Object {
	void step(double dt) {}
};

struct World {
	deque<Character> characters;
	deque<Object> objects;

	void add(const Character& character) { characters.push_back(character); }
	void add(const Object& object) { objects.push_back(object); }

	void step(double dt) {
		size_t n = characters.size();
		for (size_t i = 0; i < n; ++i) characters[i].step(dt);

		n = objects.size();
		for (size_t i = 0; i < n; ++i) objects[i].step(dt);
	}
};

struct Physics {};

enum UNIFORM_TYPE { UNIFORM_MAT4F }
struct Uniform {
	const char* name;
	enum type;
	void* data;
};

struct RenderDetail {
	VAO* vao;
	Shader* shader;
	Texture* texture;
	vector<Uniform>* uniforms;
};

struct Renderer {
	vector<RenderDetail> details;
	Matrix4f projection;

	void run() {
		size_t n = details.size();
		for(size_t i = 0; i < n; ++i) {
			if (Shader::s_id != shader->id) {
				shader->bind();

				Uniform projection_matrix = {"projection_matrix", UNIFORM_MAT4F, projection};
				bindUniform(projection_matrix);
			}
			size_t m = uniforms.size();
			for (size_t j = 0; j < m; ++j) bindUniform(uniforms[i]);

			vao->bind();
			texture->bind();

			vao->draw();
		}
	}
};

class Simulation {
	//Physics physics;
	Renderer renderer;

	World world;

	void step(double dt) {
		renderer.run();

		//physics.step(dt);
		//world.step(dt);
	}
};

int main() {
	Character w1, e1, e2;
	Object o1, o2;
	Simulation simulation;

	World& world = simulation.world;
	world.add(w1);
	world.add(e1);
	world.add(e2);

	world.add(o1);
	world.add(o2);

	return 0;
}

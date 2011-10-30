#include <vector>

#include <algorithm>
#include <cstdio>

using std::vector;

class Component;

class ComponentManager {
	vector<Component*> components;

	void add(Component* c) { components.push_back(c); }
	void remove(Component* c) { components.erase(std::remove(components.begin(), components.end(), c)); }
public:
	void update();

	~ComponentManager();

	friend class Component;
};

class Component {
	ComponentManager* m_parent;

	virtual void update() {}
public:
	Component() : m_parent(0) {
		printf("Constructor Called for %p\n", (void*)this);
	}
	virtual ~Component() {
		printf("Destructor Called for %p\n", (void*)this);
		if (m_parent) m_parent->remove(this);
	}

	void setParent(ComponentManager* parent) {
		printf("Setting Parent of %p to %p\n", (void*)this, (void*)parent);
		if (m_parent) m_parent->remove(this);
		m_parent = parent;
		if (m_parent) m_parent->add(this);
	}

	friend class ComponentManager;
};

void ComponentManager::update() {
	printf("--Updating\n");
	for (size_t i = 0; i < components.size(); ++i) { components[i]->update(); }
	printf("--Completed\n");
}

ComponentManager::~ComponentManager() {
	printf("--Destructing\n");
	for (size_t i = 0; i < components.size(); ++i) delete components[i];
}

//////////////////////////////////////////////////////////////

class Window : public Component {
	uint w, h;
public:
	Window() {}
	void update() { printf("Window Update\n"); }
};

class Renderer : public Component {
	uint w, h;
	Window* m_window;
public:
	Renderer(Window* window) : m_window(window) {}
	void update() { printf("Renderer Update\n"); }
};

class Physics : public Component {
public:
	Physics() {}
	void update() { printf("Physics Update\n"); }
};

int main() {
	ComponentManager cm;

	Component* w = new Window();
	Component* r = new Renderer((Window*) w);
	Component* p = new Physics();

	w->setParent(&cm);
	r->setParent(&cm);
	p->setParent(&cm);
	cm.update();

	delete w;
	cm.update();

	r->setParent(0);
	cm.update();

	delete r;

	return 0;
}

#pragma once

class lua_State;

namespace lua {
	class Script {
		lua_State* const m_state;
		float m_accum;
	public:
		Script(lua_State* parent);

		bool operator==(const lua_State* state) const { return m_state == state; }
		bool active() const { return m_accum >= 0.f; }

		void sleep(float time) { m_accum = time; }
		void update(float dt);
	};

	class Manager {
		lua_State* const m_state;
	public:
		Manager();

		void queueScript(const string& file_path);
		void registerFunction(const string& name, lua_CFunction fn);
		void update(float dt);
	};
}

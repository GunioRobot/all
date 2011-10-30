#pragma once

#include <lua/lua.hpp>
#include "lua/manager.hpp"
#include <std/types>

static vector<lua::Script> scripts;

////////////////////////

static lua::Script* findScript(const lua_State* state) {
	for (size_t i = 0; i < scripts.size(); ++i) {
		lua::Script& s = scripts[i];
		if (s == state) return &s;
	}
	return (lua::Script*)0;
}

int lua_sleep(lua_State *L) {
	float time = 0;
	int args = lua_gettop(L);
	if(args >= 1) {
		if (lua_isnumber(L, -args)) { time = (float) lua_tonumber(L, -args); }
	}

	lua::Script* s = lua::Manager::getInstance().findScript(L);
	if (s) s->sleep(time);

	return lua_yield(L, 0);
}

////////////////////////

lua::Script::Script(lua_State* parent) : m_state(lua_newthread(parent)), m_accum(0.f) {
	luaL_loadfile(m_state, file_name);
}

void lua::Script::update(float dt) {
	if (m_accum == 0.f) {
		const int status = lua_resume(m_state, 0);
		if (status == 0 || status > LUA_YIELD) m_accum = -1.f;
	} else {
		m_accum -= dt;
		if (m_accum < 0.f) m_accum = 0.f;
	}
}

////////////////////////

lua::Manager::Manager() : m_state(lua_open()) {
	luaL_openlibs(m_state);
}

void lua::Manager::queueScript(const string& file_path) { scripts.push_back(lua::Script(m_state, file_path)); }
void lua::Manager::registerFunction(const string& name, lua_CFunction fn) { lua_register(m_state, name.c_str(), fn); }
void lua::Manager::update(float dt) {
	for (size_t i = 0; i < scripts.size(); ++i) {
		lua::Script& s = scripts[i];
		if (s.active()) s.update(dt);
	}

	scripts.erase(
		std::remove_if(
			scripts.begin(),
			scripts.end(),
			[](const lua::Script& s) {
				return !(s.active());
			}
		),
		scripts.end()
	);
}

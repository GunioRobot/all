#include <lua/lua.hpp>
#include <vector>
#include <cassert>
#include <algorithm>

std::vector<lua::Script> scripts;

bool running = true;
int lua_finish(lua_State *) {
	running = false;
	printf("lua_finish called\n");
	return 0;
}

int main() {
	lua_State* L = lua_open();
	luaL_openlibs(L);

	lua_register(L, "sleep", lua_sleep);
	lua_register(L, "finish", lua_finish);

	lua_queue_script(L, "scripts/init.lua");
	lua_queue_script(L, "scripts/loop.lua");

	float dt = 1.0f;
	while (running) {
		lua_run_frame(dt);
	}

	lua_queue_script(L, "scripts/end.lua");
	lua_run_frame(dt);lua_run_frame(dt);

	lua_close(L);

	return 0;
}

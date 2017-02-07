#include "luaengine.h"
#include "luafunctions.h"
void print_error(lua_State* state) {
  // The error message is on top of the stack.
  // Fetch it, print it and then pop it off the stack.
  const char* message = lua_tostring(state, -1);
  puts(message);
  lua_pop(state, 1);
}
LuaEngine::LuaEngine()
{
	//Create the state
	this->L = luaL_newstate();
	//Load functions
	// Make standard libraries available in the Lua object
	luaL_openlibs(this->L);
	// Load C functions
	lua_register(this->L, "howdy", Luafunc_howdy);
	lua_register(this->L, "getglobal", Luafunc_getglobalint);
	lua_register(this->L, "setglobal", Luafunc_setglobalint);
}
LuaEngine::~LuaEngine()
{
	lua_close(L);
}
int LuaEngine::loadfile(const char* filename)
{
	// Load the program; this supports both source code and bytecode files.
	int result = luaL_loadfile(this->L, filename);

	if ( result != LUA_OK ) {
		print_error(this->L);
		return 1;
	}

	// Finally, execute the program by calling into it.
	// Change the arguments if you're not running vanilla Lua code.

	result = lua_pcall(this->L, 0, LUA_MULTRET, 0);

	if ( result != LUA_OK ) {
		print_error(this->L);
		return 2;
	}
	return result;
}
int LuaEngine::executefunction(const char* funcname)
{
	//After calling global file - call the start function!
	lua_getglobal(this->L, "start");
	lua_pushnumber(this->L, 1);
	lua_pushnumber(this->L, 2);
	lua_call(this->L, 2, 1); // Calling the function!
	int start_result  = (int)lua_tointeger(this->L, -1);
	//Error checks  - just put return LUA_OK at the end of the function
	if ( start_result != LUA_OK ) {
		print_error(this->L);
		return start_result;
	}
	lua_pop(this->L, 1);
	printf("[C] Lua Start function returned: %d\n",start_result); // should be LUA_OK = 0
	return start_result;
}
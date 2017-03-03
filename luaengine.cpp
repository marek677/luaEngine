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
	lua_register(this->L, "executeDelayed", LuaExecute_delayed);
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
// Proof of concept stolen from: https://www.lua.org/pil/25.3.html 
void LuaEngine::preparefunction (const char *func, const char *sig, va_list vl) {
	int narg = 0;  /* number of arguments*/
	lua_getglobal(L, func);  /* get function */
	int intvalue;
	/* push arguments */
	while (*sig) {  /* push arguments */
		switch (*sig++) {

		case 'd':  /* double argument */
			lua_pushnumber(L, va_arg(vl, double));
		break;

		case 'i':  /* int argument */
			intvalue = va_arg(vl, int);
			printf("Pushing int value of: %d\n",intvalue);
			lua_pushnumber(L, intvalue);
		break;

		case 's':  /* string argument */
			lua_pushstring(L, va_arg(vl, char *));
		break;

		case 'l':  /* lightuserdata argument */
			lua_pushlightuserdata(L, va_arg(vl, void *));
		break;

		default:
			printf("Luaengine invalid option (%c)\n", *(sig - 1));
		}
		narg++;
		luaL_checkstack(L, 1, "too many arguments");
	}

	this->narg = narg;
}
int LuaEngine::executefunction()
{
	/* do the call */
	int start_result = lua_pcall(L, narg, 1, 0);
	
	//Error checks  - just put return LUA_OK at the end of the function
	if ( start_result != LUA_OK ) {
		print_error(this->L);
	}
	//lua_pop(this->L, 1);
	lua_settop(this->L,0); //Safety - make sure the stack is 0, in case something went wrong.
	printf("[C] Lua Start function returned: %d\n",start_result); // should be LUA_OK = 0
	return start_result;
}
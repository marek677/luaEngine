//using lua 5.3
/*
Resources:
https://csl.name/post/lua-and-cpp/ - basic thingy
http://gamedevgeek.com/tutorials/calling-lua-functions/ - calling a lua function from C++

*/
//g++ main.cpp -llua53
#ifdef __cplusplus
# include <lua.hpp>
#else
# include <lua.h>
# include <lualib.h>
# include <lauxlib.h>
#endif
#include <cstdio>
#ifdef __cplusplus
extern "C"
#endif
int Luafunc_howdy(lua_State* state)
{
  // The number of function arguments will be on top of the stack.
  int args = lua_gettop(state);

  printf("[C] howdy() was called with %d arguments:\n", args);

  for ( int n=1; n<=args; ++n) {
    printf("\targument %d: '%s'\n", n, lua_tostring(state, n));
  }

  // Push the return value on top of the stack. NOTE: We haven't popped the
  // input arguments to our function. To be honest, I haven't checked if we
  // must, but at least in stack machines like the JVM, the stack will be
  // cleaned between each function call.

  lua_pushnumber(state, 123);

  // Let Lua know how many return values we've passed
  return 1;
}
int Lua_globals[0x1000] = {0}; // have some memory for lua global values.
int Luafunc_getglobalint(lua_State* L)
{
	//getglobal(num)
	int arg1 = lua_tointeger(L,1);
	printf("[C] Returning global number %d value: %d \n",arg1,Lua_globals[arg1]);
	lua_pushnumber(L, Lua_globals[arg1]);
	return 1;
}
int Luafunc_setglobalint(lua_State* L)
{
	//setglobal(num,val)
	int arg1 = lua_tointeger(L, 1);
	int arg2 = lua_tointeger(L, 2);
	printf("[C] Setting global number: %d value: %d\n",arg1,arg2);
	Lua_globals[arg1]=arg2;
	return 0;
}
void print_error(lua_State* state) {
  // The error message is on top of the stack.
  // Fetch it, print it and then pop it off the stack.
  const char* message = lua_tostring(state, -1);
  puts(message);
  lua_pop(state, 1);
}
void execute(const char* filename)
{
  lua_State *state = luaL_newstate();

  // Make standard libraries available in the Lua object
  luaL_openlibs(state);
  // Load C functions
  lua_register(state, "howdy", Luafunc_howdy);
  lua_register(state, "getglobal", Luafunc_getglobalint);
  lua_register(state, "setglobal", Luafunc_setglobalint);

  int result;

  // Load the program; this supports both source code and bytecode files.
  result = luaL_loadfile(state, filename);

  if ( result != LUA_OK ) {
    print_error(state);
    return;
  }

  // Finally, execute the program by calling into it.
  // Change the arguments if you're not running vanilla Lua code.

  result = lua_pcall(state, 0, LUA_MULTRET, 0);

  if ( result != LUA_OK ) {
    print_error(state);
    return;
  }
  
  //After calling global file - call the start function!
  lua_getglobal(state, "start");
  lua_pushnumber(state, 1);
  lua_pushnumber(state, 2);
  lua_call(state, 2, 1); // Calling the function!
  int start_result  = (int)lua_tointeger(state, -1);
  if ( start_result != LUA_OK ) {
    print_error(state);
    return;
  }
  lua_pop(state, 1);
  printf("[C] Lua Start function returned: %d\n",start_result);
  
}

int main(int argc, char** argv)
{
  if ( argc <= 1 ) {
    puts("Usage: runlua file(s)");
    puts("Loads and executes Lua programs.");
    return 1;
  }

  // Execute all programs on the command line
  for ( int n=1; n<argc; ++n ) {
    execute(argv[n]);
  }

  return 0;
}
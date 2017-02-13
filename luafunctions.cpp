#include <cstdio>
#include <windows.h> //Sleep function...
#include "luafunctions.h"
#include "tinythread.h"

using namespace tthread;
int Lua_globals[0x1000] = {0}; // have some memory for lua global values.
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
#ifdef __cplusplus
extern "C"
#endif
int LuaExecute_delayed(lua_State* state)
{
	int delay = lua_tointeger(state, 1);
	char* funcname = (char*) lua_tostring(state, 2);
	int args = lua_gettop(state);
	printf("[C] Calling function:%s with a delay of %d ms\n",funcname,delay);
	lua_getglobal(state, funcname); // get the function!
	for ( int n=3; n<=args; ++n) {
		printf("\targument %d: '%s'\n", n-2, lua_tostring(state, n));
		lua_pushvalue (state, n);
	}
	Sleep(delay);
	lua_call(state, args-2, 0);
	return 0;
}
#ifdef __cplusplus
extern "C"
#endif
int Luafunc_getglobalint(lua_State* L)
{
	//getglobal(num)
	int arg1 = lua_tointeger(L,1);
	printf("[C] Returning global number %d value: %d \n",arg1,Lua_globals[arg1]);
	lua_pushnumber(L, Lua_globals[arg1]);
	return 1;
}

#ifdef __cplusplus
extern "C"
#endif
int Luafunc_setglobalint(lua_State* L)
{
	//setglobal(num,val)
	int arg1 = lua_tointeger(L, 1);
	int arg2 = lua_tointeger(L, 2);
	printf("[C] Setting global number: %d value: %d\n",arg1,arg2);
	Lua_globals[arg1]=arg2;
	return 0;
}
/*
Resources:
https://csl.name/post/lua-and-cpp/ - basic thingy
http://gamedevgeek.com/tutorials/calling-lua-functions/ - calling a lua function from C++
*/

//Compiler: g++ from mingW package
//g++ main.cpp luafunctions.cpp luaengine.cpp -llua53
//using lua 5.3

#ifdef __cplusplus
	#include <lua.hpp>
	#include <cstdio>
#else
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
	#include <stdio.h>
#endif

#include "luaengine.h"

void execute(const char* filename)
{
	LuaEngine* e = new LuaEngine();
	if(e->loadfile(filename)!=LUA_OK)
	{
		printf("Could not load file: %s\n",filename);
		return;
	}
	if(e->executefunction("start")!=LUA_OK)
	{
		printf("Problem executing start function\n");
		return;
	}
	printf("LuaEngine class- everything works okay.");
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
/*
Resources:
https://csl.name/post/lua-and-cpp/ - basic thingy
http://gamedevgeek.com/tutorials/calling-lua-functions/ - calling a lua function from C++
https://www.lua.org/pil/25.3.html - better calling convencion
*/

//Compiler: g++ from mingW package
//g++ main.cpp luafunctions.cpp tinythread.cpp luascheduler.cpp luaengine.cpp -llua53
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
#include <ctime>
#include "luascheduler.h"

using namespace std;

LuaScheduler* s = new LuaScheduler();
void execute(const char* filename)
{
	s->executefunction(1, (char*)filename, (char*)"start", (char*)"is",1,"asdf");
}
int main(int argc, char** argv)
{
  if ( argc <= 1 ) {
    puts("Usage: runlua file(s)");
    puts("Loads and executes Lua programs.");
    return 1;
  }
  clock_t begin = clock();
  
  // Execute all programs on the command line
  for ( int n=1; n<argc; ++n ) {
	for(int i=0;i<200;i++)  
		execute(argv[n]);
	
  }
  clock_t end = clock();
  double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
  printf("\n\nTIME: %f",elapsed_secs);
  return 0;
}
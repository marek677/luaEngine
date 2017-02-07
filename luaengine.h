#pragma once
#ifdef __cplusplus
	#include <lua.hpp>
	#include <cstdio>
#else
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
	#include <stdio.h>
#endif

class LuaEngine
{
	public:
		LuaEngine();
		~LuaEngine();
		int loadfile(const char* filename);
		int executefunction(const char* funcname);
	private:
		lua_State *L;
};
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

#ifdef __cplusplus
extern "C"
#endif
int Luafunc_howdy(lua_State* state);

#ifdef __cplusplus
extern "C"
#endif
int Luafunc_getglobalint(lua_State* L);

#ifdef __cplusplus
extern "C"
#endif
int Luafunc_setglobalint(lua_State* L);

#ifdef __cplusplus
extern "C"
#endif
int LuaExecute_delayed(lua_State* L);


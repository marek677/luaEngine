#pragma once
#include <vector>
#include <string>
#include "luaengine.h"
#include "tinythread.h"

struct EngineInfo
{
	LuaEngine* engine;
	bool used;
};
class LuaScheduler
{
	public:
		LuaScheduler();
		~LuaScheduler();
		void executefunction(bool newthread, char* filename,char *funcname, const char *sig, ...);
	private:
		std::vector <EngineInfo*> engines;
};
#pragma once
#include <vector>
#include <string>
#include "luaengine.h"
#include "tinythread.h"

/* LUASCHEDULER */
struct EngineInfo
{
	LuaEngine* engine;
	bool used;
};
class TriggerScheduler;
class LuaScheduler
{
	public:
		LuaScheduler();
		~LuaScheduler();
		void executefunction(bool newthread, char* filename,char *funcname, const char *sig, va_list args);
		void executefunction(bool newthread, char* filename,char *funcname, const char *sig, ...);
		TriggerScheduler* getTriggerScheduler() {return this->trigger;}
	private:
		std::vector <EngineInfo*> engines;
		TriggerScheduler* trigger;
};
enum trigger_enum
{
    TRIGGER_ONE = 0,
    TRIGGER_TWO = 1,
    TRIGGER_THREE = 2
};
struct TriggerEvent
{
	char* filename;
	char* funcname;
	trigger_enum trigger_id;
};
class TriggerScheduler
{
	public:
		TriggerScheduler(LuaScheduler* luascheduler);
		bool executeTrigger(trigger_enum id,bool newthread,const char *sig, ...);
		void AddTriggerScript(char* filename,char* funcname,trigger_enum id);
		bool RemoveTriggerScript(char* filename,char* funcname, trigger_enum id);
	private:
		std::vector <TriggerEvent*> events;
		LuaScheduler* luascheduler;
};
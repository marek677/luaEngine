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
class TimerScheduler;
class LuaScheduler
{
	public:
		LuaScheduler();
		~LuaScheduler();
		void executefunction(bool newthread, char* filename,char *funcname, const char *sig, va_list args);
		void executefunction(bool newthread, char* filename,char *funcname, const char *sig, ...);
		TriggerScheduler* getTriggerScheduler() {return this->trigger;}
		TimerScheduler* getTimerScheduler() {return this->timer;}
	private:
		std::vector <EngineInfo*> engines;
		TriggerScheduler* trigger;
		TimerScheduler* timer;
};
/*TRIGGER SCHEDULER*/
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
/* TIMER SCHEDULER */
struct TimerEvent
{
	char* filename;
	char* funcname;
	int delay;
	clock_t last;
};
class TimerScheduler
{
	public:
		TimerScheduler(LuaScheduler* luascheduler);
		~TimerScheduler();
		void AddEvent(char* filename, char* funcname,int delay);
		bool RemoveEvent(char* filename, char* funcname,int delay);
		void executeEvents();
		//This is so ugly, havent found a better method to achieve the goal though...
		//So if something is stupid, but works... its not stupid anymore
		bool isExisting(){return this->exists;}
	private:
		bool exists;
		LuaScheduler* luascheduler;
		tthread::thread* TimerThread;
		std::vector <TimerEvent*> events;
};		
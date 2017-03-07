#include <ctime>
#include <cstring>
#include "luascheduler.h"
#include "tinythread.h"

using namespace tthread;

const int ENGINES_PER_CHARACTER =  6;
LuaScheduler::LuaScheduler()
{
	this->trigger = new TriggerScheduler(this);
	this->timer = new TimerScheduler(this);
	EngineInfo* newinfo;
	for(int i=0;i<ENGINES_PER_CHARACTER;i++)
	{
		newinfo = new EngineInfo;
		newinfo->used = 0;
		newinfo->engine = new LuaEngine();
		this->engines.push_back(newinfo);
	}
}
LuaScheduler::~LuaScheduler()
{
	delete this->timer;
	delete this->trigger;
	for(int i=0;i<ENGINES_PER_CHARACTER;i++)
	{
		delete this->engines[i]->engine;
	}
}
void runscript_thread(void* aArg)
{
	EngineInfo* ei = (EngineInfo*) aArg;
	ei->engine->executefunction();
	ei->used = 0;
}
void LuaScheduler::executefunction(bool newthread, char* filename,char *funcname, const char *sig, va_list args)
{
	while(1)
	{
		for(int i=0;i<this->engines.size();i++)
		{
			if(this->engines[i]->used==0)
			{
				this->engines[i]->used=1;
				this->engines[i]->engine->loadfile(filename);
				this->engines[i]->engine->preparefunction(funcname,sig,args);
				thread* a = new thread(runscript_thread,this->engines[i]);
				if(!newthread) //Biggest cheat? Maybe just call it instead...
					a->join();
				return;
			}
		}
		Sleep(0);
	}
}
void LuaScheduler::executefunction(bool newthread, char* filename,char *funcname, const char *sig, ...)
{
	va_list args;
	va_start(args, sig);
    this->executefunction(newthread,filename,funcname,sig,args);
	va_end(args);
}

TriggerScheduler::TriggerScheduler(LuaScheduler* luascheduler)
{
	this->luascheduler = luascheduler;
}

bool TriggerScheduler::executeTrigger(trigger_enum id,bool newthread,const char *sig, ...)
{
	va_list args;
	va_start(args, sig);
	bool ret = false;
	for(int i=0;i<this->events.size();i++)
		if(this->events[i]->trigger_id == id)
		{
			this->luascheduler->executefunction(newthread,this->events[i]->filename,this->events[i]->funcname,sig,args);
			ret = true;
		}
	va_end(args);
	return ret;
}
void TriggerScheduler::AddTriggerScript(char* filename,char* funcname,trigger_enum id)
{
	TriggerEvent* e = new TriggerEvent;
	e->filename = filename;
	e->funcname = funcname;
	e->trigger_id = id;
	this->events.push_back(e);
}
bool TriggerScheduler::RemoveTriggerScript(char* filename,char* funcname, trigger_enum id)
{
	for(int i=0;i<this->events.size();i++)
		if(strstr(this->events[i]->filename,filename) != NULL && strstr(this->events[i]->funcname,funcname) != NULL && this->events[i]->trigger_id==id)
		{
			this->events.erase(this->events.begin() + i);
			return 1;
		}
	return 0;
}
/* TIMER CLASS*/
//Thread!
void Timerscript_thread(void* aArg)
{
	TimerScheduler* ts = (TimerScheduler*) aArg;
	while(ts->isExisting())
	{
		ts->executeEvents();
		Sleep(0);
	}
}
TimerScheduler::TimerScheduler(LuaScheduler* luascheduler)
{
	this->exists = true;
	this->luascheduler = luascheduler;
	this->TimerThread = new thread(Timerscript_thread,this);
}
TimerScheduler::~TimerScheduler()
{
	this->exists = false;
	this->TimerThread->join();	
	delete this->TimerThread;
}
void TimerScheduler::AddEvent(char* filename, char* funcname,int delay)
{
	TimerEvent* newEvent = new TimerEvent;
	newEvent->filename = filename;
	newEvent->funcname = funcname;
	newEvent->delay = delay;
	newEvent->last = 0;
	this->events.push_back(newEvent);
}
bool TimerScheduler::RemoveEvent(char* filename, char* funcname,int delay)
{
	for(int i=0;i<this->events.size();i++)
		if(strstr(this->events[i]->filename,filename)!= NULL && strstr(this->events[i]->funcname,funcname)!= NULL && this->events[i]->delay == delay)
		{
			this->events.erase(this->events.begin()+i);
			return 1;
		}
	return 0;
}
void TimerScheduler::executeEvents()
{
	clock_t now = clock();
	for(int i=0;i<this->events.size();i++)
	{
		//*1000 = result in ms
		if((now-this->events[i]->last)*1000/CLOCKS_PER_SEC>this->events[i]->delay)
		{
			this->events[i]->last = now;
			this->luascheduler->executefunction(1, this->events[i]->filename,this->events[i]->funcname, "i", 2);
		}
	}
	return;
}
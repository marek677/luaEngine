#include <ctime>
#include <cstring>
#include "luascheduler.h"
#include "tinythread.h"

using namespace tthread;

const int ENGINES_PER_CHARACTER =  6;
LuaScheduler::LuaScheduler()
{
	EngineInfo* newinfo;
	for(int i=0;i<ENGINES_PER_CHARACTER;i++)
	{
		newinfo = new EngineInfo;
		newinfo->used = 0;
		newinfo->engine = new LuaEngine();
		this->engines.push_back(newinfo);
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
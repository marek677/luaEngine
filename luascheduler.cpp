#include <ctime>
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

void LuaScheduler::executefunction(bool newthread, char* filename,char *funcname, const char *sig, ...)
{
	va_list args;
	va_start(args, sig);
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
				if(!newthread)
					a->join();
				return;
			}
		}
		Sleep(0);
	}
	va_end(args);
}
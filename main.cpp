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

LuaScheduler* ls = new LuaScheduler();
TriggerScheduler* ts = new TriggerScheduler(ls);
double TestScheduler(bool newthread)
{
	clock_t begin = clock();
	for(int i=0;i<200;i++)  
		ls->executefunction(0, (char*)"script.lua", (char*)"start", (char*)"is",1,"asdf");
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	return elapsed_secs;
}
double TestTrigger()
{
	clock_t begin = clock();
	bool part1 = false;
	bool part2 = false;
	bool part3 = false;
	
	ts->AddTriggerScript((char*)"script.lua", (char*)"start",TRIGGER_ONE);
	ts->AddTriggerScript((char*)"script.lua", (char*)"start",TRIGGER_TWO);
	ts->RemoveTriggerScript((char*)"script.lua", (char*)"start",TRIGGER_TWO);
	
	if(ts->executeTrigger(TRIGGER_ONE,1,(char*)"is",1,"asdf")==true)
		part1 = true;
	if(ts->executeTrigger(TRIGGER_TWO,1,(char*)"is",1,"asdf")==false)
		part2 = true;
	if(ts->executeTrigger(TRIGGER_THREE,1,(char*)"is",1,"asdf")==false)
		part3 = true;
	ts->RemoveTriggerScript((char*)"script.lua", (char*)"start",TRIGGER_ONE);
	
	printf("\nTestTrigger results: %d %d %d\n",part1, part2, part3);
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	return elapsed_secs;
}
int main(int argc, char** argv)
{
  double test1 = TestScheduler(0); //Single Threaded
  double test2 = TestScheduler(1); //Multi Threaded
  double test3 = TestTrigger(); //Trigger Test
  printf("\n\n");
  printf("TEST1 - TIME: %f\n",test1);
  printf("TEST2 - TIME: %f\n",test2);
  printf("TEST3 - TIME: %f\n",test3);
  return 0;
}
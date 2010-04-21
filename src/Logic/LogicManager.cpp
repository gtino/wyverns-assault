#include "..\..\include\Logic\LogicManager.h"

using namespace WyvernsAssault;

// BEGIN SINGLETON
template<> LogicManager* Ogre::Singleton<LogicManager>::ms_Singleton = 0;
LogicManager* LogicManager::getSingletonPtr(void)
{
    return ms_Singleton;
}
LogicManager& LogicManager::getSingleton(void)
{  
    assert( ms_Singleton );  return ( *ms_Singleton );  
}
// END SINGLETON

LogicManager::LogicManager()
{
	//
	// TODO Constructor logic HERE
	//
}

LogicManager::~LogicManager()
{
	//
	// TODO Distructor logic HERE
	//
}

bool LogicManager::initialize()
{
	return true;
}

void LogicManager::finalize()
{
}

bool LogicManager::update(const float elapsedSeconds)
{
	LUA_PROPERTY_SET(LogicManager,ElapsedSeconds,elapsedSeconds);

	float totalSeconds = LUA_PROPERTY_GET(LogicManager,TotalSeconds);

	LUA_PROPERTY_SET(LogicManager,TotalSeconds, totalSeconds + elapsedSeconds);

	return true;
}

// --------------------------------
// Lua Game Lib
// --------------------------------
LUA_BEGIN_BINDING(LogicManager::gamelib)
LUA_BIND("getTotalSeconds", LogicManager::getTotalSeconds)
LUA_BIND("getElapsedSeconds", LogicManager::getElapsedSeconds)
LUA_END_BINDING()

void LogicManager::luaInitialize(lua_State *L)
{
	LuaInterface::luaInitialize(L);

	LUA_PROPERTY_SET(LogicManager,TotalSeconds,0);
	LUA_PROPERTY_SET(LogicManager,ElapsedSeconds,0);
}

//
// Load lua scripts that will be used by this manager
//
void LogicManager::luaLoadScripts()
{	
	return;
}

int LogicManager::getTotalSeconds(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 0

	/* push the total seconds */
	lua_pushnumber(L, LUA_PROPERTY_GET(LogicManager,TotalSeconds));

	/* return the number of results */
	return 1;
}

int LogicManager::getElapsedSeconds(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 0

	/* push the total seconds */
	lua_pushnumber(L, LUA_PROPERTY_GET(LogicManager,ElapsedSeconds));

	/* return the number of results */
	return 1;
}
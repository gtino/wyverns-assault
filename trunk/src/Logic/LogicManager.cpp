#include "..\..\include\Logic\LogicManager.h"

using namespace WyvernsAssault;

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
	LogicManager::smElapsedSeconds = elapsedSeconds;
	LogicManager::smTotalSeconds += elapsedSeconds;

	return true;
}

// --------------------------------
// Lua Game Lib
// --------------------------------
float LogicManager::smElapsedSeconds;
float LogicManager::smTotalSeconds;

const struct luaL_reg LogicManager::gamelib[] = {
		{"getTotalSeconds", LogicManager::getTotalSeconds},
		{"getElapsedSeconds", LogicManager::getElapsedSeconds},
	  {NULL, NULL}
	};

int LogicManager::getTotalSeconds(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 0

	/* push the total seconds */
	lua_pushnumber(L, smTotalSeconds);

	/* return the number of results */
	return 1;
}

int LogicManager::getElapsedSeconds(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 0

	/* push the total seconds */
	lua_pushnumber(L, smElapsedSeconds);

	/* return the number of results */
	return 1;
}
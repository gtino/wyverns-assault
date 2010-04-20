#include "..\..\include\Lua\LuaManager.h"

using namespace WyvernsAssault;



// --------------------------------
// Lua Physics Lib
// --------------------------------
PhysicsManager* LuaManager::smPhysicsManager;

const struct luaL_reg LuaManager::physicslib[] = {
	  {"getHOT", LuaManager::getHOT},
	  {NULL, NULL}
	};

// --------------------------------
// Lua Enemys Lib
// --------------------------------
EnemyManager* LuaManager::smEnemyManager;

const struct luaL_reg LuaManager::enemylib[] = {
	  {"create", LuaManager::createEnemy},
	  {"getPosition", LuaManager::getEnemyPosition},
	  {"setPosition", LuaManager::setEnemyPosition},
	  {"setState", LuaManager::setEnemyState},
	  {"remove", LuaManager::removeEnemy},
	  {NULL, NULL}
	};

// --------------------------------
// Lua Player Lib
// --------------------------------
PlayerManager* LuaManager::smPlayerManager;

const struct luaL_reg LuaManager::playerlib[] = {
	  {"getPosition", LuaManager::getPlayerPosition},
	  {NULL, NULL}
	};

// --------------------------------
// Lua Item Lib
// --------------------------------
ItemManager* LuaManager::smItemManager;

const struct luaL_reg LuaManager::itemlib[] = {
	  {"create", LuaManager::createItem},
	  {"getPosition", LuaManager::getItemPosition},
	  {"setPosition", LuaManager::setItemPosition},
	  {"remove", LuaManager::removeItem},
	  {NULL, NULL}
	};

// --------------------------------
// Lua Audio Lib
// --------------------------------
AudioManager* LuaManager::smAudioManager;

const struct luaL_reg LuaManager::audiolib[] = {
	  {"playSound", LuaManager::playSound},
	  {NULL, NULL}
	};

LuaManager::LuaManager() :
mEnabled(true)
{
}

LuaManager::~LuaManager()
{
	//
	// TODO Distructor logic HERE
	//
}

/** Initialize Lua and load Lua libs */
bool LuaManager::initialize()
{
	/* initialize Lua */
	L = lua_open();

	/* load Lua base libraries */ 
	lua_baselibopen(L);
	lua_mathlibopen(L);
	
	return true;
}

bool LuaManager::addLibrary(const char* name, LuaInterface* luaInterface)
{
	luaInterface->luaInitialize();

	OPEN_LUA_LIBRARY(name, luaInterface->luaGetLibrary());

	return true;
}

bool LuaManager::loadScript(const char* name)
{
	/* run the script */
	int retval = lua_dofile(L, name);

	return (retval == 0);
}

/** Finalize Lua and unload Lua libs */
void LuaManager::finalize()
{
	/* cleanup Lua */
	lua_close(L);
}

/** Reloads scripts */
bool LuaManager::reload()
{
	finalize();
	return initialize();
}

/** Runs all loaded scripts */
bool LuaManager::update(const float elapsedSeconds)
{
	if(!mEnabled)
		return false;

	bool result = runScenario(elapsedSeconds);

	return result;
}

void LuaManager::disable()
{
	mEnabled = false;
}

void LuaManager::enable()
{
	mEnabled = true;
}

bool LuaManager::isEnabled()
{
	return mEnabled;
}

//--------------------------------
// Lua Methods called FROM C++
//--------------------------------
bool LuaManager::runScenario(const float totalSeconds)
{
	///* the function name */
	lua_getglobal(L,"runScenario");
	///* push arguments */
	lua_pushnumber(L, 1);
	///* call the function with 1 argument, return 1 result */
	lua_call(L, 1, 1);
	///* get the result */
	bool result = (int)lua_toboolean(L, -1);
	lua_pop(L, 1);

	return result;
	//return true;
}

int LuaManager::getHOT(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 3

	Ogre::Real x = luaL_checknumber(L, 1);
	Ogre::Real y = luaL_checknumber(L, 2);
	Ogre::Real z = luaL_checknumber(L, 3);

	// TODO :
	//smPhysicsManager->getHOT

	/* push the total seconds */
	lua_pushnumber(L, 0); // 

	/* return the number of results */
	return 1;
}

int LuaManager::createEnemy(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 1, the enemy type

	int type = luaL_checkint(L, 1);

	EnemyPtr enemy = smEnemyManager->createEnemy((EnemyTypes)type);

	lua_pushstring(L,enemy->getName().c_str());

	/* return the number of results */
	return 1;
}

int LuaManager::getEnemyPosition(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 1;
}

int LuaManager::setEnemyPosition(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 1;
}

int LuaManager::setEnemyState(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 1;
}

int LuaManager::removeEnemy(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 1;
}

int LuaManager::getPlayerPosition(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 1;
}

int LuaManager::createItem(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 1;
}

int LuaManager::getItemPosition(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 1;
}

int LuaManager::setItemPosition(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 1;
}

int LuaManager::removeItem(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 1;
}

int LuaManager::playSound(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 1;
}
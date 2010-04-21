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

	// Load all custom libs from registered interfaces
	for(int i=0; i <mLuaInterfaceList.size(); i++)
	{
		LuaInterface* luaInterface = mLuaInterfaceList[i];
		luaInterface->luaInitialize(L);
		LUA_OPEN_LIBRARY(luaInterface->luaGetLibraryName(),luaInterface->luaGetLibrary());
	}
	
	// Load all Lua scripts that the interfaces will use
	for(int i=0; i <mLuaInterfaceList.size(); i++)
	{
		LuaInterface* luaInterface = mLuaInterfaceList[i];
		luaInterface->luaLoadScripts();
	}

	return true;
}

bool LuaManager::registerInterface(LuaInterface* luaInterface)
{
	mLuaInterfaceList.push_back(luaInterface);

	return true;
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

	return true;
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
#include "..\..\include\Lua\LuaManager.h"

using namespace WyvernsAssault;

LuaManager::LuaManager() :
mEnabled(true)
{
}

LuaManager::~LuaManager()
{
	//
	// TODO Distructor logic HERE
	//
	finalize();
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
		LuaInterfacePtr luaInterface = mLuaInterfaceList[i];
		//
		// VERY IMPORTANT! The Lua state is shared between all the interfaces!
		// This si to avoid weird problems with shared static objects if
		// Lua is linked statically. This means that the LuaManager is
		// in charge of open/close Lua. Registered interfaces will just rely
		// on this state!
		//
		luaInterface->luaInitialize(L);
		LUA_OPEN_LIBRARY(luaInterface->luaGetLibraryName(),luaInterface->luaGetLibrary());
	}
	
	// Load all Lua scripts that the interfaces will use
	for(int i=0; i <mLuaInterfaceList.size(); i++)
	{
		LuaInterfacePtr luaInterface = mLuaInterfaceList[i];
		// load scripts
		luaInterface->luaLoadScripts();
		// and enable them
		luaInterface->luaEnable();
	}

	return true;
}

bool LuaManager::registerInterface(LuaInterfacePtr luaInterface)
{
	mLuaInterfaceList.push_back(luaInterface);

	return true;
}

/** Finalize Lua and unload Lua libs */
void LuaManager::finalize()
{
	// Finalize all registered interfaces
	for(int i=0; i <mLuaInterfaceList.size(); i++)
	{
		LuaInterfacePtr luaInterface = mLuaInterfaceList[i];
		luaInterface->luaFinalize();
	}

	/* cleanup Lua */
	lua_close(L);
}

/** Reloads scripts */
bool LuaManager::reload()
{
	//
	// This is not the best way to do that, but it works and is for Debug only...
	//

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

	for(int i=0; i <mLuaInterfaceList.size(); i++)
	{
		mLuaInterfaceList[i]->luaDisable();
	}
}

void LuaManager::enable()
{
	mEnabled = true;

	for(int i=0; i <mLuaInterfaceList.size(); i++)
	{
		mLuaInterfaceList[i]->luaEnable();
	}
}

bool LuaManager::isEnabled()
{
	return mEnabled;
}
#include "..\..\include\Logic\LuaManager.h"

using namespace WyvernsAssault;

// --------------------------------
// Lua Lights Lib
// --------------------------------
LightsManager* LuaManager::smLightsManager;

const struct luaL_reg LuaManager::lightslib[] = {
		{"getLightDiffuseColor", LuaManager::getLightDiffuseColor},
		{"setLightDiffuseColor", LuaManager::setLightDiffuseColor},
		{"getLightPosition", LuaManager::getLightPosition},
		{"setLightPosition", LuaManager::setLightPosition},
		{"getAmbientLight", LuaManager::getAmbientLight},
		{"setAmbientLight", LuaManager::setAmbientLight},
	  {NULL, NULL}
	};

// --------------------------------
// Lua Game Lib
// --------------------------------
float LuaManager::smElapsedSeconds;
float LuaManager::smTotalSeconds;

const struct luaL_reg LuaManager::gamelib[] = {
		{"getTotalSeconds", LuaManager::getTotalSeconds},
		{"getElapsedSeconds", LuaManager::getElapsedSeconds},
	  {NULL, NULL}
	};

LuaManager::LuaManager(LightsManager* lightsManager) :
mEnabled(true)
{
	//
	// TODO Constructor logic HERE
	//
	smLightsManager = lightsManager;

	smTotalSeconds = 0.0f;
	smElapsedSeconds = 0.0f;
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

	// Register out libs
	luaL_openlib(L, "Game", LuaManager::gamelib, 0);
	luaL_openlib(L, "Lights", LuaManager::lightslib, 0);

	/* register our functions */
	//lua_register(L,"getLightDiffuseColor",getLightDiffuseColor);
	//lua_register(L,"getLightPosition",getLightPosition);
	//lua_register(L,"setLightDiffuseColor",setLightDiffuseColor);
	//lua_register(L,"setLightPosition",setLightPosition);
	//lua_register(L,"getTotalSeconds",getTotalSeconds);
	//lua_register(L,"getElapsedSeconds",getElapsedSeconds);

	/* run the script */
	int retval = lua_dofile(L, ".\\data\\scripts\\Scenario.lua");

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
bool LuaManager::run(const float elapsedSeconds)
{
	if(!mEnabled)
		return false;

	smElapsedSeconds = elapsedSeconds;
	smTotalSeconds += elapsedSeconds;

	bool result = runScenario(smTotalSeconds);

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

//--------------------------------
// C++ Methods called FROM Lua
//--------------------------------
int LuaManager::getLightDiffuseColor(lua_State *L)
{
	//
	// Pass it to Lua script
	//
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 1

	/* get the first argument (the light name) */
	Ogre::String lightName = lua_tostring(L, 1);

	//
	// Retrieve light color
	//
	Ogre::Light* light = smLightsManager->getLight(lightName);

	ColourValue color = light->getDiffuseColour();

	/* push the Red */
	lua_pushnumber(L, color.r);
	/* push the Green */
	lua_pushnumber(L, color.g);
	/* push the Blue */
	lua_pushnumber(L, color.b);

	/* return the number of results */
	return 3;
}

int LuaManager::setLightDiffuseColor(lua_State *L)
{
	//
	// Pass it to Lua script
	//
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 4

	/* get the color arguments */
	//if(!lua_isnumber(L,0))
	//{
	//lua_pushstring(L,"Incorrect argument to 'red' component");
	//lua_error(L);
	//}

	Ogre::String lightName = lua_tostring(L, 1);
	Ogre::Real red = lua_tonumber(L, 2);
	Ogre::Real green = lua_tonumber(L, 3);
	Ogre::Real blue = lua_tonumber(L, 4);

	//
	// Retrieve light color
	//
	Ogre::Light* light = smLightsManager->getLight(lightName);

	light->setDiffuseColour(red,green,blue);

	/* return the number of results */
	return 0;
}

int LuaManager::getLightPosition(lua_State *L)
{
	//
	// Pass it to Lua script
	//
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 1

	/* get the first argument (the light name) */
	Ogre::String lightName = lua_tostring(L, 1);

	//
	// Retrieve light color
	//
	Ogre::Light* light = smLightsManager->getLight(lightName);

	Vector3 position = light->getPosition();

	/* push the X */
	lua_pushnumber(L, position.x);
	/* push the Y */
	lua_pushnumber(L, position.y);
	/* push the Z */
	lua_pushnumber(L, position.z);

	/* return the number of results */
	return 3;
}

int LuaManager::setLightPosition(lua_State *L)
{
	//
	// Pass it to Lua script
	//
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 4

	/* get the color arguments */
	//if(!lua_isnumber(L,0))
	//{
	//lua_pushstring(L,"Incorrect argument to 'red' component");
	//lua_error(L);
	//}

	Ogre::String lightName = lua_tostring(L, 1);
	Ogre::Real x = lua_tonumber(L, 2);
	Ogre::Real y = lua_tonumber(L, 3);
	Ogre::Real z = lua_tonumber(L, 4);

	//
	// Retrieve light color
	//
	Ogre::Light* light = smLightsManager->getLight(lightName);

	light->setPosition(x,y,z);

	/* return the number of results */
	return 0;
}

int LuaManager::getAmbientLight(lua_State *L)
{
	//
	// Pass it to Lua script
	//
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 0

	//
	// Retrieve light color
	//
	ColourValue color = smLightsManager->getAmbientLight();

	/* push the Red */
	lua_pushnumber(L, color.r);
	/* push the Green */
	lua_pushnumber(L, color.g);
	/* push the Blue */
	lua_pushnumber(L, color.b);

	/* return the number of results */
	return 3;
}

int LuaManager::setAmbientLight(lua_State *L)
{
	//
	// Pass it to Lua script
	//
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 3

	/* get the color arguments */
	//if(!lua_isnumber(L,0))
	//{
	//lua_pushstring(L,"Incorrect argument to 'red' component");
	//lua_error(L);
	//}

	ColourValue color;

	color.r = lua_tonumber(L, 1);
	color.g = lua_tonumber(L, 2);
	color.b = lua_tonumber(L, 3);

	//
	// Retrieve light color
	//
	smLightsManager->setAmbientLight(color);

	/* return the number of results */
	return 0;
}

int LuaManager::getTotalSeconds(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 0

	/* push the total seconds */
	lua_pushnumber(L, smTotalSeconds);

	/* return the number of results */
	return 1;
}

int LuaManager::getElapsedSeconds(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 0

	/* push the total seconds */
	lua_pushnumber(L, smElapsedSeconds);

	/* return the number of results */
	return 1;
}
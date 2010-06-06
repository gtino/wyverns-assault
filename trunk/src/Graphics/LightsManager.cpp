#include "..\include\Graphics\LightsManager.h"

using namespace WyvernsAssault;

// BEGIN SINGLETON
template<> LightsManager* Ogre::Singleton<LightsManager>::ms_Singleton = 0;
LightsManager* LightsManager::getSingletonPtr(void)
{
    return ms_Singleton;
}
LightsManager& LightsManager::getSingleton(void)
{  
    assert( ms_Singleton );  return ( *ms_Singleton );  
}
// END SINGLETON

LightsManager::LightsManager(SceneManager* sceneManager)
: mLight(0)
{
	this->mSceneManager = sceneManager;
}

LightsManager::~LightsManager()
{
	finalize();
}

/** Initialize the lights manager */
void LightsManager::initialize()
{
}


/** Finalize the lights manager */
void LightsManager::finalize()
{
	if(mLight != NULL)
	{
		mSceneManager->destroyLight(mLight);
		mLight = NULL;
	}
}

void LightsManager::update(const float elapsedSeconds)
{
	this->runDayLight(0);
}

/** Create lights functions **/		
void LightsManager::createLight(String name, Light::LightTypes type, ColourValue diffuse, ColourValue specular, Vector3 position, Vector3 direction)
{
	// HACK!
	/*if(mSceneManager->hasLight(name))
		mLight = mSceneManager->getLight(name);
	else*/
		mLight = mSceneManager->createLight(name);

	mLight->setType(type);
	mLight->setDiffuseColour(diffuse);
	mLight->setSpecularColour(specular);
	if(position != Vector3::ZERO) mLight->setPosition(position);
	if(direction != Vector3::ZERO) mLight->setDirection(direction);
}

/** Get lights functions **/		
Ogre::Light* LightsManager::getLight(String name)
{
	if(mSceneManager->hasLight(name))
		return mSceneManager->getLight(name);
	else
		return NULL;
}

void LightsManager::setAmbientLight(ColourValue color)
{
	mSceneManager->setAmbientLight(color);
}

Ogre::ColourValue LightsManager::getAmbientLight()
{
	return mSceneManager->getAmbientLight();
}

// --------------------------------
// Lua Light Lib
// --------------------------------
LUA_BEGIN_BINDING(LightsManager::lightlib)
LUA_BIND("getLightDiffuseColor", LightsManager::getLightDiffuseColor)
LUA_BIND("setLightDiffuseColor", LightsManager::setLightDiffuseColor)
LUA_BIND("getLightPosition", LightsManager::getLightPosition)
LUA_BIND("setLightPosition", LightsManager::setLightPosition)
LUA_BIND("getAmbientLight", LightsManager::getAmbientLight)
LUA_BIND("setAmbientLight", LightsManager::setAmbientLight)
LUA_END_BINDING()

//
// Load lua scripts that will be used by this manager
//
void LightsManager::luaLoadScripts()
{
	luaLoadScript(".\\data\\scripts\\Scenario.lua");
}

// ----------------------------
// Lua Routines called from C++
// ----------------------------
bool LightsManager::runDayLight(const float totalSeconds)
{
	///* the function name */
	lua_getglobal(L,"runDayLight");
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
int LightsManager::getLightDiffuseColor(lua_State *L)
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
	Ogre::Light* light = LightsManager::getSingleton().getLight(lightName);

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

int LightsManager::setLightDiffuseColor(lua_State *L)
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
	Ogre::Light* light = LightsManager::getSingleton().getLight(lightName);

	light->setDiffuseColour(red,green,blue);

	/* return the number of results */
	return 0;
}

int LightsManager::getLightPosition(lua_State *L)
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
	// Retrieve light position
	//
	Ogre::Light* light = LightsManager::getSingleton().getLight(lightName);

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

int LightsManager::setLightPosition(lua_State *L)
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
	Ogre::Light* light = LightsManager::getSingleton().getLight(lightName);

	light->setPosition(x,y,z);

	/* return the number of results */
	return 0;
}

int LightsManager::getAmbientLight(lua_State *L)
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
	ColourValue color = LightsManager::getSingleton().getAmbientLight();

	/* push the Red */
	lua_pushnumber(L, color.r);
	/* push the Green */
	lua_pushnumber(L, color.g);
	/* push the Blue */
	lua_pushnumber(L, color.b);

	/* return the number of results */
	return 3;
}

int LightsManager::setAmbientLight(lua_State *L)
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
	LightsManager::getSingleton().setAmbientLight(color);

	/* return the number of results */
	return 0;
}

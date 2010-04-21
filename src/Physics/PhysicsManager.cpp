#include "..\..\include\Physics\PhysicsManager.h"

using namespace WyvernsAssault;

// BEGIN SINGLETON
template<> PhysicsManager* Ogre::Singleton<PhysicsManager>::ms_Singleton = 0;
PhysicsManager* PhysicsManager::getSingletonPtr(void)
{
    return ms_Singleton;
}
PhysicsManager& PhysicsManager::getSingleton(void)
{  
    assert( ms_Singleton );  return ( *ms_Singleton );  
}
// END SINGLETON

PhysicsManager::PhysicsManager()
{
	//
	// TODO Constructor logic HERE
	//
}

PhysicsManager::~PhysicsManager()
{
	//
	// TODO Distructor logic HERE
	//
}

bool PhysicsManager::initialize()
{
	//
	// TODO
	//
	return true;
}

void PhysicsManager::finalize()
{
	//
	// TODO
	//
}

// --------------------------------
// Lua Physics Lib
// --------------------------------
LUA_BEGIN_BINDING(PhysicsManager::physicslib)
LUA_BIND("getHOT", PhysicsManager::getHOT)
LUA_END_BINDING()

int PhysicsManager::getHOT(lua_State *L)
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
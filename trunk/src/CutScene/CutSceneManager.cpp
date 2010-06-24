#include "..\include\CutScene\CutSceneManager.h"

using namespace WyvernsAssault;

// BEGIN SINGLETON
template<> CutSceneManager* Ogre::Singleton<CutSceneManager>::ms_Singleton = 0;
CutSceneManager* CutSceneManager::getSingletonPtr(void)
{
    return ms_Singleton;
}
CutSceneManager& CutSceneManager::getSingleton(void)
{  
    assert( ms_Singleton );  return ( *ms_Singleton );  
}
// END SINGLETON

CutSceneManager::CutSceneManager(SceneManager* sceneManager)
: mCutSceneNode(0)
{
	this->mSceneManager = sceneManager;
}

CutSceneManager::~CutSceneManager()
{
	finalize();
}

/** Initialize the lights manager */
void CutSceneManager::initialize()
{
}


/** Finalize the lights manager */
void CutSceneManager::finalize()
{
}

void CutSceneManager::update(const float elapsedSeconds)
{
}

// --------------------------------
// Lua CutScene Lib
// --------------------------------
LUA_BEGIN_BINDING(CutSceneManager, videolib)
LUA_BIND(CutSceneManager, play)
LUA_BIND(CutSceneManager, stop)
LUA_END_BINDING()

//
// Load lua scripts that will be used by this manager
//
LUA_BEGIN_LOAD_SCRIPTS(CutSceneManager)
//
// TODO
//
LUA_END_LOAD_SCRIPTS()

//--------------------------------
// C++ Methods called FROM Lua
//--------------------------------
LUA_DEFINE_FUNCTION(CutSceneManager, play)
{
	//
	// Pass it to Lua script
	//
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 0;
}

LUA_DEFINE_FUNCTION(CutSceneManager, stop)
{
	//
	// Pass it to Lua script
	//
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 0;
}
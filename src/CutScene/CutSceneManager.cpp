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
, mCutSceneId(CutSceneId::Nothing)
, mCurrentStep(0)
, mElapsedSceneTime(0.0f)
, mWaitTimer(0.0f)
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
	//return; // NOTE : Disabled until Lua scripts are not ready

	if(mCutSceneId == CutSceneId::Nothing)
		return;

	// Checks if the current cut scene has ended.
	// If so we will go back to 'nothing'
	bool finished = true;

	switch(mCutSceneId)
	{
	case CutSceneId::Intro:
		finished = playIntroCutScene(elapsedSeconds);
		break;
	case CutSceneId::Wheat:
		//finished = playWheatCutScene(elapsedSeconds);
		break;
	case CutSceneId::Bridge:
		//finished = playBridgeCutScene(elapsedSeconds);
		break;
	case CutSceneId::Forest:
		//finished = playForestCutScene(elapsedSeconds);
		break;
	case CutSceneId::WoodenWall:
		//finished = playWoodenWallCutScene(elapsedSeconds);
		break;
	case CutSceneId::Castle:
		//finished = playCastleCutScene(elapsedSeconds);
		break;
	default:
		finished = true;
		break;
	}

	if(finished)
	{
		reset();
	}
	else
	{
		mElapsedSceneTime += elapsedSeconds;
	}
}

const bool CutSceneManager::wait(const float timeout)
{
	if(timeout <= 0.0f)
		return true;

	if(mWaitTimer <= 0.0f)
	{
		mWaitTimer = mElapsedSceneTime + timeout;
	}

	if(mElapsedSceneTime >= mWaitTimer)
	{
		// Reset the timer!
		mWaitTimer = 0.0f;
		return true;
	}

	return false;
}

void CutSceneManager::reset()
{
	mCutSceneId = CutSceneId::Nothing;
	mElapsedSceneTime = 0.0f;
	mWaitTimer = 0.0f;
	mCurrentStep = 0;
}

// --------------
// Event handlers
// --------------
EVENTS_BEGIN_REGISTER_HANDLERS(CutSceneManager)
	EVENTS_REGISTER_HANDLER(CutSceneManager, GameAreaChanged)
EVENTS_END_REGISTER_HANDLERS()

EVENTS_BEGIN_UNREGISTER_HANDLERS(CutSceneManager)
	EVENTS_UNREGISTER_HANDLER(CutSceneManager, GameAreaChanged)
EVENTS_END_UNREGISTER_HANDLERS()

EVENTS_DEFINE_HANDLER(CutSceneManager, GameAreaChanged)
{
	int previousArea = evt->getPreviousArea();
	int actualArea = evt->getActualArea();

	// We do the cut scene only if we are moving forward
	if( actualArea > previousArea )
	{
		switch(actualArea)
		{
		case 0:
			mCutSceneId = CutSceneId::Intro;
			break;
		default:
			mCutSceneId = CutSceneId::Nothing;
			break;
			// TODO : All other cut scenes!
		}
	}
}

// --------------------------------
// Lua CutScene Lib
// --------------------------------
LUA_BEGIN_BINDING(CutSceneManager, cutscenelib)
LUA_BIND(CutSceneManager, getCurrentStep)
LUA_BIND(CutSceneManager, getElapsedSceneTime)
LUA_BIND(CutSceneManager, wait)
LUA_BIND(CutSceneManager, nextStep)
LUA_BIND(CutSceneManager, reset)
LUA_END_BINDING()

//
// Load lua scripts that will be used by this manager
//
LUA_BEGIN_LOAD_SCRIPTS(CutSceneManager)
LUA_LOAD_SCRIPT(".\\data\\scripts\\CutScene.Intro.lua")
//LUA_LOAD_SCRIPT(".\\data\\scripts\\CutScene.Wheat.lua")
//LUA_LOAD_SCRIPT(".\\data\\scripts\\CutScene.Bridge.lua")
//LUA_LOAD_SCRIPT(".\\data\\scripts\\CutScene.Forest.lua")
//LUA_LOAD_SCRIPT(".\\data\\scripts\\CutScene.WoodenWall.lua")
//LUA_LOAD_SCRIPT(".\\data\\scripts\\CutScene.Castle.lua")
LUA_END_LOAD_SCRIPTS()

//--------------------------------
// C++ Methods called FROM Lua
//--------------------------------
LUA_DEFINE_FUNCTION(CutSceneManager, getCurrentStep)
{
	//
	// Pass it to Lua script
	//
	/* get number of arguments */
	int n = lua_gettop(L);

	int step = CutSceneManager::getSingleton().getCurrentStep();

	lua_pushnumber(L,step);

	/* return the number of results */
	return 1;
}

LUA_DEFINE_FUNCTION(CutSceneManager, getElapsedSceneTime)
{
	//
	// Pass it to Lua script
	//
	/* get number of arguments */
	int n = lua_gettop(L);

	float elapsedSceneTime = CutSceneManager::getSingleton().getElapsedSceneTime();

	lua_pushnumber(L,elapsedSceneTime);

	/* return the number of results */
	return 1;
}

LUA_DEFINE_FUNCTION(CutSceneManager, wait)
{
	//
	// Pass it to Lua script
	//
	/* get number of arguments */
	int n = lua_gettop(L);

	float waitTime = luaL_checknumber(L, 1);

	bool expired = CutSceneManager::getSingleton().wait(waitTime);

	lua_pushboolean(L, expired);

	/* return the number of results */
	return 1;
}

LUA_DEFINE_FUNCTION(CutSceneManager, nextStep)
{
	//
	// Pass it to Lua script
	//
	/* get number of arguments */
	int n = lua_gettop(L);

	int nextStep = CutSceneManager::getSingleton().nextStep();
	
	lua_pushnumber(L,nextStep);

	/* return the number of results */
	return 1;
}

LUA_DEFINE_FUNCTION(CutSceneManager, reset)
{
	//
	// Pass it to Lua script
	//
	/* get number of arguments */
	int n = lua_gettop(L);

	CutSceneManager::getSingleton().reset();

	/* return the number of results */
	return 0;
}

// ----------------------------
// Lua Routines called from C++
// ----------------------------
bool CutSceneManager::playIntroCutScene(const float totalSeconds)
{
	///* the function name */
	lua_getglobal(L,"playIntroCutScene");
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
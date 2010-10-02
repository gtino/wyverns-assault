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
, mCurrentLevel(-1)
, mCutSceneId(CutScene::CutSceneId::Nothing)
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
	mAlreadyPlayed.clear();

	for(LevelToCutScenesMapIterator it = mCutScenesMap.begin(); it != mCutScenesMap.end(); ++it)
	{
		GameAreaToCutSceneIdMapPtr gaMap = it->second;

		gaMap->clear();
	}

	mCutScenesMap.clear();
}

void CutSceneManager::play(int level, CutScene::CutSceneId id)
{
	mCurrentLevel = level;
	mCutSceneId = id;

	CutSceneStartEventPtr evt = CutSceneStartEventPtr(new CutSceneStartEvent(mCutSceneId));
	EVENTS_FIRE(evt);
}

void CutSceneManager::update(const float elapsedSeconds)
{
	//return; // NOTE : Disabled until Lua scripts are not ready

	if(mCutSceneId == CutScene::CutSceneId::Nothing)
		return;

	// Checks if the current cut scene has ended.
	// If so we will go back to 'nothing'
	bool finished = true;

	switch(mCutSceneId)
	{
	case CutScene::CutSceneId::Intro:
		finished = playCutScene(elapsedSeconds, "playIntroCutScene");
		break;
	case CutScene::CutSceneId::FirstKills:
		finished = playCutScene(elapsedSeconds, "playFirstKillsCutScene");
		break;
	case CutScene::CutSceneId::Beer:
		finished = playCutScene(elapsedSeconds, "playBeerCutScene");
		break;
	case CutScene::CutSceneId::Wheat:
		finished = playCutScene(elapsedSeconds, "playWheatCutScene");
		break;
	case CutScene::CutSceneId::Bridge:
		finished = playCutScene(elapsedSeconds, "playBridgeCutScene");
		break;
	case CutScene::CutSceneId::Forest:
		finished = playCutScene(elapsedSeconds, "playForestCutScene");
		break;
	case CutScene::CutSceneId::WoodenWall:
		finished = playCutScene(elapsedSeconds, "playWoodenWallCutScene");
		break;
	case CutScene::CutSceneId::Village:
		finished = playCutScene(elapsedSeconds, "playVillageCutScene");
		break;
	case CutScene::CutSceneId::Siege:
		finished = playCutScene(elapsedSeconds, "playSiegeCutScene");
		break;
	case CutScene::CutSceneId::Castle:
		finished = playCutScene(elapsedSeconds, "playCastleCutScene");
		break;
	case CutScene::CutSceneId::Portal:
		finished = playCutScene(elapsedSeconds, "playPortalCutScene");
		break;
	case CutScene::CutSceneId::Boss:
		finished = playCutScene(elapsedSeconds, "playBossCutScene");
		break;
	case CutScene::CutSceneId::Final:
		finished = playCutScene(elapsedSeconds, "playFinalCutScene");
		break;
	default:
		finished = true;
		break;
	}

	if(finished)
	{
		reset();

		CutSceneEndEventPtr evt = CutSceneEndEventPtr(new CutSceneEndEvent(mCutSceneId));
		EVENTS_FIRE(evt);
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
	mCutSceneId = CutScene::CutSceneId::Nothing;
	mElapsedSceneTime = 0.0f;
	mWaitTimer = 0.0f;
	mCurrentStep = 0;
}

void CutSceneManager::add(int level, int gameArea, int id, const Ogre::String script)
{
	if(mCutScenesMap.find(level) == mCutScenesMap.end())
	{
		//
		// Level not present yet, so create a map for it
		//
		mCutScenesMap[level] = GameAreaToCutSceneIdMapPtr(new GameAreaToCutSceneIdMap());
	}

	GameAreaToCutSceneIdMapPtr gameAreaMap = mCutScenesMap[level];

	if(gameAreaMap->find(gameArea) != gameAreaMap->end())
	{
		// This should never happen, it means that we have already set a script
		// for this area... We are not gonna overwrite it...
		// Assert fals is not the best way but this is rush code :-)
		assert(false);
	}
	else
	{
		// Instantiate a new cut scene
		// assign it to the given game area.
		//
		// HACK! It looks like this is the only way to be able to
		// use the [] access with this map that actually is a shared pointer!
		//
		GameAreaToCutSceneIdMap* m = gameAreaMap.get();
		
		(*m)[gameArea] = CutScenePtr(new CutScene(level,id,script));
	}
}

// --------------
// Event handlers
// --------------
EVENTS_BEGIN_REGISTER_HANDLERS(CutSceneManager)
	EVENTS_REGISTER_HANDLER(CutSceneManager, GameAreaChanged)
	EVENTS_REGISTER_HANDLER(CutSceneManager, GameAreaCleared)
	EVENTS_REGISTER_HANDLER(CutSceneManager, EnemyKilled)
EVENTS_END_REGISTER_HANDLERS()

EVENTS_BEGIN_UNREGISTER_HANDLERS(CutSceneManager)
	EVENTS_UNREGISTER_HANDLER(CutSceneManager, GameAreaChanged)
	EVENTS_UNREGISTER_HANDLER(CutSceneManager, GameAreaCleared)
	EVENTS_UNREGISTER_HANDLER(CutSceneManager, EnemyKilled)
EVENTS_END_UNREGISTER_HANDLERS()

EVENTS_DEFINE_HANDLER(CutSceneManager, GameAreaChanged)
{
	int level = evt->getLevel();
	int previousArea = evt->getPreviousArea();
	int actualArea = evt->getActualArea();

	// We do the cut scene only if we are moving forward
	if( actualArea > previousArea )
	{
		if(level == 0) // Level1_1
		{
			switch(actualArea)
			{
			case 0:
				mCutSceneId = CutScene::CutSceneId::Intro;
				break;
			case 1:
				mCutSceneId = CutScene::CutSceneId::FirstKills;
				break;
			case 2:
				mCutSceneId = CutScene::CutSceneId::Beer;
				break;
			case 4:
				mCutSceneId = CutScene::CutSceneId::Bridge;
				break;
			case 6:
				mCutSceneId = CutScene::CutSceneId::Forest;
				break;
			case 7:
				mCutSceneId = CutScene::CutSceneId::WoodenWall;
				break;
			case 8:
				mCutSceneId = CutScene::CutSceneId::Village;
				break;
			case 9:
				mCutSceneId = CutScene::CutSceneId::Siege;
				break;
			case 10: 
				mCutSceneId = CutScene::CutSceneId::Castle;
				break;
			case 11: 
				mCutSceneId = CutScene::CutSceneId::Portal;
				break;
			default:
				mCutSceneId = CutScene::CutSceneId::Nothing;
				break;
				// TODO : All other cut scenes!
			}
		}
		else if(level == 1) // the Boss level
		{
			switch(actualArea)
			{
				case 2:
					mCutSceneId = CutScene::CutSceneId::Boss;
					break;
				default:
					mCutSceneId = CutScene::CutSceneId::Nothing;
				break;
			}
		}

		CutScenesPlayedMapIterator it = mAlreadyPlayed.find(mCutSceneId);

		if(it == mAlreadyPlayed.end())
		{
			mAlreadyPlayed[mCutSceneId] = true;

			//
			// HACK : to make sure this plays from the very first frame!
			//
			if(mCutSceneId == CutScene::CutSceneId::Intro)
			{
				if(level == 0)
				{
					playCutScene(0,"playIntroCutScene");
				}
				else if(level == 1)
				{
					//playCutScene(0,"playBossCutScene");
				}
			}
		}
		else
		{
			mCutSceneId = CutScene::CutSceneId::Nothing;
		}
	}
}


EVENTS_DEFINE_HANDLER(CutSceneManager, GameAreaCleared)
{
	int level = evt->getLevel();
	int area = evt->getGameArea();

	if(level == 1) // The boss has been destroyed!
	{
		//mCutSceneId = CutSceneId::Final;
	}
}

EVENTS_DEFINE_HANDLER(CutSceneManager, EnemyKilled)
{
	EnemyPtr e = evt->getEnemy();

	Enemy::EnemyTypes type = e->getEnemyType();

	if(type == Enemy::EnemyTypes::Boss)
	{
		mCutSceneId = CutScene::CutSceneId::Final;
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
LUA_BIND(CutSceneManager, add)
LUA_END_BINDING()

//
// Load lua scripts that will be used by this manager
//
LUA_BEGIN_LOAD_SCRIPTS(CutSceneManager)
LUA_LOAD_SCRIPT(".\\data\\scripts\\CutScene.Intro.lua")
LUA_LOAD_SCRIPT(".\\data\\scripts\\CutScene.FirstKills.lua")
LUA_LOAD_SCRIPT(".\\data\\scripts\\CutScene.Beer.lua")
LUA_LOAD_SCRIPT(".\\data\\scripts\\CutScene.Wheat.lua")
LUA_LOAD_SCRIPT(".\\data\\scripts\\CutScene.Bridge.lua")
LUA_LOAD_SCRIPT(".\\data\\scripts\\CutScene.Forest.lua")
LUA_LOAD_SCRIPT(".\\data\\scripts\\CutScene.WoodenWall.lua")
LUA_LOAD_SCRIPT(".\\data\\scripts\\CutScene.Village.lua")
LUA_LOAD_SCRIPT(".\\data\\scripts\\CutScene.Siege.lua")
LUA_LOAD_SCRIPT(".\\data\\scripts\\CutScene.Castle.lua")
LUA_LOAD_SCRIPT(".\\data\\scripts\\CutScene.Portal.lua")
LUA_LOAD_SCRIPT(".\\data\\scripts\\CutScene.Boss.lua")
LUA_LOAD_SCRIPT(".\\data\\scripts\\CutScene.Final.lua")
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

LUA_DEFINE_FUNCTION(CutSceneManager, add)
{
	//
	// Pass it to Lua script
	//
	/* get number of arguments */
	int n = lua_gettop(L);

	//CutSceneManager::getSingleton().add(level,gameArea,cutSceneId,script);

	/* return the number of results */
	return 0;
}

// ----------------------------
// Lua Routines called from C++
// ----------------------------
bool CutSceneManager::playCutScene(const float totalSeconds, Ogre::String cutSceneName)
{
	///* the function name */
	lua_getglobal(L,cutSceneName.c_str());
	///* push arguments */
	lua_pushnumber(L, 1);
	///* call the function with 1 argument, return 1 result */
	lua_call(L, 1, 1);
	///* get the result */
	bool result = (int)lua_toboolean(L, -1);
	lua_pop(L, 1);

	return result;
}
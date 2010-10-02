#include "..\include\Level\LevelManager.h"

using namespace WyvernsAssault;

// BEGIN SINGLETON
template<> LevelManager* Ogre::Singleton<LevelManager>::ms_Singleton = 0;
LevelManager* LevelManager::getSingletonPtr(void)
{
    return ms_Singleton;
}
LevelManager& LevelManager::getSingleton(void)
{  
    assert( ms_Singleton );  return ( *ms_Singleton );  
}
// END SINGLETON

LevelManager::LevelManager()
: mInitialized(false)
, mEnabled(true)
, mCurrentLevelIndex(0)
, mIsLast(false)
{
	//
	// TODO Constructor
	//
}

LevelManager::~LevelManager()
{
	//
	// TODO Destructor
	//
	if(mInitialized)
	{
		finalize();
	}
}

/** Initialize the input manager */
bool LevelManager::initialize( int initialLevel )
{	
	if(!mInitialized)
	{
		mInitialized = true;

		//
		// HACK : we are loading the levels statically. This is not the best option, but
		// we don't have enough time to do something more 'generic', and we have just
		// a couple of levels anyway...
		//
		mLevels[0] = LevelPtr(new Level("Level1_1.scene","Level1_1_cameras.scene","Level1_1_gameAreas.scene"));
		mLevels[1] = LevelPtr(new Level("Boss.scene","Boss_cameras.scene","Boss_gameAreas.scene"));

		mCurrentLevelIndex = initialLevel;
	}

	return mInitialized;
}

void LevelManager::update(const float elapsedSeconds)
{

}

void LevelManager::finalize()
{
	mLevels.clear();

	mInitialized = false;
}

int LevelManager::getCurrentLevelIndex()
{
	return mCurrentLevelIndex;
}

LevelPtr LevelManager::getCurrentLevel()
{
	return mLevels[mCurrentLevelIndex];
}

bool LevelManager::next()
{
	return change(mCurrentLevelIndex+1);
}

bool LevelManager::change(int newLevelIndex)
{
	mCurrentLevelIndex = newLevelIndex;

	mIsLast = (mCurrentLevelIndex == mLevels.size());

	// return 0 if it was the last level
	return mIsLast;
}

// --------------
// Event handlers
// --------------
EVENTS_BEGIN_REGISTER_HANDLERS(LevelManager)
	EVENTS_REGISTER_HANDLER(LevelManager, GameAreaChanged)
	EVENTS_REGISTER_HANDLER(LevelManager, GameAreaCleared)
	EVENTS_REGISTER_HANDLER(LevelManager, CutSceneEnd)
EVENTS_END_REGISTER_HANDLERS()

EVENTS_BEGIN_UNREGISTER_HANDLERS(LevelManager)
	EVENTS_UNREGISTER_HANDLER(LevelManager, GameAreaChanged)
	EVENTS_UNREGISTER_HANDLER(LevelManager, GameAreaCleared)
	EVENTS_UNREGISTER_HANDLER(LevelManager, CutSceneEnd)
EVENTS_END_UNREGISTER_HANDLERS()

EVENTS_DEFINE_HANDLER(LevelManager, GameAreaChanged)
{
	Debug::Out("LevelManager : handleGameAreaChangedEvent");
}

EVENTS_DEFINE_HANDLER(LevelManager, GameAreaCleared)
{
	Debug::Out("LevelManager : handleGameAreaClearedEvent");

	// if it is last area, fire an event of level complete
	if(evt->isLast())
	{
		int previous = mCurrentLevelIndex;

		// Next level. It will return 'true' if this (previous) was the last one!
		bool isLast = next();

		int current = mCurrentLevelIndex;

		LevelCompleteEventPtr e = LevelCompleteEventPtr(new LevelCompleteEvent(previous, current, isLast));
		EVENTS_FIRE(e);
	}
}

EVENTS_DEFINE_HANDLER(LevelManager, CutSceneEnd)
{
	Debug::Out("LevelManager : handleCutSceneEnd");

	CutScene::CutSceneId id = evt->getId();

	if(id == CutScene::CutSceneId::Portal)
	{
		// Level 1 completed
		int previous = mCurrentLevelIndex;

		// Next level. It will return 'true' if this (previous) was the last one!
		bool isLast = next();

		int current = mCurrentLevelIndex;

		LevelCompleteEventPtr e = LevelCompleteEventPtr(new LevelCompleteEvent(previous, current, isLast));
		EVENTS_FIRE(e);
	}
	else if(id == CutScene::CutSceneId::Final)
	{
		// Game completed!
		int previous = mCurrentLevelIndex;

		// Next level. It will return 'true' if this (previous) was the last one!
		bool isLast = next();

		int current = mCurrentLevelIndex;

		LevelCompleteEventPtr e = LevelCompleteEventPtr(new LevelCompleteEvent(previous, current, isLast));
		EVENTS_FIRE(e);
	}
}

// --------------------------------
// Lua Level Lib
// --------------------------------
LUA_BEGIN_BINDING(LevelManager, levellib)
LUA_BIND(LevelManager, change)
LUA_BIND(LevelManager, disable)
LUA_BIND(LevelManager, enable)
LUA_BIND(LevelManager, isEnabled)
LUA_END_BINDING()

//
// Load lua scripts that will be used by this manager
//
LUA_BEGIN_LOAD_SCRIPTS(LevelManager)
// 
// TODO : Load scripts if needed
//
LUA_END_LOAD_SCRIPTS()

LUA_DEFINE_FUNCTION(LevelManager, change)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 1
	int level = luaL_checkint(L, 1);

	LevelManager::getSingleton().change(level);

	/* return the number of results */
	return 0;
}

LUA_DEFINE_FUNCTION(LevelManager, disable)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 0

	LevelManager::getSingleton().disable();

	/* return the number of results */
	return 0;
}

LUA_DEFINE_FUNCTION(LevelManager, enable)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 0

	LevelManager::getSingleton().enable();

	/* return the number of results */
	return 0;
}

LUA_DEFINE_FUNCTION(LevelManager, isEnabled)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 0

	bool isEnabled = LevelManager::getSingleton().isEnabled();

	lua_pushboolean(L, isEnabled);

	/* return the number of results */
	return 1;
}
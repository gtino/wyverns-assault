#include "..\include\Weather\WeatherManager.h"

using namespace WyvernsAssault;

// BEGIN SINGLETON
template<> WeatherManager* Ogre::Singleton<WeatherManager>::ms_Singleton = 0;
WeatherManager* WeatherManager::getSingletonPtr(void)
{
    return ms_Singleton;
}
WeatherManager& WeatherManager::getSingleton(void)
{  
    assert( ms_Singleton );  return ( *ms_Singleton );  
}
// END SINGLETON

WeatherManager::WeatherManager()
: mInitialized(false)
, mEnabled(true)
{
	//
	// TODO Constructor
	//
}

WeatherManager::~WeatherManager()
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
bool WeatherManager::initialize()
{	
	if(!mInitialized)
	{
		mInitialized = true;
	}

	return mInitialized;
}

void WeatherManager::update(const float elapsedSeconds)
{

}

void WeatherManager::finalize()
{
	mInitialized = false;
}

// --------------
// Event handlers
// --------------
EVENTS_BEGIN_REGISTER_HANDLERS(WeatherManager)
	EVENTS_REGISTER_HANDLER(WeatherManager, GameAreaChanged)
	EVENTS_REGISTER_HANDLER(WeatherManager, GameAreaCleared)
EVENTS_END_REGISTER_HANDLERS()

EVENTS_BEGIN_UNREGISTER_HANDLERS(WeatherManager)
	EVENTS_UNREGISTER_HANDLER(WeatherManager, GameAreaChanged)
	EVENTS_UNREGISTER_HANDLER(WeatherManager, GameAreaCleared)
EVENTS_END_UNREGISTER_HANDLERS()

EVENTS_DEFINE_HANDLER(WeatherManager, GameAreaChanged)
{
	Debug::Out("WeatherManager : handleGameAreaChangedEvent");

	int gameArea = evt->getActualArea();
	int level = evt->getLevel();

	if(level == 0) // first level Level1_1
	{
		WeatherTypes weatherType = WeatherTypes::None;

		switch(gameArea)
		{
		case 0:
			weatherType = WeatherTypes::Dawn;
			break;
		case 2:
			weatherType = WeatherTypes::Noon;
			break;
		case 5:
			weatherType = WeatherTypes::Rain;
			break;
		case 8:
			weatherType = WeatherTypes::Sunset;
			break;
		case 10:
			weatherType = WeatherTypes::Night;
			break;
		default:
			break;
		}

		if(weatherType != WeatherTypes::None)
		{
			WeatherChangedEventPtr e = WeatherChangedEventPtr(new WeatherChangedEvent(weatherType));
			EVENTS_FIRE(e);
		}
	}
	else if(level == 1) // Boss level
	{
		switch(gameArea)
		{
		case 0:
			break;
		default:
			break;
		}
	}
}

EVENTS_DEFINE_HANDLER(WeatherManager, GameAreaCleared)
{
	Debug::Out("WeatherManager : handleGameAreaClearedEvent");
}

// --------------------------------
// Lua Level Lib
// --------------------------------
LUA_BEGIN_BINDING(WeatherManager, weatherlib)
LUA_BIND(WeatherManager, disable)
LUA_BIND(WeatherManager, enable)
LUA_BIND(WeatherManager, isEnabled)
LUA_END_BINDING()

//
// Load lua scripts that will be used by this manager
//
LUA_BEGIN_LOAD_SCRIPTS(WeatherManager)
// 
// TODO : Load scripts if needed
//
LUA_END_LOAD_SCRIPTS()

LUA_DEFINE_FUNCTION(WeatherManager, disable)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 0

	WeatherManager::getSingleton().disable();

	/* return the number of results */
	return 0;
}

LUA_DEFINE_FUNCTION(WeatherManager, enable)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 0

	WeatherManager::getSingleton().enable();

	/* return the number of results */
	return 0;
}

LUA_DEFINE_FUNCTION(WeatherManager, isEnabled)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 0

	bool isEnabled = WeatherManager::getSingleton().isEnabled();

	lua_pushboolean(L, isEnabled);

	/* return the number of results */
	return 1;
}
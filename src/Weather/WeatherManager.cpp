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
, mCurrentGameArea(0)
, mCurrentLevel(0)
, mTimer(0.0)
{
	//
	// TODO Constructor
	//
	mRepeat = fmodf(rand(), ( LIGHTNING_MAX_TIME - LIGHTNING_MIN_TIME )) + LIGHTNING_MIN_TIME;
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
	// HACK - making lightning and thunder in game areas 5, 6 and 7 from level 0
	if( mCurrentLevel == 0 )
	{
		if( mCurrentGameArea > 4 && mCurrentGameArea < 8 )
		{
			mTimer = mTimer + elapsedSeconds;

			if( mTimer > mRepeat )
			{
				// Create lightning effect
				SpecialEffectEventPtr evt = SpecialEffectEventPtr(new SpecialEffectEvent(SpecialEffectEvent::EffectType::Lightning, 1.0, 2.0));
				EVENTS_FIRE(evt);
				evt = SpecialEffectEventPtr(new SpecialEffectEvent(SpecialEffectEvent::EffectType::Lightning, 1.0, 1.0));
				EVENTS_FIRE_AFTER(evt, 0.7);

				mRepeat = fmodf(rand(), ( LIGHTNING_MAX_TIME - LIGHTNING_MIN_TIME )) + LIGHTNING_MIN_TIME;
				mTimer = 0.0;
			}
		}
	}
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
	EVENTS_REGISTER_HANDLER(WeatherManager, SpecialEffect)
EVENTS_END_REGISTER_HANDLERS()

EVENTS_BEGIN_UNREGISTER_HANDLERS(WeatherManager)
	EVENTS_UNREGISTER_HANDLER(WeatherManager, GameAreaChanged)
	EVENTS_UNREGISTER_HANDLER(WeatherManager, GameAreaCleared)
	EVENTS_UNREGISTER_HANDLER(WeatherManager, SpecialEffect)
EVENTS_END_UNREGISTER_HANDLERS()

EVENTS_DEFINE_HANDLER(WeatherManager, GameAreaChanged)
{
	Debug::Out("WeatherManager : handleGameAreaChangedEvent");

	mCurrentGameArea = evt->getActualArea();
	mCurrentLevel = evt->getLevel();

	if(mCurrentLevel == 0) // first level Level1_1
	{
		WeatherTypes weatherType = WeatherTypes::None;

		switch(mCurrentGameArea)
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
	else if(mCurrentLevel == 1) // Boss level
	{
		switch(mCurrentGameArea)
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

EVENTS_DEFINE_HANDLER(WeatherManager, SpecialEffect)
{
	Debug::Out("WeatherManager : handleSpecialEffectEvent");


	// Only with lightning effect
	if( evt->getType() == SpecialEffectEvent::EffectType::Lightning )
	{
		WeatherChangedEventPtr e = WeatherChangedEventPtr(new WeatherChangedEvent(WeatherTypes::Lightning));
		EVENTS_FIRE(e);
	}

	// Restore weather
	if(mCurrentLevel == 0)
	{
		WeatherTypes weatherType = WeatherTypes::None;

		switch(mCurrentGameArea)
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
		case 6:
			weatherType = WeatherTypes::Rain;
			break;
		case 7:
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
			EVENTS_FIRE_AFTER(e, 0.4);
		}
	}
	else if(mCurrentLevel == 1) // Boss level
	{
		switch(mCurrentGameArea)
		{
		case 0:
			break;
		default:
			break;
		}
	}
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
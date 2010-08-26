#include "..\include\GameArea\GameAreaManager.h"

using namespace WyvernsAssault;

// BEGIN SINGLETON
template<> GameAreaManager* Ogre::Singleton<GameAreaManager>::ms_Singleton = 0;
GameAreaManager* GameAreaManager::getSingletonPtr(void)
{
    return ms_Singleton;
}
GameAreaManager& GameAreaManager::getSingleton(void)
{  
    assert( ms_Singleton );  return ( *ms_Singleton );  
}
// END SINGLETON

GameAreaManager::GameAreaManager()
: mInitialized(false)
, mIsDebugEnabled(false)
, mCurrentGameArea(-1)
, mGameAreaCleared(true)
{

}

GameAreaManager::~GameAreaManager()
{
	if(mInitialized)
	{
		//
		// Destroy all the objects (destructors will take care of deleting them)
		//
		mGameAreas.clear();
		mInitialized = false;
	}
}

void GameAreaManager::initialize()
{
	// Now it is initialized!
	mInitialized = true;
}

void GameAreaManager::load(Ogre::String file)
{
	assert(mInitialized);
}

void GameAreaManager::update(Vector3 playerPosition, const float elpasedSeconds)
{
	int playerArea = positionGameArea(playerPosition);

	// Game Area changed and game are is cleared raise event
	if( mCurrentGameArea != playerArea )
	{
		GameAreaChangedEventPtr evt = GameAreaChangedEventPtr(new GameAreaChangedEvent(mCurrentGameArea, playerArea));
		EVENTS_FIRE(evt);

		mCurrentGameArea = playerArea;
		mGameAreaCleared = false;
	}	
}

// Get game area from a position
int GameAreaManager::positionGameArea(Vector3 position)
{
	// Look for player's current game area
	for(int areaId = 0; areaId < mGameAreas.size(); areaId++)
	{
		// Define a polygon (in correct order) for the game area
		Ogre::Polygon area;
		area.insertVertex( mGameAreas[areaId].mBeginNear);
		area.insertVertex( mGameAreas[areaId].mEndNear);
		area.insertVertex( mGameAreas[areaId].mEndFar);
		area.insertVertex( mGameAreas[areaId].mBeginFar);

		// Check if player position (without height) is inside this game area
		if(area.isPointInside(position * Vector3(1,0,1)))
		{	
			return areaId;
		}
	}

	// Return last game area
	return mCurrentGameArea;
}

/** Add game area to vector */
void GameAreaManager::addGameArea(Vector3 beginNear, Vector3 endNear, Vector3 beginFar, Vector3 endFar)
{
	GameArea area;
	area.mBeginNear = beginNear;
	area.mEndNear = endNear;
	area.mBeginFar = beginFar;
	area.mEndFar = endFar;
	mGameAreas.push_back(area);
}


// --------------
// Event handlers
// --------------
EVENTS_BEGIN_REGISTER_HANDLERS(GameAreaManager)
	EVENTS_REGISTER_HANDLER(GameAreaManager, GameAreaCleared)
EVENTS_END_REGISTER_HANDLERS()

EVENTS_BEGIN_UNREGISTER_HANDLERS(GameAreaManager)
	EVENTS_UNREGISTER_HANDLER(GameAreaManager, GameAreaCleared)
EVENTS_END_UNREGISTER_HANDLERS()

EVENTS_DEFINE_HANDLER(GameAreaManager, GameAreaCleared)
{
	Debug::Out("GameAreaManager : handleGameAreaClearedEvent");

	if( mCurrentGameArea == evt->getGameArea() )
		mGameAreaCleared = true;
}

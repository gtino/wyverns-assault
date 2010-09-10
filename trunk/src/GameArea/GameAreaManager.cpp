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
, mCurrentLevel(CURRENT_LEVEL)
, mGameAreaCleared(true)
, mEnemiesAlive(false)
, mTime(0.0)
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

void GameAreaManager::update(Vector3 playerPosition, const float elapsedSeconds)
{
	int playerArea = positionGameArea(playerPosition);
	bool isLast = (playerArea == getLastGameArea());

	// Game Area changed and game area is cleared raise event
	if( mCurrentGameArea != playerArea && mGameAreaCleared )
	{
		GameAreaChangedEventPtr evt = GameAreaChangedEventPtr(new GameAreaChangedEvent(mCurrentLevel, mCurrentGameArea, playerArea, isLast));
		EVENTS_FIRE(evt);

		mCurrentGameArea = playerArea;
		mGameAreaCleared = false;
		mEnemiesAlive = (mGameAreas[playerArea].mEnemies != 0);
		mTime = 0.0;

		// Call manage area
		manageGameArea();
	}

	mTime += elapsedSeconds;

	// Game Area cleared by time
	if( mGameAreas[mCurrentGameArea].mFinishTime != 0 && !mGameAreaCleared)
	{
		if( mGameAreas[mCurrentGameArea].mFinishTime < mTime )
		{
			mGameAreaCleared = true;
			GameAreaClearedEventPtr evt = GameAreaClearedEventPtr(new GameAreaClearedEvent(mCurrentLevel, mCurrentGameArea, mGameAreas[mCurrentGameArea].mType, isLast));
			EVENTS_FIRE(evt);
		}
	}
	// Cleared by killing enemies
	else if( !mEnemiesAlive && !mGameAreaCleared)
	{	
		mGameAreaCleared = true;
		GameAreaClearedEventPtr evt = GameAreaClearedEventPtr(new GameAreaClearedEvent(mCurrentLevel, mCurrentGameArea, mGameAreas[mCurrentGameArea].mType, isLast));
		EVENTS_FIRE(evt);
	}
}

// Gets the last game area 
int GameAreaManager::getLastGameArea()
{
	return mGameAreas.size()-1;
}

// Manage game area enemy creation events
void GameAreaManager::manageGameArea()
{
	// Area cleared by time
	if( mGameAreas[mCurrentGameArea].mFinishTime > 0.0 )
	{
		// Time mini-game
		if(mGameAreas[mCurrentGameArea].mType == 1)
		{
			for(int i = 0; i < mGameAreas[mCurrentGameArea].mFinishTime * 2; i++ )
			{
				Enemy::EnemyTypes type;
				int x = rand() % 3;

				if( x == 0 )
					type = Enemy::EnemyTypes::Naked;
				else if( x == 1 )
					type = Enemy::EnemyTypes::Cow;
				else
					type = Enemy::EnemyTypes::Chicken;

				EnemyCreationEventPtr evt = EnemyCreationEventPtr(new EnemyCreationEvent(type, mGameAreas[mCurrentGameArea].mDifficult, getSpawnPoint(mCurrentGameArea), mCurrentGameArea));
				float time = i / 2;
				EVENTS_FIRE_AFTER(evt, time);
			}
		}
	}
	// Area cleared killing enemies
	else
	{
		// Only knights spwan
		if(mGameAreas[mCurrentGameArea].mType == 0)
		{
			Enemy::EnemyTypes type = Enemy::EnemyTypes::Knight;

			for(int i = 0; i < mGameAreas[mCurrentGameArea].mEnemies; i++ )
			{
				EnemyCreationEventPtr evt = EnemyCreationEventPtr(new EnemyCreationEvent(type, mGameAreas[mCurrentGameArea].mDifficult, getSpawnPoint(mCurrentGameArea), mCurrentGameArea));
				EVENTS_FIRE_AFTER(evt, i+5);
			}
		}
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
		area.insertVertex( mGameAreas[areaId].mBeginNear * Vector3(1,0,1) );
		area.insertVertex( mGameAreas[areaId].mEndNear * Vector3(1,0,1) );
		area.insertVertex( mGameAreas[areaId].mEndFar * Vector3(1,0,1) );
		area.insertVertex( mGameAreas[areaId].mBeginFar * Vector3(1,0,1) );

		// Check if player position (without height) is inside this game area
		if(area.isPointInside(position * Vector3(1,0,1)))
		{	
			return areaId;
		}
	}

	// Return last game area
	return mCurrentGameArea;
}

// Get a point from game area boundaries
Vector3 GameAreaManager::getSpawnPoint(int gameArea)
{
	int point = rand() % mGameAreas[gameArea].mSpawnPoints.size();

	return mGameAreas[gameArea].mSpawnPoints[point];
}


/** Add game area to vector */
void GameAreaManager::addGameArea(GameArea area)
{
	mGameAreas.push_back(area);
}


// --------------
// Event handlers
// --------------
EVENTS_BEGIN_REGISTER_HANDLERS(GameAreaManager)
	EVENTS_REGISTER_HANDLER(GameAreaManager, GameAreaEnemiesDeath)
EVENTS_END_REGISTER_HANDLERS()

EVENTS_BEGIN_UNREGISTER_HANDLERS(GameAreaManager)
	EVENTS_UNREGISTER_HANDLER(GameAreaManager, GameAreaEnemiesDeath)
EVENTS_END_UNREGISTER_HANDLERS()

EVENTS_DEFINE_HANDLER(GameAreaManager, GameAreaEnemiesDeath)
{
	Debug::Out("GameAreaManager : handleGameAreaEnemiesDeathEvent");

	if( mCurrentGameArea == evt->getGameArea() )
		mEnemiesAlive = false;
}

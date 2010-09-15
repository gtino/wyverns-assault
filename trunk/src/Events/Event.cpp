#include "..\include\Events\Event.h"

using namespace WyvernsAssault;

Event::Event(EventTypes type, EventPriorities priority)
: mData(0)
, mTimer(0.0f)
{
	//
	// TODO Constructor
	//
	this->mType = type;
	this->mPriority = priority;
}

Event::~Event()
{
	//
	// TODO Destructor
	//
}

/** Returns the event type */
EventTypes Event::getType()
{
	return mType;
}

/** Returns the event priority */
EventPriorities Event::getPriority()
{
	return mPriority;
}

/** Returns the event data */
EventData Event::getData()
{
	return mData;
}

/** Sets the event data */
void Event::setData(EventData data)
{
	mData = data;
}

void Event::setTimer(const float timoutInSeconds)
{
	mTimer = timoutInSeconds;
}

void Event::updateTimer(const float elapsedSeconds)
{
	mTimer -= elapsedSeconds;

	if(mTimer < 0.0f) mTimer = 0.0f;
}

bool Event::hasExpired()
{
	return (mTimer <= 0.0f);
}

// -------------------------------
bool EventComparator::operator() (EventPtr& evt1, EventPtr& evt2)
{
	if (!evt1.get()) return true;
	if (!evt2.get()) return false;
	// The more or less standard convention for a priority queue
	// assigns more priority to values with lower numbers.
	// However, the STL implementation does the opposite ¬_¬.
	// To stick with the more familiar convention, the comparison operator
	// used for this method is > instead of <, so that the queue ordering 
	// gets reversed.
	return evt1->getPriority()>evt2->getPriority();
}

//----------------------

CollisionEvent::CollisionEvent()
:Event(EventTypes::Collision,EventPriorities::Normal)
{
	
};

//----------------------

EnemyAttackEvent::EnemyAttackEvent(EnemyPtr e)
:Event(EventTypes::EnemyAttack, EventPriorities::Normal)
{
	mEnemy = e;
};

//----------------------

EnemyHitEvent::EnemyHitEvent(EnemyPtr e, PlayerPtr p)
: Event(EventTypes::EnemyHit, EventPriorities::Normal)
, mDamage(0)
{
	mEnemy = e;
	mPlayer = p;
};

//----------------------

EnemyKilledEvent::EnemyKilledEvent(EnemyPtr e, PlayerPtr p)
:Event(EventTypes::EnemyKilled, EventPriorities::Normal)
{
	mEnemy = e;
	mPlayer = p;
};

//----------------------

EnemyRemoveEvent::EnemyRemoveEvent(EnemyPtr e)
:Event(EventTypes::EnemyRemove, EventPriorities::Normal)
{
	mEnemy = e;
};

//----------------------

EnemyCustomEvent::EnemyCustomEvent(EnemyPtr e)
:Event(EventTypes::EnemyCustom, EventPriorities::Normal)
{
	mEnemy = e;
};

//----------------------

EnemyCreationEvent::EnemyCreationEvent(Enemy::EnemyTypes type, int difficult, Vector3 position, int gameArea)
:Event(EventTypes::EnemyCreation, EventPriorities::Normal)
{
	mType = type;
	mDifficult = difficult;
	mPosition = position;
	mGameArea = gameArea;
};

//----------------------

EnemyPhysicsEvent::EnemyPhysicsEvent(EnemyPtr e, int gameArea)
:Event(EventTypes::EnemyPhysics, EventPriorities::Normal)
{
	mEnemy = e;
	mGameArea = gameArea;
};

//----------------------

EnemyCreateItemEvent::EnemyCreateItemEvent(EnemyPtr e, int gameArea)
:Event(EventTypes::EnemyCreateItem, EventPriorities::Normal)
{
	mEnemy = e;
	mGameArea = gameArea;
};

//----------------------

PlayerHitEvent::PlayerHitEvent(EnemyPtr e, PlayerPtr p)
:Event(EventTypes::PlayerHit, EventPriorities::Normal)
{
	mPlayer = p;
	mEnemy = e;	
}

//----------------------

PlayerKilledEvent::PlayerKilledEvent(PlayerPtr p)
:Event(EventTypes::PlayerKilled, EventPriorities::Normal)
{
	mPlayer = p;
}

//----------------------

PlayerAttackEvent::PlayerAttackEvent(PlayerPtr p)
:Event(EventTypes::PlayerAttack, EventPriorities::Normal)
{
	mPlayer = p;
};

//----------------------

PlayerAttackSpecialEvent::PlayerAttackSpecialEvent(PlayerPtr p)
:Event(EventTypes::PlayerAttackSpecial, EventPriorities::Normal)
{
	mPlayer = p;
};

//----------------------

PlayerStatusUpdateEvent::PlayerStatusUpdateEvent(PlayerPtr p)
:Event(EventTypes::PlayerStatusUpdate, EventPriorities::Normal)
{
	mPlayer = p;
};

//----------------------

ItemCreationEvent::ItemCreationEvent(ItemPtr i)
:Event(EventTypes::ItemCreation, EventPriorities::Normal)
{
	mItem = i;
};

//----------------------

ItemCatchEvent::ItemCatchEvent(PlayerPtr p, ItemPtr i)
:Event(EventTypes::ItemCatch, EventPriorities::Normal)
{
	mPlayer = p;
	mItem = i;
};

//----------------------

ItemRemoveEvent::ItemRemoveEvent(ItemPtr i)
:Event(EventTypes::ItemRemove, EventPriorities::Normal)
{
	mItem = i;
};

//----------------------

ItemPhysicsEvent::ItemPhysicsEvent(ItemPtr i, int gameArea)
:Event(EventTypes::ItemPhysics, EventPriorities::Normal)
{
	mItem = i;
	mGameArea = gameArea;
};

//----------------------

ObjectHitEvent::ObjectHitEvent(ObjectPtr o, PlayerPtr p)
: Event(EventTypes::ObjectHit, EventPriorities::Normal)
, mDamage(0)
{
	mObject = o;
	mPlayer = p;
};

//----------------------

ObjectKilledEvent::ObjectKilledEvent(ObjectPtr o, PlayerPtr p)
:Event(EventTypes::ObjectKilled, EventPriorities::Normal)
{
	mObject = o;
	mPlayer = p;
};

//----------------------

ObjectRemoveEvent::ObjectRemoveEvent(ObjectPtr o)
:Event(EventTypes::ObjectRemove, EventPriorities::Normal)
{
	mObject = o;
};

//----------------------

ObjectCustomEvent::ObjectCustomEvent(ObjectPtr o)
:Event(EventTypes::ObjectCustom, EventPriorities::Normal)
{
	mObject = o;
};

//----------------------

GameAreaChangedEvent::GameAreaChangedEvent(int level, int previousArea, int actualArea, bool isLast)
:Event(EventTypes::GameAreaChanged, EventPriorities::High)
{
	mLevel = level;
	mPreviousArea = previousArea;
	mActualArea = actualArea;
	mIsLast = isLast;
};

//----------------------

GameAreaClearedEvent::GameAreaClearedEvent(int level, int gameArea, int type, bool isLast)
:Event(EventTypes::GameAreaCleared, EventPriorities::Normal)
{
	mLevel = level;
	mGameArea = gameArea;
	mType = type;
	mIsLast = isLast;
};

//----------------------

GameAreaEnemiesDeathEvent::GameAreaEnemiesDeathEvent(int level, int gameArea)
:Event(EventTypes::GameAreaEnemiesDeath, EventPriorities::Normal)
{
	mLevel = level;
	mGameArea = gameArea;
};

//----------------------

ProjectileFireEvent::ProjectileFireEvent(EnemyPtr e)
:Event(EventTypes::ProjectileFire, EventPriorities::Normal)
{
	mEnemy = e;	
}

//----------------------

ProjectileUpdateEvent::ProjectileUpdateEvent(ProjectilePtr p, Enemy::EnemyTypes type)
:Event(EventTypes::ProjectileUpdate, EventPriorities::Normal)
{
	mProjectile = p;
	mType = type;
}

//----------------------

ProjectileRemoveEvent::ProjectileRemoveEvent(ProjectilePtr p)
:Event(EventTypes::ProjectileRemove, EventPriorities::Normal)
{
	mProjectile = p;
}

//----------------------

ProjectileHitEvent::ProjectileHitEvent(ProjectilePtr p, PlayerPtr player)
:Event(EventTypes::ProjectileHit, EventPriorities::Normal)
{
	mProjectile = p;	
	mPlayer = player;
}

//----------------------

LevelCompleteEvent::LevelCompleteEvent(int previousLevel, int newLevel, bool isLast)
:Event(EventTypes::LevelComplete, EventPriorities::High)
{
	mPreviousLevel = previousLevel;
	mNewLevel = newLevel;
	mIsLast = isLast;
};

//----------------------

WeatherChangedEvent::WeatherChangedEvent(WeatherTypes weatherType)
:Event(EventTypes::WeatherChanged, EventPriorities::Normal)
{
	mWeatherType = weatherType;
};

//----------------------

GameAreaFlashCounterEvent::GameAreaFlashCounterEvent(int seconds)
:Event(EventTypes::GameAreaFlashCounter, EventPriorities::Normal)
{
	mSeconds = seconds;
};
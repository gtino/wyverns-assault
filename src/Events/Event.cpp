#include "..\include\Events\Event.h"

using namespace WyvernsAssault;

Event::Event(EventTypes type, EventPriorities priority)
{
	//
	// TODO Constructor
	//
	this->mType = type;
	this->mPriority = priority;
	this->mData = 0;
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

EnemyHitEvent::EnemyHitEvent(EnemyPtr e, PlayerPtr p)
:Event(EventTypes::EnemyHit, EventPriorities::Normal)
{
	mEnemy = e;
	mPlayer = p;
};
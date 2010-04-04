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
#include "..\..\include\Events\EventsInterface.h"

using namespace WyvernsAssault;

void EventsInterface::initialize(EventsManager *em)
{
	mEventsManager = em;
}

void EventsInterface::registerHandler(EventHandlerPtr handler,EventTypes evtType)
{
	mEventsManager->registerHandler(handler, evtType);
}

void EventsInterface::unregisterHandler(EventHandlerPtr handler,EventTypes evtType)
{
	mEventsManager->unregisterHandler(handler, evtType);
}

// Raise an event
void EventsInterface::raiseEvent(EventPtr e)
{
	mEventsManager->addEvent(e);
}

// Raise an event
void EventsInterface::raiseEvent(EventPtr e, const float timer)
{
	e->setTimer(timer);

	mEventsManager->addEvent(e);
}


// Handle an event
void EventsInterface::handleEvent(EventPtr e)
{
}
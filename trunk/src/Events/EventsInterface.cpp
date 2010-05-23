#include "..\..\include\Events\EventsInterface.h"

using namespace WyvernsAssault;

// Raise an event
void EventsInterface::raiseEvent(const EventPtr e)
{
	mEventsManager->addEvent(e);
}

// Handle an event
void EventsInterface::handleEvent(const EventPtr e)
{
}
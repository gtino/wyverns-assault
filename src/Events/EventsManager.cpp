#include "..\include\Events\EventsManager.h"

using namespace WyvernsAssault;

EventsManager::EventsManager()
{
	//
	// TODO Constructor
	//
}

EventsManager::~EventsManager()
{
	//
	// TODO Destructor
	//
}

void EventsManager::addEvent(EventPtr evt)
{
	mEventQueue.push(evt);
}

void EventsManager::dispatchEvents()
{
	while(!mEventQueue.empty())
	{
		EventPtr evt = removeEvent();
		if (mEventHandlers.count(evt->getType()))
		{
			for (EventHandlerList::iterator it = mEventHandlers[evt->getType()].begin();it!=mEventHandlers[evt->getType()].end();++it)
			{
				(*it)->handleEvent(evt);				
			}
		}
	}
}

EventPtr EventsManager::peekEvent() const
{
	if (!mEventQueue.empty())
		return mEventQueue.top();
	else return EventPtr();
}

EventPtr EventsManager::removeEvent()
{
	if (!mEventQueue.empty())
	{
		EventPtr result=mEventQueue.top();
		mEventQueue.pop();
		return result;
	}
	return EventPtr();
}

void EventsManager::registerHandler(EventHandlerPtr handler, EventTypes evtType)
{
	if (mEventHandlers.count(evtType)==0)
		mEventHandlers[evtType].clear();

	mEventHandlers[evtType].push_back(handler);
}

void EventsManager::unregisterHandler(EventHandlerPtr hdl, EventTypes evtType)
{
	if (!mEventHandlers.empty() && mEventHandlers.count(evtType))
	{
		//look for handler in the list
		EventHandlerList::iterator newEnd=remove_if(mEventHandlers[evtType].begin(),
			mEventHandlers[evtType].end(),EventHandlerComparisonFunctor(hdl));

		mEventHandlers[evtType].erase(newEnd,mEventHandlers[evtType].end());

		if (mEventHandlers[evtType].size()==0)
			mEventHandlers.erase(evtType);
	}
}

void EventsManager::clear()
{
	while(!mEventQueue.empty())
		mEventQueue.pop();

	for (EventHandlerMap::iterator it=mEventHandlers.begin();it!=mEventHandlers.end();++it)
	{
		(it->second).clear();
	}

	mEventHandlers.clear();
}

void EventsManager::registerInterface(EventsInterface* eventsInterface)
{
	eventsInterface->initialize(this);
	eventsInterface->registerHandlers();
}

void EventsManager::unregisterInterface(EventsInterface* eventsInterface)
{
	eventsInterface->unregisterHandlers();
}
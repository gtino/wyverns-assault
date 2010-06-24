#include "..\include\Events\EventsManager.h"

using namespace WyvernsAssault;

// BEGIN SINGLETON
template<> EventsManager* Ogre::Singleton<EventsManager>::ms_Singleton = 0;
EventsManager* EventsManager::getSingletonPtr(void)
{
    return ms_Singleton;
}
EventsManager& EventsManager::getSingleton(void)
{  
    assert( ms_Singleton );  return ( *ms_Singleton );  
}
// END SINGLETON

EventsManager::EventsManager()
{
	//
	// TODO Constructor
	//
}

EventsManager::~EventsManager()
{
	clear();
}

void EventsManager::addEvent(EventPtr evt)
{
	if(evt->hasExpired())
		mEventQueue.push(evt);
	else
		mDelayedEventList.push_back(evt);
}

void EventsManager::update(const float elapsedSeconds)
{
	updateDelayedEvents(elapsedSeconds);
	dispatchEvents();
}

void EventsManager::updateDelayedEvents(const float elapsedSeconds)
{
	for(EventListIterator it = mDelayedEventList.begin();it != mDelayedEventList.end(); )
	{
		EventPtr e = (EventPtr)(*it);

		if(e->hasExpired())
		{
			mEventQueue.push(e);
			it = mDelayedEventList.erase(it);
		}
		else
		{
			e->updateTimer(elapsedSeconds);
			it++;
		}
	}
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

void EventsManager::registerInterface(EventsInterfacePtr eventsInterface)
{
	eventsInterface->initialize(this);
	eventsInterface->registerHandlers();
}

void EventsManager::unregisterInterface(EventsInterfacePtr eventsInterface)
{
	eventsInterface->unregisterHandlers();
}

// --------------------------------
// Lua Event Lib
// --------------------------------
LUA_BEGIN_BINDING(EventsManager, eventslib)
LUA_BIND(EventsManager, fire)
LUA_END_BINDING()

//
// Load lua scripts that will be used by this manager
//
LUA_BEGIN_LOAD_SCRIPTS(EventsManager)
// 
// TODO : Load scripts if needed
//
LUA_END_LOAD_SCRIPTS()

LUA_DEFINE_FUNCTION(EventsManager, fire)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be ?

	/* return the number of results */
	return 1;
}
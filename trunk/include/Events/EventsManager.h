/*
-----------------------------------------------------------------------------
Wyverns' Assault 3D Videgame.
Copyright (C) 2010  Giorgio Tino, Javier Soto Huesa, Jordi Carreras Ribot, 
					Marc Serena, Elm Oliver Torres

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

-----------------------------------------------------------------------------
*/
#ifndef __EVENTS_MANAGER_H_
#define __EVENTS_MANAGER_H_

#include <list>
#include <queue>
#include <vector>
#include <map>

#include <Ogre.h>
#include <OgreSingleton.h>
#include <boost/enable_shared_from_this.hpp>

#include "Event.h"
#include "EventHandler.h"
#include "EventsInterface.h"

namespace WyvernsAssault
{
	typedef std::priority_queue<EventPtr,std::vector<EventPtr>,EventComparator> EventQueue;
	typedef std::list<EventHandlerPtr> EventHandlerList;
	typedef std::map<EventTypes,EventHandlerList> EventHandlerMap;
	typedef std::list<EventPtr> EventList;
	typedef std::list<EventPtr>::iterator EventListIterator;

	class EventsInterface;
	typedef boost::shared_ptr<EventsInterface> EventsInterfacePtr;

	class EventsManager : public Ogre::Singleton<EventsManager>, public boost::enable_shared_from_this<EventsManager>
	{
	public:
		EventsManager();
		virtual ~EventsManager();
		static EventsManager& getSingleton(void);
		static EventsManager* getSingletonPtr(void);

	public:
		/// Delete all items from the event queue and
		/// event handlers map
		void clear();
		/// Add an event to the queue
		/// @param evt event to enqueue
		void addEvent(EventPtr evt);
		void update(const float elapsedSeconds);
		/// Get the highest-priority event on the queue
		/// @return highest-priority event on the queue
		EventPtr peekEvent() const;
		/// Remove the highest-priority event from the queue, and
		/// return it.
		/// @return removed event
		EventPtr removeEvent();

		//
		// Register an interface (handlers and events)
		//
		void registerInterface(EventsInterfacePtr eventsInterface);
		void unregisterInterface(EventsInterfacePtr eventsInterface);
	
	private:
		/// Update delayed events
		void updateDelayedEvents(const float elapsedSeconds);
		/// Process all events
		void dispatchEvents();

	public:
		/// Add a new event handler for the given event type
		/// @param handler	functor object containing the instance that will handle the event 
		///					and the callback method it'll invoke
		/// @param evtType  type of the event that will be handled
		void registerHandler(EventHandlerPtr handler,EventTypes evtType);


		/// Delete an event handler for the given event type
		/// @param handler  functor object to remove
		/// @param evtType  type of the event
		void unregisterHandler(EventHandlerPtr handler,EventTypes evtType);

	private:
		EventList mDelayedEventList;

		/** Priority queue to hold the events posted by the game objects or subsystems. */
		EventQueue mEventQueue;
		/// Map containing the registered event handlers. For each type of event there is a list linked to it
		/// so the events may be quickly dispatched only to the instances interested on them.
		/// For instance, say we have two event types: CAUSE_DAMAGE and RECEIVE_DAMAGE
		/// A particular game object wants to call its member function "onCauseDamage" any time he hits something
		/// Then, the map entry for CAUSE_DAMAGE will contain at least one event handler object linking the instance
		/// that will react to the event and the callback method (the function onCauseDamage).		
		EventHandlerMap mEventHandlers;
	};

	typedef boost::shared_ptr<EventsManager> EventsManagerPtr;
}

#endif // __EVENTS_MANAGER_H_
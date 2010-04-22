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

#include "Event.h"
#include "EventHandler.h"

namespace WyvernsAssault
{
	typedef std::priority_queue<EventPtr,std::vector<EventPtr>,EventComparator> EventQueue;
	typedef std::list<EventHandlerPtr> EventHandlerList;
	typedef std::map<EventTypes,EventHandlerList> EventHandlerMap;

	class EventsManager
	{
	public:
		EventsManager();
		~EventsManager();

	public:
		/// Delete all items from the event queue and
		/// event handlers map
		void clear();
		/// Add an event to the queue
		/// @param evt event to enqueue
		void addEvent(EventPtr evt);
		/// Process all events
		void dispatchEvents();
		/// Get the highest-priority event on the queue
		/// @return highest-priority event on the queue
		EventPtr peekEvent() const;
		/// Remove the highest-priority event from the queue, and
		/// return it.
		/// @return removed event
		EventPtr removeEvent();

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
}

#endif // __EVENTS_MANAGER_H_
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
#ifndef __EVENTS_INTERFACE_H_
#define __EVENTS_INTERFACE_H_

#include "Event.h"
#include "EventsDefines.h"
#include "EventHandler.h"
#include "EventsManager.h"

namespace WyvernsAssault
{
	class EventsInterface
	{
	public:
		// Set the events processor
		void initialize(EventsManager* em);
		// Register event handlers
		virtual void registerHandlers() = 0;
		// Unregister handlers
		virtual void unregisterHandlers() = 0;
		// Raise an event
		void raiseEvent(EventPtr e);
		// Raise an event after the given amount of time (in seconds)
		void raiseEvent(EventPtr e, const float timer);
		// Handle an event
		void handleEvent(EventPtr e);
	protected:
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
		// The events manager used to deal with events (raise, handle)
		EventsManager* mEventsManager;
	};
}

#endif // __EVENTS_INTERFACE_H_
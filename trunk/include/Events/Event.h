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
#ifndef __EVENT_H_
#define __EVENT_H_

#include <boost/shared_ptr.hpp>

namespace WyvernsAssault
{
	/** Event types */
	enum EventTypes
	{
		PlayerHit = 0,
		ObjectHit
	};

	/** Event priority, used to put the event in the correct queue */
	enum EventPriorities
	{
		Highest = 0,
		High,
		Normal,
		Low,
		Lower
	};

	typedef void* EventData;

	class Event
	{
	public:
		Event(EventTypes type, EventPriorities priority);
		virtual ~Event() = 0;

	public:
		/** Returns the event type */
		EventTypes getType();
		/** Returns the event priority */
		EventPriorities getPriority();
		/** Returens the event data */
		EventData getData();
		/** Sets the event data */
		void setData(EventData data);

	private:
		EventTypes mType;
		EventPriorities mPriority;
		EventData mData;
	};

	typedef boost::shared_ptr<Event> EventPtr;

	/** Functor class used to compare two event pointers */
	class EventComparator
	{
	public:
		/** Compare two events checking their priorities:
		    The event with the lower value will have higher priority */
		bool operator()(EventPtr& evt1, EventPtr& evt2);
	};
}

#endif // __EVENT_H_
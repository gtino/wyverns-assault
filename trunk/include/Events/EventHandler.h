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
#ifndef __EVENT_HANDLER_H_
#define __EVENT_HANDLER_H_

#include <boost/shared_ptr.hpp>

#include "Event.h"

namespace WyvernsAssault
{
	class EventHandler
	{
	public:
		virtual ~EventHandler() = 0;

		/// Handle the given event by performing double dispatch
		/// @param evt event to handle
		void handleEvent(const EventPtr evt)
		{
			invoke(evt);
		}
		/// Equality operator: It'll return true if
		/// their types are the same and they're deemed equal
		/// @param other instance to compare
		/// @return true if 'other' has the same type as this, 
		///				 and their values are equal
		bool operator== (const EventHandler& other) const
		{
			if (typeid(*this)!=typeid(other)) return false;
			return equal(other);
		}
		/// Return true if other is the same object as this
		/// @param other instance to compare
		/// @return true if 'other' is the same object as this
		virtual bool equal(const EventHandler& other) const=0;
	protected:
		/// Process the event
		/// @param evt event to handle
		virtual void invoke(const EventPtr evt)=0;
	};

	typedef boost::shared_ptr<EventHandler> EventHandlerPtr;

	/// Compares two event handler shared pointers
	class EventHandlerComparisonFunctor
	{
	private:
		/// instance against which other handlers will be compared
		EventHandlerPtr mInstance;
	public:
		/// Constructor
		EventHandlerComparisonFunctor(EventHandlerPtr instance):mInstance(instance){}
		/// Functor operator: it'll retrieve the raw pointers and then
		/// will test their contents for equality
		/// @param other handler to compare
		/// @return true if this instance is equal as 'other'
		bool operator()(EventHandlerPtr& other) const
		{
			return *(mInstance.get())==*(other.get());
		}
	};
}

#endif // __EVENT_HANDLER_H_
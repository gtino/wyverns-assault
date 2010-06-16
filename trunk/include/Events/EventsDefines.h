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
#ifndef __EVENTS_DEFINES_H__
#define __EVENTS_DEFINES_H__

#define EVENTS_INTERFACE()	void registerHandlers(); \
							void unregisterHandlers();

#define EVENTS_HANDLER(h)	void handle##h##Event(h##EventPtr evt);

#define EVENTS_BEGIN_REGISTER_HANDLERS(c)	void c##::registerHandlers() \
											{ \
											boost::shared_ptr<c> this_ = shared_from_this();
#define EVENTS_REGISTER_HANDLER(c,e)		registerHandler(EventHandlerPtr(new EventHandler<c,e##Event>(this_,&##c##::handle##e##Event)),EventTypes::##e);
#define EVENTS_END_REGISTER_HANDLERS()		}

#define EVENTS_BEGIN_UNREGISTER_HANDLERS(c)	void c##::unregisterHandlers() \
											{
#define EVENTS_UNREGISTER_HANDLER(c,e)		
#define EVENTS_END_UNREGISTER_HANDLERS()	}

#define EVENTS_DEFINE_HANDLER(c,e)	void c##::handle##e##Event(e##EventPtr evt)

#include <Ogre.h>

using namespace Ogre;

namespace WyvernsAssault
{
	class EventsManager;

	class EventsInterface;
	typedef boost::shared_ptr<EventsInterface> EventsInterfacePtr;
}

#endif // __EVENTS_DEFINES_H__
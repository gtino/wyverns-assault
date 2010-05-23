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

#define EVENTS_BEGIN_REGISTER_HANDLERS(c)	void c##::registerHandlers() \
											{
#define EVENTS_REGISTER_HANDLER(h,t)		mEventsManager->registerHandler(h,t);
#define EVENTS_END_REGISTER_HANDLERS();		}

#define EVENTS_BEGIN_UNREGISTER_HANDLERS(c)	void c##::registerHandlers() \
											{
#define EVENTS_UNREGISTER_HANDLER(h,t)		mEventsManager->unregisterHandler(h,t);
#define EVENTS_END_UNREGISTER_HANDLERS();	}

#endif // __EVENTS_DEFINES_H__
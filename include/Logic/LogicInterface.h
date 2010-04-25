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
#ifndef __LOGIC_INTERFACE_H__
#define __LOGIC_INTERFACE_H__

#include <Ogre.h>

#include "..\Lua\LuaDefines.h"

namespace WyvernsAssault
{
	/**
	Interface implemented by objects with logic
	*/
	class LogicInterface
	{
	public:
		// Initialize logic with callback to Lua scripting
		virtual void initializeLogic(){return;}

		virtual void finalizeLogic(){return;}

		virtual void updateLogic(lua_State *L, const float elapsedSeconds){return;}
	};
}

#endif // __LOGIC_INTERFACE_H__
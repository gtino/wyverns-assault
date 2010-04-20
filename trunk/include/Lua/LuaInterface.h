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
#ifndef __LUA_INTERFACE_H__
#define __LUA_INTERFACE_H__

extern "C" {
	#include "..\..\externals\Lua-5.0-SDK\include\lua.h"
	#include "..\..\externals\Lua-5.0-SDK\include\lualib.h"
	#include "..\..\externals\Lua-5.0-SDK\include\lauxlib.h"
}

#define DECLARE_LUA_LIBRARY(x) static const struct luaL_reg x[];
#define DECLARE_LUA_FUNCTION(x) static int x(lua_State *L);
#define EXPORT_LUA_LIBRARY const struct luaL_reg*
#define OPEN_LUA_LIBRARY(n,l) luaL_openlib(L, n, l, 0);

namespace WyvernsAssault
{
	/**
	Interface to export Lua scripting methods
	*/
	class LuaInterface
	{
	public:
		virtual EXPORT_LUA_LIBRARY luaGetLibrary() = 0;
		virtual void luaInitialize() = 0;
		virtual void luaFinalize() = 0;
	};
}

#endif // __LUA_INTERFACE_H__
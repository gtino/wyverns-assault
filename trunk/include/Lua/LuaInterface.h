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

#define LUA_SCRIPTS_FOLDER 

#define LUA_DECLARE_LIBRARY(x) static const struct luaL_reg x[];
#define LUA_DECLARE_FUNCTION(x) static int x(lua_State *L);

#define LUA_OPEN_LIBRARY(n,l) luaL_openlib(L, n, l, 0);
#define LUA_EXPORT_LIBRARY(n,l) const char* luaGetLibraryName() const {return n;} ; const struct luaL_reg* luaGetLibrary() {return l;}

#define LUA_BEGIN_BINDING(x) const struct luaL_reg x[] = {
#define LUA_BIND(x,y) {x, y},
#define LUA_END_BINDING() {NULL, NULL}};

namespace WyvernsAssault
{
	/**
	Interface to export Lua scripting methods
	*/
	class LuaInterface
	{
	public:
		virtual const char* luaGetLibraryName() const = 0;
		virtual const struct luaL_reg* luaGetLibrary() = 0;
		virtual void luaLoadScripts() = 0;
		virtual bool luaLoadScript(const char* name);
		virtual void luaInitialize(lua_State* L){this->L = L;};
		virtual void luaFinalize() = 0;
		virtual void luaReload() = 0;

	protected:
		/* the Lua interpreter */
		lua_State* L;
	};
}

#endif // __LUA_INTERFACE_H__
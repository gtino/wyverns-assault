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
#ifndef __LUA_MANAGER_H__
#define __LUA_MANAGER_H__

extern "C" {
	#include "..\..\externals\Lua-5.0-SDK\include\lua.h"
	#include "..\..\externals\Lua-5.0-SDK\include\lualib.h"
	#include "..\..\externals\Lua-5.0-SDK\include\lauxlib.h"
}

#include "..\Graphics\LightsManager.h"

namespace WyvernsAssault
{
	/**
	Class used to load/manage logic and AI scripts
	*/
	class LuaManager
	{
	public:
		LuaManager(LightsManager* lightsManager);
		~LuaManager();

	public:
		/** Initialize Lua and load Lua libs */
		bool initialize();
		/** Finalize Lua and unload Lua libs */
		void finalize();
		/** Reloads scripts */
		void reload();

		/** Runs all loaded scripts */
		void run(const float elapsedSeconds);

	private: // LightsManager
		static LightsManager* smLightsManager;

		// From C++ to Lua
		bool runLightsLogic(const float totalSeconds);

		// From Lua to C++
		static int getLightDiffuseColor(lua_State *L);
		static int setLightDiffuseColor(lua_State *L);
		static int getLightPosition(lua_State *L);
		static int setLightPosition(lua_State *L);

	private: // Game data
		static float smTotalSeconds;
		static float smElapsedSeconds;

		// From Lua to C++
		static int getTotalSeconds(lua_State *L);
		static int getElapsedSeconds(lua_State *L);

	private:
		/* the Lua interpreter */
		lua_State* L;
	};
}

#endif // __LUA_MANAGER_H__
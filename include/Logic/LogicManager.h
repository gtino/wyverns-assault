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
#ifndef __LOGIC_MANAGER_H__
#define __LOGIC_MANAGER_H__

#include "..\Lua\LuaInterface.h"

namespace WyvernsAssault
{
	/**
	Class used to load/manage logic and AI scripts
	*/
	class LogicManager : public LuaInterface
	{
	public:
		LogicManager();
		~LogicManager();

		bool initialize();
		void finalize();
		bool update(const float elapsedSeconds);

	private: // Game data
		static float smTotalSeconds;
		static float smElapsedSeconds;

		// Game Lib (exported to Lua)
		DECLARE_LUA_LIBRARY(gamelib);

		// From Lua to C++
		DECLARE_LUA_FUNCTION(getTotalSeconds)
		DECLARE_LUA_FUNCTION(getElapsedSeconds)

		EXPORT_LUA_LIBRARY luaGetLibrary() {return gamelib;}

		void luaInitialize() {LogicManager::smTotalSeconds = 0; LogicManager::smElapsedSeconds = 0;}
		void luaFinalize() {LogicManager::smTotalSeconds = 0; LogicManager::smElapsedSeconds = 0;}
	};
}

#endif // __LOGIC_MANAGER_H__
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

#include <Ogre.h>
#include <OgreSingleton.h>
#include <boost/enable_shared_from_this.hpp>

#include "..\Lua\LuaInterface.h"

namespace WyvernsAssault
{
	/**
	Class used to load/manage logic and AI scripts
	*/
	class LogicManager : public Ogre::Singleton<LogicManager>, public boost::enable_shared_from_this<LogicManager>, public LuaInterface
	{
	public:
		LogicManager();
		~LogicManager();
		static LogicManager& getSingleton(void);
		static LogicManager* getSingletonPtr(void);

		bool initialize();
		void finalize();
		bool update(const float elapsedSeconds);

	private: // Game data
		LUA_PROPERTY(TotalSeconds,float);
		LUA_PROPERTY(ElapsedSeconds,float);

	public:
		// Game Lib (exported to Lua)
		LUA_LIBRARY("Game",gamelib)

		// From Lua to C++
		LUA_FUNCTION(getTotalSeconds)
		LUA_FUNCTION(getElapsedSeconds)

		void luaLoadScripts();
		void luaInitialize(lua_State *L);
	};

	typedef boost::shared_ptr<LogicManager> LogicManagerPtr;
}

#endif // __LOGIC_MANAGER_H__
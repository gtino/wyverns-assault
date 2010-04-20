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

#include "..\Lua\LuaInterface.h"
#include "..\Graphics\LightsManager.h"
#include "..\Camera\CameraManager.h"
#include "..\Physics\PhysicsManager.h"
#include "..\Entity\Enemy\EnemyManager.h"
#include "..\Entity\Player\PlayerManager.h"
#include "..\Entity\Item\ItemManager.h"
#include "..\Audio\AudioManager.h"

namespace WyvernsAssault
{
	/**
	Class used to load/manage logic and AI scripts
	*/
	class LuaManager
	{
	public:
		LuaManager();
		~LuaManager();

	public:
		/** Initialize Lua and load Lua default libs */
		bool initialize();
		/** Add and open custom library */
		bool addLibrary(const char* name, LuaInterface* luaInterface);
		/** Do file */
		bool loadScript(const char* name);
		/** Finalize Lua and unload Lua libs */		
		void finalize();
		/** Reloads scripts */
		bool reload();

		/** Runs all loaded scripts */
		bool update(const float elapsedSeconds);
		void enable();
		void disable();
		bool isEnabled();

	private:
		bool mEnabled;

	private:
		// From C++ to Lua
		bool runScenario(const float totalSeconds);

	private: // PhysicsManager
		static PhysicsManager* smPhysicsManager;

		// Physics Lib (exported to Lua)
		DECLARE_LUA_LIBRARY(physicslib);

		DECLARE_LUA_FUNCTION(getHOT)

	private: // EnemyManager
		static EnemyManager* smEnemyManager;

		// Enemys Lib (exported to Lua)
		DECLARE_LUA_LIBRARY(enemylib);

		DECLARE_LUA_FUNCTION(createEnemy)
		DECLARE_LUA_FUNCTION(getEnemyPosition)
		DECLARE_LUA_FUNCTION(setEnemyPosition)
		DECLARE_LUA_FUNCTION(setEnemyState)
		DECLARE_LUA_FUNCTION(removeEnemy)

	private: // PlayerManager
		static PlayerManager* smPlayerManager;

		// Player Lib (exported to Lua)
		DECLARE_LUA_LIBRARY(playerlib);

		DECLARE_LUA_FUNCTION(getPlayerPosition)

	private: // ItemManager
		static ItemManager* smItemManager;

		// Item Lib (exported to Lua)
		DECLARE_LUA_LIBRARY(itemlib);

		DECLARE_LUA_FUNCTION(createItem)
		DECLARE_LUA_FUNCTION(getItemPosition)
		DECLARE_LUA_FUNCTION(setItemPosition)
		DECLARE_LUA_FUNCTION(removeItem)

	private: // AudioManager
		static AudioManager* smAudioManager;

		// Audio Lib (exported to Lua)
		DECLARE_LUA_LIBRARY(audiolib);

		DECLARE_LUA_FUNCTION(playSound)

	private:
		/* the Lua interpreter */
		lua_State* L;
	};
}

#endif // __LUA_MANAGER_H__
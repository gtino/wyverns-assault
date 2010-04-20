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
		LuaManager(LightsManager* lightsManager, PhysicsManager* physicsManager, EnemyManager* enemysManager, PlayerManager* playerManager, ItemManager* itemManager, AudioManager* audioManager);
		~LuaManager();

	public:
		/** Initialize Lua and load Lua libs */
		bool initialize();
		/** Finalize Lua and unload Lua libs */
		void finalize();
		/** Reloads scripts */
		bool reload();

		/** Runs all loaded scripts */
		bool run(const float elapsedSeconds);
		void enable();
		void disable();
		bool isEnabled();

	private:
		bool mEnabled;

	private:
		// From C++ to Lua
		bool runScenario(const float totalSeconds);

	private: // LightsManager
		static LightsManager* smLightsManager;

		// Lights Lib (exported to Lua)
		static const struct luaL_reg lightlib [];

		// From Lua to C++
		static int getLightDiffuseColor(lua_State *L);
		static int setLightDiffuseColor(lua_State *L);
		static int getLightPosition(lua_State *L);
		static int setLightPosition(lua_State *L);
		static int getAmbientLight(lua_State *L);
		static int setAmbientLight(lua_State *L);

	private: // Game data
		static float smTotalSeconds;
		static float smElapsedSeconds;

		// Game Lib (exported to Lua)
		static const struct luaL_reg gamelib [];

		// From Lua to C++
		static int getTotalSeconds(lua_State *L);
		static int getElapsedSeconds(lua_State *L);

	private: // PhysicsManager
		static PhysicsManager* smPhysicsManager;

		// Physics Lib (exported to Lua)
		static const struct luaL_reg physicslib [];

		static int getHOT(lua_State *L);

	private: // EnemyManager
		static EnemyManager* smEnemysManager;

		// Enemys Lib (exported to Lua)
		static const struct luaL_reg enemylib [];

		static int createEnemy(lua_State *L);
		static int getEnemyPosition(lua_State *L);
		static int setEnemyPosition(lua_State *L);
		static int setEnemyState(lua_State *L);
		static int removeEnemy(lua_State *L);

	private: // PlayerManager
		static PlayerManager* smPlayerManager;

		// Player Lib (exported to Lua)
		static const struct luaL_reg playerlib [];

		static int getPlayerPosition(lua_State *L);

	private: // ItemManager
		static ItemManager* smItemManager;

		// Item Lib (exported to Lua)
		static const struct luaL_reg itemlib [];

		static int createItem(lua_State *L);
		static int getItemPosition(lua_State *L);
		static int setItemPosition(lua_State *L);
		static int removeItem(lua_State *L);

	private: // AudioManager
		static AudioManager* smAudioManager;

		// Audio Lib (exported to Lua)
		static const struct luaL_reg audiolib [];

		static int playSound(lua_State *L);

	private:
		/* the Lua interpreter */
		lua_State* L;
	};
}

#endif // __LUA_MANAGER_H__
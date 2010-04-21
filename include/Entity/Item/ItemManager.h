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
#ifndef __ITEM_MANAGER_H__
#define __ITEM_MANAGER_H__

#include "..\..\..\include\Lua\LuaInterface.h"

#include "..\..\..\include\Entity\EntityManager.h"

namespace WyvernsAssault
{
	/**
	Class used to manage all the enemies
	*/
	class ItemManager : public EntityManager, public LuaInterface
	{
	public:
		ItemManager();
		~ItemManager();

	public:
		bool initialize();
		void finalize();

	// --------------------------------
	// BEGIN Lua Interface Declarations
	// --------------------------------
	public:
		// Item Lib (exported to Lua)
		LUA_LIBRARY("Item",itemlib);

		LUA_FUNCTION(createItem)
		LUA_FUNCTION(getItemPosition)
		LUA_FUNCTION(setItemPosition)
		LUA_FUNCTION(removeItem)

	public:
		void luaLoadScripts(){};
		void luaInitialize(lua_State* L) {LuaInterface::luaInitialize(L);ItemManager::smItemManager = this;}
		void luaFinalize() {ItemManager::smItemManager = NULL;}
		void luaReload(){};

	private: // ItemManager
		static ItemManager* smItemManager;
	// ------------------------------
	// END Lua Interface Declarations
	// ------------------------------
	};
}

#endif // __ITEM_MANAGER_H__
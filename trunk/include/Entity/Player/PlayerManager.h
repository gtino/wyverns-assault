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
#ifndef __PLAYER_MANAGER_H__
#define __PLAYER_MANAGER_H__

#include <Ogre.h>
#include <OgreSingleton.h>

#include "..\..\..\include\Lua\LuaInterface.h"
#include "..\..\..\include\Entity\EntityManager.h"
#include "Player.h"

using namespace Ogre;

namespace WyvernsAssault
{
	typedef std::map<Ogre::String,PlayerPtr> PlayerMap;
	typedef std::map<Ogre::String,PlayerPtr>::iterator PlayerMapIterator;

	typedef std::vector<PlayerPtr> PlayerList;
	typedef std::vector<PlayerPtr>::iterator PlayerListIterator;

	/**
	Class used to manage all the enemies
	*/
	class PlayerManager : public Ogre::Singleton<PlayerManager>, public EntityManager, public LuaInterface
	{
	public:
		PlayerManager(Ogre::SceneManager* sceneManager);
		~PlayerManager();
		static PlayerManager& getSingleton(void);
		static PlayerManager* getSingletonPtr(void);

		void initialize();
		void finalize();

		PlayerPtr createPlayer(Ogre::String name, Ogre::String mesh);

		PlayerPtr getPlayer(Ogre::String name);
		bool removePlayer(Ogre::String name);

	private:
		SceneManager *mSceneManager;

		PlayerList mPlayerList;
		PlayerMap mPlayerMap;

	// --------------------------------
	// BEGIN Lua Interface Declarations
	// --------------------------------
	public:
		// Player Lib (exported to Lua)
		LUA_LIBRARY("Player",playerlib);

		LUA_FUNCTION(getPlayerPosition)

	public:
		void luaLoadScripts(){};
	// ------------------------------
	// END Lua Interface Declarations
	// ------------------------------
	};
}

#endif // __PLAYER_MANAGER_H__
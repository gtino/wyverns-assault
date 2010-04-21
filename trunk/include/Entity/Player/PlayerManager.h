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

// Player default movement speed
#define SPEED	5

using namespace Ogre;

namespace WyvernsAssault
{
	/**
	Class used to manage all the enemies
	*/
	class PlayerManager : public Ogre::Singleton<PlayerManager>, public EntityManager, public LuaInterface
	{
	public:
		PlayerManager();
		~PlayerManager();
		static PlayerManager& getSingleton(void);
		static PlayerManager* getSingletonPtr(void);

		void initialize(String name, String mesh, SceneManager* levelSceneManager, Vector3 position);
		void finalize();

		SceneNode* getPlayerSceneNode() const {return mPlayerSceneNode;}

		Vector3 getPlayerPosition(){ return mPlayerSceneNode->getPosition(); }

		// Player movement functions
		void move(Real x, Real y, Real z);

		// Animation functions
		void updateAnimation(float elapsedSeconds);

	private:
		void initializeVariables();
		SceneManager *mSceneManager;

	private:
		Entity*				mPlayerMesh;
		SceneNode*			mPlayerSceneNode;
		AnimationState*		mPlayerAnimation;

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
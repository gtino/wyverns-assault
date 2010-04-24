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
#ifndef __PHYSICS_MANAGER_H__
#define __PHYSICS_MANAGER_H__

#include <Ogre.h>
#include <OgreSingleton.h>
//#include "OgreOde_Core.h"

#include "..\Lua\LuaInterface.h"
#include "..\Entity\Player\PlayerManager.h"
#include "..\Entity\Player\Player.h"

using namespace Ogre;

namespace WyvernsAssault
{

	/**
	Class used to manage entities/world physics
	*/
	class PhysicsManager : public Ogre::Singleton<PhysicsManager>, public LuaInterface, public OgreOde::CollisionListener
	{
	public:
		PhysicsManager(SceneManager* sceneManager);
		~PhysicsManager();
		static PhysicsManager& getSingleton(void);
		static PhysicsManager* getSingletonPtr(void);

	public:
		bool initialize();
		void finalize();

		void synchronizeWorld(Real time);
		void showDebuggObjects();

		void createPhysicGround(Ogre::String mesh);
		void createPhysicCharacter(Ogre::String name,PlayerPtr mPlayer);
		void createPhysicEnemy(Ogre::String name,Ogre::String mesh);

		//Update ray of one character
		void updateRay(PlayerPtr mPlayer);

		/*
		Called by OgreOde whenever a collision occurs, so 
		that we can modify the contact parameters
		*/
		bool collision(OgreOde::Contact* contact);

		//Move one character
		void move(PlayerPtr mPlayer, int rotate, int thrust);
		

	// --------------------------------
	// BEGIN Lua Interface Declarations
	// --------------------------------
	public:
		// Physics Lib (exported to Lua)
		LUA_LIBRARY("Physics",physicslib);

		LUA_FUNCTION(getHOT)

	public:
		void luaLoadScripts(){};
	// ------------------------------
	// END Lua Interface Declarations
	// ------------------------------

	protected:
		SceneManager* mSceneManager;
		OgreOde::World* mWorld;
		OgreOde::StepHandler* mStepper;
		OgreOde::Space* mSpace;

		OgreOde::TriangleMeshGeometry* geom_ground;
		PlayerPtr mPlayer_temp;

	};
}

#endif // __PHYSICS_MANAGER_H__
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
#include <boost/enable_shared_from_this.hpp>
//#include "OgreOde_Core.h"

#include "..\Lua\LuaInterface.h"

#include "PhysicsInterface.h"

#include "..\Entity\Player\PlayerManager.h"
#include "..\Entity\Player\Player.h"

#include "..\Entity\Enemy\EnemyManager.h"
#include "..\Entity\Enemy\Enemy.h"

#include "..\Events\EventsInterface.h"

using namespace Ogre;

namespace WyvernsAssault
{
	typedef std::map<size_t,PlayerPtr> OdePlayerMap;
	typedef std::map<size_t,PlayerPtr>::iterator OdePlayerMapIterator;

	typedef std::map<size_t,EnemyPtr> OdeEnemyMap;
	typedef std::map<size_t,EnemyPtr>::iterator OdeEnemyMapIterator;

	// ------------------------
	// NEW PHYSICS!
	// ------------------------
	// ------------------------

	/**
	Class used to manage entities/world physics
	*/
	class PhysicsManager	: public Ogre::Singleton<PhysicsManager>
							, public boost::enable_shared_from_this<PhysicsManager>
							, public LuaInterface
							, public OgreOde::CollisionListener
							, public EventsInterface
	{
	public:
		PhysicsManager(SceneManager* sceneManager);
		~PhysicsManager();
		static PhysicsManager& getSingleton(void);
		static PhysicsManager* getSingletonPtr(void);

	public:
		bool initialize();
		void finalize();
		SceneManager* getSceneManager(){return this->mSceneManager;}

		void showDebugObjects();

		void update(const float elapsedSeconds);

		void createGround(Ogre::String mesh,Ogre::String name,Ogre::Vector3 position,Ogre::Vector3 scale);
		void addPlayer(PlayerPtr player);
		void addEnemy(EnemyPtr enemy);
		void addAnimal(EnemyPtr animal);

		/*
		Called by OgreOde whenever a collision occurs, so 
		that we can modify the contact parameters
		*/
		bool collision(OgreOde::Contact* contact);

		//Move one character
		void move(PlayerPtr player, Vector3 direction);
		void move(EnemyPtr enemy, int rotate, int thrust);
		void moveAnimal(EnemyPtr animal, Vector3 direction);

		// TEST!
		void checkForCollisions();

		//
		// Events interface
		//
		// Register event handlers
		void registerHandlers(){};
		// Unregister handlers
		void unregisterHandlers(){};

	private:
		void synchronizeWorld(Real time);
		//Update ray of one character
		void updateRay(PlayerPtr player); 
		void updateRay(EnemyPtr enemy);

	protected:
		SceneManager* mSceneManager;
		OgreOde::World* mWorld;
		OgreOde::StepHandler* mStepper;
		OgreOde::Space* mSpace;

		OgreOde::TriangleMeshGeometry* geom_ground;
		
		OdePlayerMap mPlayerMap;
		OdeEnemyMap mEnemyMap;
		OdeEnemyMap mAnimalMap;
		OdePlayerMap mPlayerGeomMap;
		OdeEnemyMap mEnemyGeomMap;
		OdeEnemyMap mAnimalGeomMap;

		// Keep track of the things we create so we can 
		// delete them automatically when we switch scenes
		std::vector<OgreOde::Body*>		_bodies;
		std::vector<OgreOde::Geometry*> _geoms;
		std::vector<OgreOde::Joint*>	_joints;

	private:
		// Control variable for states changing (from attacking to non attacking). Need for skipping multiple calling to same event.
		bool mPlayerSpecialState;
		int mPlayerAttackLast;

	// ------------------------
	// NEW PHYSICS!
	// ------------------------
	public:

	private:
		//PlayerMap mPlayerMap;
		//EnemyMap mEnemyMap;
		//ItemMap mItemMap;
		//ObjectMap mObjectMap;

		// 

	// ------------------------

	// --------------------------------
	// BEGIN Lua Interface Declarations
	// --------------------------------
	public:
		// Physics Lib (exported to Lua)
		LUA_LIBRARY("Physics",physicslib);

		LUA_FUNCTION(LuaGetHOT)
		LUA_FUNCTION(LuaGetDistance)
		LUA_FUNCTION(LuaGetNearestPlayer)

	public:
		void luaLoadScripts(){};
	// ------------------------------
	// END Lua Interface Declarations
	// ------------------------------
	};

	typedef boost::shared_ptr<PhysicsManager> PhysicsManagerPtr;
}

#endif // __PHYSICS_MANAGER_H__
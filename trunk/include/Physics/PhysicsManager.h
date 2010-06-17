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

#define PHYSICS_NODE_NAME "PhysicsNode"

#include <Ogre.h>
#include <OgreSingleton.h>
#include <boost/enable_shared_from_this.hpp>

#include "..\Lua\LuaInterface.h"

#include "PhysicsInterface.h"

#include "..\Utils\Utils.h"
#include "..\Entity\Player\PlayerManager.h"
#include "..\Entity\Player\Player.h"

#include "..\Entity\Enemy\EnemyManager.h"
#include "..\Entity\Enemy\Enemy.h"

#include "..\Events\EventsInterface.h"

using namespace Ogre;

namespace WyvernsAssault
{		
	//typedef std::map<size_t,ItemPtr> ItemMap;
	//typedef std::map<size_t,ItemPtr>::iterator ItemMapIterator;

	//typedef std::map<size_t,ObjectPtr> ObjectMap;
	//typedef std::map<size_t,ObjectPtr>::iterator ObjectMapIterator;

	/* Ground types
	*/
	enum GroundQueryFlags
	{
		BASIC_GROUND_MASK = 1<<0,
		ROAD_GROUND_MASK = 1<<1,
		WATER_GROUND_MASK = 1<<2,
		WHEAT_GROUND_MASK = 1<<3,
		BORDER_GROUND_MASK = 1<<4
	};

	/**
	Class used to manage entities/world physics
	*/
	class PhysicsManager	: public Ogre::Singleton<PhysicsManager>
							, public boost::enable_shared_from_this<PhysicsManager>
							, public LuaInterface
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

		void addPhysicGround(Ogre::String mesh, Ogre::String name, WyvernsAssault::GroundQueryFlags type, Ogre::Vector3 position, Ogre::Vector3 scale);
		void addPhysicPlayer(PlayerPtr player);
		void addPhysicEnemy(EnemyPtr enemy);
		//void addPhysicItem();
		//void addPhysicObject();

		void removeEnemy(EnemyPtr enemy);

		void update(const float elapsedSeconds);

		//Move one character
		void move(PlayerPtr player, const float elapsedSeconds, bool fastMode = false);
		void move(EnemyPtr enemy, const float elapsedSeconds);

		// Collision check
		void checkForCollisions();

		Ogre::SceneNode* _getSceneNode() const { return mPhysicsNode; }

		// Debug visibility function
		void setDebugEnabled(bool visible){ mPhysicsNode->setVisible(visible); }

		//
		// Events interface
		//
		// Register event handlers
		void registerHandlers();
		// Unregister handlers
		void unregisterHandlers();

		void handleEnemyKillEvent(EnemyKillEventPtr evt);

	private:

		Vector3 calculateY(const Vector3 &point, const Ogre::uint32 queryMask = 0xFFFFFFFF);
		bool raycast(const Vector3 &point, const Vector3 &normal, Vector3 &result, float &closest_distance, const Ogre::uint32 queryMask);
		bool collidesWithBorders(const Vector3& fromPoint, const Vector3& toPoint, const float collisionRadius = 2.5f, const float rayHeightLevel = 0.0f, const uint32 queryMask = 0xFFFFFFFF);
		void GetMeshInformation(const Ogre::MeshPtr mesh,
                                size_t &vertex_count,
                                Ogre::Vector3* &vertices,
                                size_t &index_count,
                                unsigned long* &indices,
                                const Ogre::Vector3 &position,
                                const Ogre::Quaternion &orient,
                                const Ogre::Vector3 &scale);

	protected:

		SceneManager* mSceneManager;
		SceneNode* mPhysicsNode;
		Ogre::RaySceneQuery *mRaySceneQuery;

		PlayerMap mPlayerMap;
		EnemyMap mEnemyMap;
		//ItemMap mItemMap;
		//ObjectMap mObjectMap;

	private:

		// Control variable for states changing (from attacking to non attacking). Need for skipping multiple calling to same event.
		bool mPlayerSpecialState;
		int mPlayerAttackLast;


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
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

#include "..\Entity\Item\ItemManager.h"
#include "..\Entity\Item\Item.h"

#include "..\Scenario\ScenarioManager.h"
#include "..\Scenario\Object.h"

#include "..\Events\EventsInterface.h"

using namespace Ogre;

namespace WyvernsAssault
{		

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
		void addPhysicItem(ItemPtr item);
		void addPhysicObject(ObjectPtr obj);

		void removeEnemy(EnemyPtr enemy);
		void removeItem(ItemPtr item);
		void removeObject(ObjectPtr obj);

		void update(const float elapsedSeconds);

		//Move one character
		void move(PlayerPtr player, const float elapsedSeconds, bool fastMode = false);
		void move(EnemyPtr enemy, const float elapsedSeconds);

		// Collision check
		void checkForCollisions();

		Ogre::SceneNode* _getSceneNode() const { return mPhysicsNode; }

		// Debug visibility function
		void setDebugEnabled(bool visible){ mPhysicsNode->setVisible(visible); }

		// ----------------
		// Events interface
		// ----------------
		EVENTS_INTERFACE()

		EVENTS_HANDLER(ItemCatch)
		EVENTS_HANDLER(EnemyKilled)

	private:

		Vector3 calculateY(const Vector3 &point);
		bool collides(const Vector3& fromPoint, const Vector3& toPoint, PhysicsMeshInfo objInfo, const float collisionRadius = 2.5, const float rayHeightLevel = 0.0f);
		bool raycast(const Vector3 &point, const Vector3 &normal, Vector3 &result, float &closest_distance, PhysicsMeshInfo objInfo);
		bool collidesAllObjects(PlayerPtr player, const Vector3& fromPoint, const Vector3& toPoint, const float collisionRadius = 2.5f, const float rayHeightLevel = 0.0f );

	protected:

		SceneManager* mSceneManager;
		SceneNode* mPhysicsNode;
		Ogre::RaySceneQuery *mRaySceneQuery;
		
		//Grounds geometry
		GeometryPtr basicGround;
		GeometryPtr borderGround;

		PlayerMap mPlayerMap;
		EnemyMap mEnemyMap;
		ItemMap mItemMap;
		ObjectMap mObjectMap;

	private:
		int		mLastAttackChecked;

	public:
		void enable() {mEnabled = true;}
		void disable(){mEnabled = false;}
		bool isEnabled(){return mEnabled;}

	private: // Game data
		bool mEnabled;

	// --------------------------------
	// BEGIN Lua Interface Declarations
	// --------------------------------
	public:
		LUA_INTERFACE();

		// Physics Lib (exported to Lua)
		LUA_LIBRARY("Physics",physicslib);

		LUA_FUNCTION(enable)
		LUA_FUNCTION(disable)
		LUA_FUNCTION(isEnabled)
		LUA_FUNCTION(getHOT);
		LUA_FUNCTION(getDistance);
		LUA_FUNCTION(getNearestPlayer);
	// ------------------------------
	// END Lua Interface Declarations
	// ------------------------------
	};

	typedef boost::shared_ptr<PhysicsManager> PhysicsManagerPtr;

}

#endif // __PHYSICS_MANAGER_H__
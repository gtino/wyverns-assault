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
#ifndef __ENEMY_MANAGER_H__
#define __ENEMY_MANAGER_H__

#define ENEMY_NODE_NAME "EnemyNode"

#include <Ogre.h>
#include <OgreSingleton.h>

#include <boost/enable_shared_from_this.hpp>

#include "..\..\Debug\Debug.h"

#include "..\..\Utils\Utils.h"
#include "..\..\Lua\LuaInterface.h"
#include "..\..\Entity\EntityManager.h"
#include "..\..\Events\EventsInterface.h"

#include "Enemy.h"

namespace WyvernsAssault
{
	typedef std::vector<EnemyPtr> EnemyList;
	typedef std::vector<EnemyPtr>::iterator EnemyListIterator;

	typedef std::map<int, EnemyList> EnemyMapList;
	typedef std::map<int, EnemyList>::iterator EnemyMapListIterator;

	/**
	Class used to manage all the enemies
	*/
	class EnemyManager	: public Ogre::Singleton<EnemyManager>
						, public boost::enable_shared_from_this<EnemyManager>
						, public EntityManager
						, public LuaInterface
						, public EventsInterface
	{
	public:
		EnemyManager(Ogre::SceneManager* sceneManager);
		~EnemyManager();
		static EnemyManager& getSingleton(void);
		static EnemyManager* getSingletonPtr(void);

	public:
		void initialize();
		void finalize();
		SceneManager* getSceneManager(){return this->mSceneManager;}

		Enemy::EnemyParameters createParameters(int difficult, Enemy::EnemyTypes type);

		EnemyPtr createEnemy(Enemy::EnemyTypes type, int difficult, Vector3 position);	// For random enemies creation.
		EnemyPtr createEnemy(Enemy::EnemyTypes type, Ogre::String name, Ogre::Entity* mesh, Ogre::SceneNode* sceneNode, Enemy::EnemyParameters params, int gameArea, bool visible);

		int getCount();
		int getCount(int gameArea);

		EnemyPtr getEnemy(int index);
		EnemyPtr getEnemy(int index, int gameArea);
		EnemyPtr getEnemy(Ogre::String name);
		EnemyPtr getEnemy(Ogre::String name, int gameArea);

		bool removeEnemy(Ogre::String name);

		void update(const float elpasedSeconds);

		// Visibility
		void setEnemiesVisible(bool visibility, int gameArea);

		// Enable Debug Stuff
		void setDebugEnabled(bool isDebugEnabled);
		bool getDebugEnabled(){return mIsDebugEnabled;};

		Ogre::SceneNode* _getSceneNode() const { return mEnemyNode; }

	private:
		Ogre::String createUniqueId();

	private:
		Ogre::SceneManager*		mSceneManager;
		Ogre::SceneNode*		mEnemyNode;

		// Where enemies are stored and sorted by game area
		EnemyMapList			mEnemyMapList;
		int						mCurrentGameArea;

		int						mId;
		bool					mIsDebugEnabled;

	public:
		// ----------------
		// Events interface
		// ----------------
		EVENTS_INTERFACE()

		EVENTS_HANDLER(Collision)
		EVENTS_HANDLER(EnemyHit)
		EVENTS_HANDLER(EnemyKilled)
		EVENTS_HANDLER(EnemyRemove)
		EVENTS_HANDLER(EnemyCustom)
		EVENTS_HANDLER(EnemyCreation)
		EVENTS_HANDLER(GameAreaChanged)
		EVENTS_HANDLER(GameAreaCleared)

		// --------------------------------
		// BEGIN Lua Interface Declarations
		// --------------------------------
	public:
		LUA_INTERFACE();

		// Enemies Lib (exported to Lua)
		LUA_LIBRARY("Enemy", enemylib);

		LUA_FUNCTION(create)
		LUA_FUNCTION(getCount)
		LUA_FUNCTION(getName)
		LUA_FUNCTION(getPosition)
		LUA_FUNCTION(setPosition)
		LUA_FUNCTION(setState)
		LUA_FUNCTION(setTarget)
		LUA_FUNCTION(getStateTimeout)
		LUA_FUNCTION(isHurt)
		LUA_FUNCTION(isDying)
		LUA_FUNCTION(remove)
		// ------------------------------
		// END Lua Interface Declarations
		// ------------------------------
	};

	typedef boost::shared_ptr<EnemyManager> EnemyManagerPtr;
}

#endif // __ENEMY_MANAGER_H__
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

#include <Ogre.h>
#include <OgreSingleton.h>
#include <boost/enable_shared_from_this.hpp>

#include <vector>

#include "..\..\Lua\LuaInterface.h"
#include "..\..\Entity\EntityManager.h"
#include "..\..\Events\EventsInterface.h"
#include "Enemy.h"

namespace WyvernsAssault
{
	typedef std::map<Ogre::String,EnemyPtr> EnemyMap;
	typedef std::map<Ogre::String,EnemyPtr>::iterator EnemyMapIterator;

	typedef std::vector<EnemyPtr> EnemyList;
	typedef std::vector<EnemyPtr>::iterator EnemyListIterator;

	/**
	Class used to manage all the enemies
	*/
	class EnemyManager : public Ogre::Singleton<EnemyManager>, public boost::enable_shared_from_this<EnemyManager>, public EntityManager, public LuaInterface, public EventsInterface
	{
		public:
		EnemyManager(Ogre::SceneManager* sceneManager);
		~EnemyManager();
		static EnemyManager& getSingleton(void);
		static EnemyManager* getSingletonPtr(void);
		
		void initialize();
		void finalize();
		SceneManager* getSceneManager(){return this->mSceneManager;}

		EnemyPtr createEnemy(EnemyTypes type);
		EnemyPtr createEnemy(EnemyTypes type, Ogre::String name, Ogre::String mesh);

		int getCount();

		EnemyPtr getEnemy(int index);
		EnemyPtr getEnemy(Ogre::String name);

		bool removeEnemy(Ogre::String name);

		/* TODO : Enemy IA
		*/
		void update(const float elpasedSeconds);

		//
		// Events interface
		//
		// Register event handlers
		void registerHandlers();
		// Unregister handlers
		void unregisterHandlers();

		void handleCollisionEvent(CollisionEventPtr evt);
		void handleEnemyHitEvent(EnemyHitEventPtr evt);

	private:
		Ogre::String createUniqueId();

	private:
		int mCount;
		int mId;

		EnemyList mEnemyList;
		EnemyMap mEnemyMap;

		Ogre::SceneManager* mSceneManager;

	// --------------------------------
	// BEGIN Lua Interface Declarations
	// --------------------------------
	public:
		// Enemys Lib (exported to Lua)
		LUA_LIBRARY("Enemy", enemylib);

		LUA_FUNCTION(createEnemy)
		LUA_FUNCTION(getEnemyCount)
		LUA_FUNCTION(getEnemyName)
		LUA_FUNCTION(getEnemyPosition)
		LUA_FUNCTION(setEnemyPosition)
		LUA_FUNCTION(setEnemyState)
		LUA_FUNCTION(setEnemyTarget)
		LUA_FUNCTION(getEnemyStateTimeout)
		LUA_FUNCTION(isEnemyHurt)
		LUA_FUNCTION(removeEnemy)

	public:
		void luaLoadScripts(){luaLoadScript(".\\data\\scripts\\EnemyLogic.lua");}

	private:
		static const struct luaL_reg l[];
	// ------------------------------
	// END Lua Interface Declarations
	// ------------------------------
	};

	typedef boost::shared_ptr<EnemyManager> EnemyManagerPtr;
}

#endif // __ENEMY_MANAGER_H__
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

#include "..\..\..\include\Entity\EntityManager.h"
#include "Enemy.h"
#include <vector>

namespace WyvernsAssault
{
	/** List of enemy types */
	enum EnemyTypes
	{
		Naked	= 0,
		Soldier	= 1,
		Wizard	= 2,
		Knight	= 3,
		Pesant	= 4,
		Woman	= 5
	};

	typedef std::map<Ogre::String,EnemyPtr> EnemyMap;
	typedef std::map<Ogre::String,EnemyPtr>::iterator EnemyMapIterator;

	typedef std::vector<EnemyPtr> EnemyList;
	typedef std::vector<EnemyPtr>::iterator EnemyListIterator;

	/**
	Class used to manage all the enemies
	*/
	class EnemyManager : public EntityManager
	{
		public:
		EnemyManager(Ogre::SceneManager* sceneManager);
		~EnemyManager();

		void finalize();

		EnemyPtr createEnemy(EnemyTypes type);
		EnemyPtr createEnemy(Ogre::String name, Ogre::String mesh);

		EnemyPtr getEnemy(Ogre::String name);
		bool removeEnemy(Ogre::String name);

		/* TODO : Enemy IA
		*/
		void update(const float elpasedSeconds);

	private:
		void unLoad();
		Ogre::String createUniqueId();

	private:
		int mCount;
		int mId;

		EnemyList mEnemyList;
		EnemyMap mEnemyMap;

		Ogre::SceneManager* mSceneManager;
	};
}

#endif // __ENEMY_MANAGER_H__
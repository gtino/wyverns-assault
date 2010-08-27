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
#ifndef __PROJECTILE_MANAGER_H__
#define __PROJECTILE_MANAGER_H__

#include <Ogre.h>
#include <OgreSingleton.h>

#include <boost/enable_shared_from_this.hpp>

#include "..\..\Debug\Debug.h"

#include "..\..\Utils\Utils.h"
#include "..\..\Entity\EntityManager.h"
#include "..\..\Events\EventsInterface.h"

#include "Projectile.h"

namespace WyvernsAssault
{
	typedef std::vector<ProjectilePtr> ProjectileList;
	typedef std::vector<ProjectilePtr>::iterator ProjectileListIterator;

	/**
	Class used to manage all the projectiles
	*/
	class ProjectileManager	: public Ogre::Singleton<ProjectileManager>
						, public boost::enable_shared_from_this<ProjectileManager>
						, public EntityManager
						, public EventsInterface
	{
	public:
		ProjectileManager(Ogre::SceneManager* sceneManager);
		~ProjectileManager();

		static ProjectileManager& getSingleton(void);
		static ProjectileManager* getSingletonPtr(void);

		bool initialize();
		void finalize();
		SceneManager* getSceneManager(){return this->mSceneManager;}

		ProjectilePtr createProjectile(Ogre::String name, Ogre::SceneNode* sceneNode, Ogre::Vector3 init, Ogre::Vector3 finish);
		bool removeProjectile(Ogre::String name);

		ProjectilePtr getProjectile(Ogre::String name);

		// Enable Debug Stuff
		void setDebugEnabled(bool isDebugEnabled);
		bool getDebugEnabled(){return mIsDebugEnabled;};

	private:
		Ogre::String createUniqueId();

	private:
		Ogre::SceneManager*		mSceneManager;

		ProjectileList			mProjectileList;
		int						mId;
		bool					mIsDebugEnabled;

	public:
		// ----------------
		// Events interface
		// ----------------
		EVENTS_INTERFACE()

		EVENTS_HANDLER(ProjectileFire)
		EVENTS_HANDLER(ProjectileRemove)

	};

	typedef boost::shared_ptr<ProjectileManager> ProjectileManagerPtr;
}

#endif // __PROJECTILE_MANAGER_H__
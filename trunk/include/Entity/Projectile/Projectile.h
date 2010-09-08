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
#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include <Ogre.h>

#include "..\EntityInterface.h"
#include "..\..\Physics\PhysicsInterface.h"

#include "..\OBBoxRenderable.h"

#define PROJECTILE_LIVE_TIME 10
#define PROJECTILE_SPEED	 80

namespace WyvernsAssault
{
	/**
	Class used to manage single projectile
	*/
	class Projectile : public EntityInterface, public PhysicsInterface
	{

	public:

		Projectile(Ogre::String name, Ogre::Vector3 init, Ogre::Vector3 finish);
		~Projectile();

		//
		// Entity Interface
		//
		void initializeEntity(Ogre::Entity* entity, Ogre::SceneNode* sceneNode, Ogre::SceneManager* sceneManager);
		void finalizeEntity();

		Ogre::Vector3 getFinishPoint(){return finishPoint;}

		// Enable Debug Stuff
		void setDebugEnabled(bool isDebugEnabled);
		bool getDebugEnabled(){return mIsDebugEnabled;};

		void setProjectileTimer(float seconds){ mAliveTime = seconds; }
		float getProjectileTimer(){ return mAliveTime; }

		Real getAliveTime(){ return mDuration; }

		void setProjectileDamage(Ogre::Real damage){ mDamage = damage; }
		Ogre::Real getProjectileDamage(){ return mDamage; }

		void setProjectileSpeed(Ogre::Real speed){ speed = mSpeed; }
		Ogre::Real getProjectileSpeed(){ return mSpeed; }

		void death(){ mLive = false; }
		bool isLive(){ return mLive; }

	private:

		Vector3 initPoint;
		Vector3 finishPoint;

		Ogre::Real	mDamage;
		Ogre::Real	mSpeed;
		Ogre::Real	mAliveTime;
		Ogre::Real	mDuration;
		bool		mLive;

		OBBoxRenderable* mOBBoxRenderable;
		bool mIsDebugEnabled;

	};

	typedef boost::shared_ptr<Projectile> ProjectilePtr;
}

#endif // __PROJECTILE_H__
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
#ifndef __PHYSICS_INTERFACE_H__
#define __PHYSICS_INTERFACE_H__

#include <Ogre.h>
#include "OgreOde_Core.h"

#include "Physics.h"

using namespace Ogre;

namespace WyvernsAssault
{
	typedef struct{
		OgreOde::RayGeometry* geometry;
		Ogre::Real radius;
		Ogre::Vector3 lastContact;
		bool updated;
	}PhysicsRayInfo;

	typedef std::vector<OgreOde::Body*> BodyList;

	/**
	Interface implemented by objects with physics
	*/
	class PhysicsInterface
	{
	public:
		virtual void initializePhysics(OgreOde::Body* body){mBody = body;}
		virtual void finalizePhysics(){return;} 
		virtual void updatePhysics(const float elapsedSeconds){};

		virtual void setUpdated( bool updated ) { mRayInfo.updated = updated; }
		virtual bool getUpdated(){ return mRayInfo.updated; }

		virtual void setBody( OgreOde::Body* body ){ mBody = body;}
		virtual OgreOde::Body* getBody(){ return mBody; }

		virtual BodyList* getBodyList() { return &bodys; }

		virtual size_t getGeometryId() const { return mRayInfo.geometry->getID(); }
		
		virtual void setRayInfo(PhysicsRayInfo rayInfo) { 
			mRayInfo.geometry = rayInfo.geometry;
			mRayInfo.radius = rayInfo.radius;
			mRayInfo.lastContact = Vector3(0,0,0);
			mRayInfo.updated = rayInfo.updated; // TODO : Double check!
		};
		virtual PhysicsRayInfo* getRayInfo(){ return &mRayInfo; }

	protected:
		OgreOde::Body* mBody;
		PhysicsRayInfo mRayInfo;
		BodyList bodys;

		// ----------------------
		// NEW PHYSICS INTERFACE!
		// ----------------------
	public:
		virtual void initializePhysics(PhysicsPtr physics) {mPhysics = physics;}
		//virtual void finalizePhysics(){return;} 
		//virtual void updatePhysics(const float elapsedSeconds){};

		Ogre::Real getSpeed(){return mSpeed;}
		void setSpeed(Ogre::Real speed){mSpeed = speed;}

		//Ogre::Vector3 getPosition(){return mPosition;}
		//void setPosition(Ogre::Vector3 position){mPosition = position;}

		Ogre::Quaternion getDirection(){return mDirection;}
		void setDirection(Ogre::Quaternion direction){mDirection = direction;}

		PhysicsPtr getPhysics(){return mPhysics;}
		void setPhysics(PhysicsPtr physics){mPhysics = physics;}

	private:
		Ogre::Real mSpeed;
		Ogre::Vector3 mPosition;
		Ogre::Quaternion mDirection; 
		PhysicsPtr mPhysics;
	};
}

#endif // __PHYSICS_INTERFACE_H__
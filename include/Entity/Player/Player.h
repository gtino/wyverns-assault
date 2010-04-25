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
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <Ogre.h>
#include "OgreOde_Core.h"

#include "..\EntityInterface.h"
#include "..\..\Physics\PhysicsInterface.h"
#include "..\..\Logic\LogicInterface.h"

// Player default movement speed (10 = 1 m/s)
#define SPEED	50

using namespace Ogre;

namespace WyvernsAssault
{
	/**
	Class used to manage all the players
	*/
	class Player : public EntityInterface, public PhysicsInterface
	{
	public:
		Player(Ogre::String name);
		~Player();
	
		void initialize(Ogre::Entity* mesh, Ogre::SceneNode* sceneNode, Ogre::SceneNode* autoTrackingNode);
		void finalize();

		void setPosition(Ogre::Vector3 position);

		void setLastDirection(Ogre::Vector3 dir){mLastDirection = dir;};
		Ogre::Vector3 getLastDirection(){ return mLastDirection;};

		// Player movement functions
		void move(Real x, Real y, Real z);

		// Animation functions
		void updateAnimation(float elapsedSeconds);

	private:
		Ogre::SceneNode* mAutoTrackingNode;
		Ogre::Vector3 mDirection;
		Ogre::Vector3 mLastDirection;
	};

	typedef boost::shared_ptr<Player> PlayerPtr;
}

#endif // __PLAYER_H__
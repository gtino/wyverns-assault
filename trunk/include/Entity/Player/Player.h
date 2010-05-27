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
#include <OgreTagPoint.h>
#include <ParticleUniversePlugin.h>

#include "Ogre/AnimationSystem.h"

#include "..\EntityInterface.h"
#include "..\..\Physics\PhysicsInterface.h"
#include "..\..\Logic\LogicInterface.h"

#include "..\OBBoxRenderable.h"

// Player animations number in tree
#define IDDLE		0
#define RUN			1
#define SPECIAL		2
#define ATTACKA1	3
#define ATTACKA2	4
#define ATTACKA3	5

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

		// EntityInterface
		void initializeEntity(Ogre::Entity* mesh, Ogre::SceneNode* sceneNode);
		void finalizeEntity();

		// Particle systems
		void setFireBreath(ParticleUniverse::ParticleSystem* fireBreath);

		void setPosition(Ogre::Vector3 position);

		void setLastDirection(Ogre::Vector3 dir){mLastDirection = dir;};
		Ogre::Vector3 getLastDirection(){ return mLastDirection;};

		// Player actions functions
		void move(Real x, Real y, Real z);
		// Main attack combo
		void attackA();
		// Main attack chains
		void attackA1();
		void attackA2();
		void attackA3();
		// Secondary Attack
		void attackB();
		// Special Attack
		void attackSpecial();

		bool isAttacking(){ return attacking != 0 ; }
		bool isSpecial(){ return special ; }

		// Animation functions
		void updateAnimation(float elapsedSeconds);

		// Enable Debug Stuff
		void setDebugEnabled(bool isDebugEnabled);
		bool getDebugEnabled(){return mIsDebugEnabled;};

		// Attack's grids
		void hideGrids();

	private:
		Ogre::Vector3 mLastDirection;

		// Fire breath particle system and attach point
		TagPoint*							mBreathPoint;
		ParticleUniverse::ParticleSystem*	mFireBreath;
		
		// Debug bounding box
		OBBoxRenderable* mOBBoxRenderable;
		bool mIsDebugEnabled;

	private:
		
		// Animation control variables
		bool moving;		
		bool special;
		Real attacking;
		bool continueAttacking;

		// Animation system
		tecnofreak::IAnimationSystem*		mAnimationSystem;
		tecnofreak::IParameter*				mCurrentAnimation;

	};

	typedef boost::shared_ptr<Player> PlayerPtr;
}

#endif // __PLAYER_H__
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
#include "..\..\Camera\CameraManager.h"

#include "..\OBBoxRenderable.h"

#define PLAYER1 "Player1"
#define PLAYER2 "Player2"
#define PLAYER3 "Player3"
#define PLAYER4 "Player4"

// Player animations number in tree
#define IDDLE		0
#define RUN			1
#define SPECIAL		2
#define ATTACKA1	3
#define ATTACKA2	4
#define ATTACKA3	5
#define DIE			6

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
		virtual ~Player();

		// EntityInterface
		void initializeEntity(Ogre::Entity* entity, Ogre::SceneNode* sceneNode, SceneManager* sceneManager);
		void finalizeEntity();
		void updateEntity(const float elapsedSeconds);

		// Player functions
		void setPosition(Ogre::Vector3 position);

		bool isAttacking(){ return attacking != 0 ; }
		Real wichAttack(){ return attacking; }
		bool isSpecial(){ return special ; }
		bool isDeath() { return !live; }
		bool isDying() { return timeDeath <= 3; }

		Ogre::Real getHitDamage(){return 30.0f;} // TODO : This shoul be a config parameter!
		Ogre::Real getSpecialHitDamage(){return 100.0f;} // TODO : This shoul be a config parameter!

		// Fire attack bounding box
		AxisAlignedBox getFireBox(){  return mFireMesh->getWorldBoundingBox(); }

		// Particle systems
		void setFireBreath(ParticleUniverse::ParticleSystem* fireBreath);

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
		// Die
		void Die();

		// Enable Debug Stuff
		void setDebugEnabled(bool isDebugEnabled);
		bool getDebugEnabled(){return mIsDebugEnabled;};

		// Attack's grids
		void hideGrids();

	private:
		// Fire breath particle system and attach point
		TagPoint*							mBreathPoint;
		ParticleUniverse::ParticleSystem*	mFireBreath;
		Entity*								mFireMesh;
		
		// Debug bounding box
		OBBoxRenderable* mOBBoxRenderable;
		OBBoxRenderable* mFireOBBoxRenderable;
		bool mIsDebugEnabled;

	private:
		
		// Animation control variables
		bool moving;		
		bool special;
		Real attacking;
		bool continueAttacking;
		bool live;
		Real timeDeath;

		// Animation system
		tecnofreak::IAnimationSystem*		mAnimationSystem;
		tecnofreak::IParameter*				mCurrentAnimation;

	};

	typedef boost::shared_ptr<Player> PlayerPtr;
}

#endif // __PLAYER_H__
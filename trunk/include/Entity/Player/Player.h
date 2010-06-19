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
#include "..\..\GUI\GuiWidget.h"

#include "..\OBBoxRenderable.h"

#define PLAYER1 "Player1"
#define PLAYER2 "Player2"
#define PLAYER3 "Player3"
#define PLAYER4 "Player4"

#define SPECIAL_COST 25

#define REDWYVERN_HEIGHT			20
#define REDWYVERN_SPEED				80
#define REDWYVERN_ATTACK_DAMAGE		30
#define REDWYVERN_SPECIAL_DAMAGE	250

// Player animations number in tree
#define PLAYER_IDDLE		0
#define PLAYER_RUN			1
#define PLAYER_SPECIAL		2
#define PLAYER_ATTACKA1		3
#define PLAYER_ATTACKA2		4
#define PLAYER_ATTACKA3		5
#define PLAYER_DIE			6

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

		void setMoving(bool move){moving = move;}
		void setAttacking(Real attack){attacking = attack;}

		bool isAttacking(){ return attacking != 0 ; }
		Real wichAttack(){ return attacking; }
		bool attackStart() { return newAttack; }
		void attackFinish() { newAttack = false; }
		bool isSpecial(){ return special ; }
		bool isDeath() { return !live; }
		bool isDying() { return timeDeath <= 3; }

		Ogre::Real getHitDamage(){return mAttackDamage;}
		Ogre::Real getComboHitDamage(){return mAttackDamage * 3;}
		Ogre::Real getSpecialHitDamage(){return mSpecialDamage;}

		// Fire attack bounding box
		AxisAlignedBox getFireBox(){  return mFireMesh->getWorldBoundingBox(); }

		// Particle systems
		void setFireBreath(ParticleUniverse::ParticleSystem* fireBreath);

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
		void die();

		// Enable Debug Stuff
		void setDebugEnabled(bool isDebugEnabled);
		bool getDebugEnabled(){return mIsDebugEnabled;};

		// Attack's grids
		void hideGrids();

		// Life and special
		float getLife(){ return mLife; }
		void setLife(float life);
		float getSpecial(){ return mSpecial; }		
		void setSpecial(float special);
		// Player score
		float getPoints() { return mScore; }
		void addPoints(float score){ mScore += score; }

		// Movement speed
		float getSpeed(){ return mSpeed; }

		// Gui
		void setGuiId(GuiWidgetId id){ mGuiId = id; }
		GuiWidgetId getGuiId(){ return mGuiId; }

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
		float	mMaxLife;
		float	mLife;
		float	mMaxSpecial;
		float	mSpecial;
		float	mScore;
		float	mSpeed;
		float	mAttackDamage;
		float	mSpecialDamage;

		GuiWidgetId		mGuiId;

	private:
		
		// Animation control variables
		bool moving;		
		bool special;
		Real attacking;
		bool continueAttacking;
		bool newAttack;
		bool live;
		Real timeDeath;

		// Animation system
		tecnofreak::IAnimationSystem*		mAnimationSystem;
		tecnofreak::IParameter*				mCurrentAnimation;

	};

	typedef boost::shared_ptr<Player> PlayerPtr;
}

#endif // __PLAYER_H__
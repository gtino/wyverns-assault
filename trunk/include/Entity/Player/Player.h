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

#define SPECIAL_COST 100

#define REDWYVERN_HEIGHT			24
#define REDWYVERN_SPEED				60
#define REDWYVERN_ATTACK_DAMAGE		30
#define REDWYVERN_SPECIAL_DAMAGE	250

#define PLAYER_MAX_LIFE 100.0f
#define PLAYER_MAX_SPECIAL 100.0f

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

		// Control functions
		bool isMoving(){ return moving; }
		bool isAttacking(){ return attackNumber != 0 ; }
		float wichAttack(){ return attackNumber; }
		bool attackStart() { return newAttack; }
		void attackFinished() { newAttack = false; }
		void setAttackHited(bool value){ attackHited = value; }
		bool hasAttackHited() { return attackHited; }
		bool isSpecial(){ return special; }
		bool isDeath() { return !live; }
		bool isDying() { return timeDeath <= 3; }
		void hurt(float damage){ 
			mLife -= damage * (1 / mDrunkMult); 
			if(mLife < 0) mLife = 0;
		} 

		Ogre::Real getHitDamage(){return mAttackDamage * mDrunkMult;}
		Ogre::Real getComboHitDamage(){return mAttackDamage * mDrunkMult * 3;}
		Ogre::Real getSpecialHitDamage(){return mSpecialDamage * mDrunkMult;}
		float getSpecialLength(){ return mSpecialLength; }

		// Fire attack bounding box
		AxisAlignedBox getFireBox(){ return mFireMesh->getWorldBoundingBox(); }

		Vector3 getAttackPosition(){ return mAttackNode->_getDerivedPosition(); }

		// Particle systems
		void setFireBreath(ParticleUniverse::ParticleSystem* pSystem);
		void setDust(ParticleUniverse::ParticleSystem* pSystem);

		// Movie function
		void setMoving(bool move);

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

		// Drunk when drinks beer, resistance and damage modified
		void drunk(float multiplier, float time);

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
		void doubleSpeed(){ mSpeed = mSpeed * 2; }
		void resetSpeed(){ mSpeed = REDWYVERN_SPEED; }

		// Distance from center to ground
		float getHeight(){ return mHeight; }

		// Gui
		void setGuiId(GuiWidgetId id){ mGuiId = id; }
		GuiWidgetId getGuiId(){ return mGuiId; }

		void toggleGodMode(){ mGodMode ? mGodMode = false : mGodMode = true;}
		bool isGodModeOn(){return mGodMode;}

	private:
		// Fire breath particle system and attach point
		TagPoint*							mBreathPoint;
		ParticleUniverse::ParticleSystem*	mFireBreath;
		Entity*								mFireMesh;
		// Walk dust particle system
		ParticleUniverse::ParticleSystem*	mDust;
		
		// Debug bounding box
		OBBoxRenderable* mOBBoxRenderable;
		OBBoxRenderable* mFireOBBoxRenderable;
		bool mIsDebugEnabled;
		bool mGodMode;

		SceneNode*		mAttackNode;

	private:
		float		mMaxLife;
		float		mLife;
		float		mMaxSpecial;
		float		mSpecial;
		float		mScore;
		float		mSpeed;
		float		mHeight;
		Ogre::Real	mAttackDamage;
		Ogre::Real	mSpecialDamage;
		float		mSpecialLength;
		float		mDrunkMult;
		float		mDrunkTime;

		GuiWidgetId		mGuiId;

	private:
		
		// Animation control variables
		bool	moving;		
		bool	special;
		float	attackNumber;
		bool	continueAttacking;
		bool	newAttack;
		bool	attackHited;
		bool	live;
		float	timeDeath;
		float	timeRunning;

		// Animation system
		tecnofreak::IAnimationSystem*		mAnimationSystem;
		tecnofreak::IParameter*				mCurrentAnimation;

	};

	typedef boost::shared_ptr<Player> PlayerPtr;
}

#endif // __PLAYER_H__
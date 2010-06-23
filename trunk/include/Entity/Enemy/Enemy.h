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
#ifndef __ENEMY_H__
#define __ENEMY_H__

#include <Ogre.h>

#include "..\EntityInterface.h"
#include "..\..\Physics\PhysicsInterface.h"
#include "..\..\Logic\LogicInterface.h"

#include "..\OBBoxRenderable.h"

#include "Ogre/AnimationSystem.h"

#define ENEMY_BILLBOARD_SHOW_TIME 2.0f // seconds

#define ENEMY_HEIGHT		11.0f
#define ENEMY_SLOW_SPEED	50.0f
#define ENEMY_FAST_SPEED	70.0f

#define ANIMAL_HEIGHT		2.0f
#define ANIMAL_SLOW_SPEED	30.0f
#define ANIMAL_FAST_SPEED	50.0f

// Enemies animations number in tree
#define ENEMY_IDDLE			0
#define ENEMY_RUN			1
#define ENEMY_ATTACK		2
#define ENEMY_DIE			3

namespace WyvernsAssault
{
	/**
	Class used to manage all the enemies
	*/
	class Enemy : public EntityInterface, public PhysicsInterface, public LogicInterface
	{
	public:
		/** List of enemy types */
		enum EnemyTypes
		{
			KnightA		= 0,
			KnightA2	= 1,
			KnightA3	= 2,
			KnightA4	= 3,
			KnightA5	= 4,
			KnightB		= 5,
			KnightB2	= 6,
			KnightB3	= 7,
			KnightB4	= 8,
			KnightB5	= 9,
			KnightC		= 10,
			KnightC2	= 11,
			KnightC3	= 12,
			KnightC4	= 13,
			KnightC5	= 14,
			Wizard		= 15,
			Wizard2		= 16,
			Naked		= 17,
			Soldier		= 18,					
			Peasant		= 19,
			Woman		= 20,
			Chicken		= 21,
			Cow			= 22
		};

		enum EnemyStates
		{
			Initial = 0,
			Idle = 1,
			Sleeping = 2,
			What = 3,
			Alert = 4,
			Rage = 5,
			Love = 6,
			Fear = 7,
			Magic = 8,
			Patrol = 9,
			// This is when it has been killed, but is still there spilling bool and crying!
			Dying = 10,
			// This is when the enemy has to been removed from the scene!
			Dead = 11
		};

	public:
		Enemy(Ogre::String name, Enemy::EnemyTypes type);
		~Enemy();

		//
		// Entity Interface
		//
		void initializeEntity(Ogre::Entity* mesh, Ogre::SceneNode* sceneNode, Ogre::SceneManager* sceneManager);
		void finalizeEntity();
		void updateEntity(const float elapsedSeconds);
		
		//
		// Logic Interface
		//
		void updateLogic(lua_State *L, const float elapsedSeconds);

		float getStateTimeout(){return mStateTimeout;}

		Enemy::EnemyTypes getEnemyType(){return mType;}
		Enemy::EnemyStates getEnemyState(){return mState;}

		void setTarget(SceneNode* target);
		void autoTrackTarget();
		
		Ogre::Real getHitDamage(){return mAttackDamage;}
		void hit(float damage);
		float getPoints(){ return mPoints; }

		bool isHurt();
		bool isDying();

		int mChannel;
		int getChannel(){return mChannel;}
		void setChannel(int channel){mChannel = channel;}

		float getSpeed(){return mCurrentSpeed;}
		float getHeight(){return mHeight; }

		// Enable Debug Stuff
		void setDebugEnabled(bool isDebugEnabled);
		bool getDebugEnabled(){return mIsDebugEnabled;};

		/** Animation functions */
		void setMoving(bool move){ moving = move; }
		void setAttacking(bool attack);
		bool isAttacking(){ return attacking; }
		bool attackStart(){ return newAttack; }
		void attackFinish();

		// Die functions
		bool isBurning(){ return burning; }
		void setBurning(bool status){ burning = status; }
		void setDieMesh(Ogre::Entity* entity);
		bool hasDieMesh(){ return mEntityDie != NULL; }
		bool hasDieAnimation(){ return mDieAnimation != NULL; }
		void dieSwitch();
		void dieToCamera();
		void stop();

	private:
		Enemy::EnemyTypes mType;

		BillboardSet* mBalloonSet;
		Billboard* mBalloon;

		SceneNode* mTarget;

		Enemy::EnemyStates mState;
		float mStateTimeout;
		
		float mCurrentSpeed;
		float mSlowSpeed;
		float mFastSpeed;
		float mLife;
		float mMaxLife;
		float mPoints;
		float mAttackDamage;
		float mSpecialDamage;
		float mHeight;

		Vector3 beginPatrolPoint;
		Vector3 endPatrolPoint;

		OBBoxRenderable* mOBBoxRenderable;
		bool mIsDebugEnabled;

		// Animation system
		tecnofreak::IAnimationSystem*		mAnimationSystem;
		tecnofreak::IParameter*				mCurrentAnimation;

		// State control
		bool		moving;
		bool		attacking;
		bool		newAttack;
		bool		burning;

		//Dying mesh
		Entity*		mEntityDie;
		// Die animation
		AnimationState*		mDieAnimation;		

	private:

		void setDirectionToTarget();
		void setDirectionOutTarget();

	public:
		static Enemy::EnemyTypes StringToType(Ogre::String typeStr);
	};

	typedef boost::shared_ptr<Enemy> EnemyPtr;
}

#endif // __ENEMY_H__
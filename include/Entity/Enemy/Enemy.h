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

// Enemies animations number in tree
#define ENEMY_IDDLE			0
#define ENEMY_RUN			1
#define ENEMY_ATTACK		2
#define ENEMY_SPECIAL		3

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
			Naked		= 0,
			Chicken		= 1,
			Knight		= 2,
			Wizard		= 3,
			Soldier		= 4,					
			Peasant		= 5,
			Woman		= 6,			
			Cow			= 7
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

		struct EnemyParameters
		{
			String animationTree;
			Real life;
			Real points;
			Real speed;
			Real damage;
			Real specialDamage;
			Real height;
			String dieMesh;
			String dieAnimation;
			String physicMesh;
		};

	public:
		Enemy(Ogre::String name, Enemy::EnemyTypes type, Enemy::EnemyParameters params);
		~Enemy();

		//
		// Entity Interface
		//
		void initializeEntity(Ogre::Entity* entity, Ogre::SceneNode* sceneNode, Ogre::SceneManager* sceneManager);
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
		
		Ogre::Real getHitDamage(){return mParameters.damage;}
		void hit(float damage);
		float getPoints(){ return mParameters.points; }

		bool isHurt();
		bool isDying();

		int mChannel;
		int getChannel(){return mChannel;}
		void setChannel(int channel){mChannel = channel;}

		float getSpeed(){return mParameters.speed;}
		float getHeight(){return mParameters.height; }

		// Enable Debug Stuff
		void setDebugEnabled(bool isDebugEnabled);
		bool getDebugEnabled(){return mIsDebugEnabled;};

		/** Animation functions */
		void setMoving(bool move){ moving = move; }
		void setAttacking(bool attack);
		bool isAttacking(){ return attacking; }
		bool attackStart(){ return newAttack; }
		void attackFinished(){ newAttack = false; }
		void setAttackHited(bool value){ attackHited = value; }
		bool hasAttackHited() { return attackHited; }

		// Die functions
		bool isBurning(){ return burning; }
		void setBurning(bool status){ burning = status; }
		void setDieMesh(Ogre::Entity* entity);
		bool hasDieMesh(){ return mDieMesh != NULL; }
		void setDieMaterialName(Ogre::String material){ mDieMesh->setMaterialName(material); }
		void setDieAnimation(Ogre::AnimationState* dieAnimation);
		bool hasDieAnimation(){ return mDieAnimation != NULL; }
		void dieSwitch();
		void dieToCamera();
		bool isFlying(){ return flying; }
		void stop();

	private:
		Enemy::EnemyTypes mType;

		BillboardSet* mBalloonSet;
		Billboard* mBalloon;

		SceneNode* mTarget;

		Enemy::EnemyStates mState;
		float mStateTimeout;
		
		EnemyParameters		mParameters;
		Real				mMaxLife;

		Entity*				mDieMesh;
		AnimationState*		mDieAnimation;

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
		bool		attackHited;
		bool		burning;
		bool		flying;			

	private:

		void setDirectionToTarget();
		void setDirectionOutTarget();
		void setDirectionRandom();

	public:
		static Enemy::EnemyTypes StringToType(Ogre::String typeStr);
	};

	typedef boost::shared_ptr<Enemy> EnemyPtr;
}

#endif // __ENEMY_H__
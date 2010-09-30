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
#include <OgreTagpoint.h>

#include "..\EntityInterface.h"
#include "..\..\Physics\PhysicsInterface.h"
#include "..\..\Logic\LogicInterface.h"

#include "..\OBBoxRenderable.h"

#include "Ogre/AnimationSystem.h"


#define ENEMY_BILLBOARD_SHOW_TIME 1.0f // seconds

// Enemies animations number in tree
#define ENEMY_IDDLE			0
#define ENEMY_RUN			1
#define ENEMY_ATTACK		2
#define ENEMY_SPECIAL		3
#define	WOMAN_GIVE			4

#define BOSS_IDDLE			0
#define BOSS_ATTACK1		1
#define BOSS_ATTACK2		2
#define BOSS_ATTACK3		3
#define BOSS_ATTACK4		4
#define BOSS_HIT			5

#define PHYSIC_SIZE			20

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
			Naked			= 0,
			Chicken			= 1,
			Cow				= 2,
			Woman			= 3,
			Peasant			= 4,
			Knight			= 5,			
			Wizard			= 6,
			Archer			= 7,
			BatteringRam	= 8,
			Boss			= 9
		};

		enum EnemyStates
		{
			Initial		=	0,
			Idle		=	1,
			Sleeping	=	2,
			What		=	3,
			Alert		=	4,
			Rage		=	5,
			Love		=	6,
			Fear		=	7,
			Magic		=	8,
			Fire		=	9,
			Patrol		=	10,
			Dying		=	11,	// This is when it has been killed, but is still there spilling bool and crying		
			Dead		=	12,	// This is when the enemy has to been removed from the scene
			IdleSpecial	=	13, // This is for special attack 
			Special		=	14
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
			Real attackCooldown;
			String dieMesh;
			String dieAnimation;
			Vector3 physicSize;
		};

	public:
		Enemy(Ogre::String name, Enemy::EnemyTypes type, Enemy::EnemyParameters params);
		~Enemy();

		//
		// Entity Interface
		//
		void initializeEntity(Ogre::Entity* entity, Ogre::SceneNode* sceneNode, Ogre::SceneManager* sceneManager);
		void initializeBossEntity(Ogre::Entity* entity, Ogre::SceneNode* sceneNode, Ogre::SceneManager* sceneManager);
		void finalizeEntity();
		void updateEntity(const float elapsedSeconds);
		void updateBossEntity(const float elapsedSeconds);
		
		//
		// Logic Interface
		//
		void updateLogic(lua_State *L, const float elapsedSeconds);
		void updateBossLogic(lua_State *L, const float elapsedSeconds);

		void setBossTarget(lua_State *L);

		float getStateTimeout(){return mStateTimeout;}

		Enemy::EnemyTypes getEnemyType(){return mType;}
		Enemy::EnemyStates getEnemyState(){return mState;}

		void setTarget(SceneNode* target);
		SceneNode* getTarget(){return mTarget;}
		
		// Parameters functions
		Ogre::Real getHitDamage(){return mParameters.damage;}
		Ogre::Real getSpecialHitDamage(){return mParameters.specialDamage;}
		void hit(float damage);
		float getPoints(){ return mParameters.points; }
		float getSpeed(){return mParameters.speed;}
		float getHeight(){return mParameters.height;}	
		Real getAttackTimeout(){return mAttackTimeout;}
		void setAttackTimeout(Real time){mAttackTimeout = time;}
		Real getAttackCooldown(){return mParameters.attackCooldown;}
		Vector3 getPhysicSize(){ return mParameters.physicSize; }
		void setLife(Ogre::Real life){ mParameters.life = life; }
		
		bool hasItem(){ return mHasItem; }
		void itemDroped(){ mHasItem = false; }

		// Enemy type functions
		bool isRanged();
		bool isMashable();

		// Sound functions
		int mChannel;
		int getChannel(){return mChannel;}
		void setChannel(int channel){mChannel = channel;}

		// Enable Debug Stuff
		void setDebugEnabled(bool isDebugEnabled);
		bool getDebugEnabled(){return mIsDebugEnabled;}

		// Last enemy collision
		void setLastEnemyCollision(String enemy){mLastEnemyCollision = enemy;}
		String getLastEnemyCollision(){return mLastEnemyCollision;}

		// Visibility function
		void setVisible(bool visibility);
		void hideBalloon(){ mBalloonSet->setVisible(false); }

		// Animation functions
		void setMoving(bool move){ moving = move; }

		void setAttacking(bool attack);
		bool isAttacking(){ return attacking; }

		void setSpecial(bool value){ special = value; }
		bool isSpecial(){ return special; }

		bool attackStart(){ return newAttack; }
		void attackFinished(){ newAttack = false; }
		void setAttackHited(bool value){ attackHited = value; }
		bool hasAttackHited() { return attackHited; }

		// Die functions
		bool isHurt();
		bool isDying();
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

	public:
		// Boss exclusive
		Vector3 getPhysicsPosition(int index){ return mPhysicsListPosition[index]->_getDerivedPosition(); }
		SceneNode* getPhysicsNode(int index){ return mPhysicsList[index]; }		
		int getPhysicPositionIndex(){ return mPhysicsListIndex; }
		void setPhysicPositionIndex(int value){ mPhysicsListIndex = value; }

		Real getAttackStartTime();
		int getAttackNumber(){ return mBossRandomAttack; }

	private:
		Enemy::EnemyTypes	mType;

		BillboardSet*		mBalloonSet;
		Billboard*			mBalloon;

		SceneNode*			mTarget;

		Enemy::EnemyStates	mState;
		Enemy::EnemyStates	mLastState;

		float				mStateTimeout;
		
		EnemyParameters		mParameters;
		Real				mMaxLife;

		Real				mAttackTimeout;

		Entity*				mDieMesh;
		AnimationState*		mDieAnimation;

		String			mLastEnemyCollision;



		OBBoxRenderable* mOBBoxRenderable;
		bool mIsDebugEnabled;

		// Animation system
		tecnofreak::IAnimationSystem*		mAnimationSystem;
		tecnofreak::IParameter*				mCurrentAnimation;

		// State control
		bool		moving;
		bool		attacking;
		bool		special;
		bool		newAttack;
		bool		attackHited;
		bool		burning;
		bool		flying;			
		bool		mSearchPlayer;
		bool		mHasItem;

	private:
		// Boss exclusive
		std::vector<Ogre::SceneNode*>		mPhysicsList;
		std::vector<Ogre::TagPoint*>		mPhysicsListPosition;
		int									mPhysicsListIndex;

		// Boss special control
		int			mBossRandomAttack;
		bool		mBossControlTimeHit;
		bool		mBossControlTimeSpecial;

		// Boss control animation times
		float		mAnimationTime;


		int bossHitAnimation;
		bool hitControl;



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
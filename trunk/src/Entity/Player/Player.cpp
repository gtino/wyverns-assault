#include "..\..\..\include\Entity\Player\Player.h"

using namespace WyvernsAssault;

Player::Player(Ogre::String name) 
: EntityInterface(name)
, PhysicsInterface()
, mOBBoxRenderable(0)
, mFireOBBoxRenderable(0)
, mIsDebugEnabled(false)
, mFireMesh(0)
, mMaxLife(PLAYER_MAX_LIFE)
, mLife(PLAYER_MAX_LIFE)
, mMaxSpecial(PLAYER_MAX_SPECIAL)
, mSpecial(PLAYER_MAX_SPECIAL)
, mScore(0.0f)
, mDrunkMult(1.0f)
, mDrunkTime(0.0f)
, mSpeed(REDWYVERN_SPEED)
, mHeight(REDWYVERN_HEIGHT)
, mAttackDamage (REDWYVERN_ATTACK_DAMAGE)
, mSpecialDamage (REDWYVERN_SPECIAL_DAMAGE)
, mGuiId(0)
, mGodMode(false)
{
}

Player::~Player()
{
	finalizeEntity();
}

void Player::initializeEntity(Ogre::Entity* entity, Ogre::SceneNode* sceneNode, SceneManager* sceneManager)
{
	// Always call base method before!
	EntityInterface::initializeEntity(entity, sceneNode, sceneManager);

	mFireMesh = mSceneManager->createEntity("fireMesh", "redWyvernFireCone.mesh");
	mFireMesh->setVisible(mIsDebugEnabled);

	// Physic Body
	initializePhysics(entity->getName(), Vector3(20,50,20),"OBBoxManualMaterial_Player"); 
	sceneNode->attachObject(getGeometry()->getMovableObject());


	// Animation system
	mAnimationSystem = new tecnofreak::ogre::AnimationSystem( mEntity );
	mAnimationSystem->loadAnimationTree( "data/animations/wyvern.xml" );
	mCurrentAnimation = mAnimationSystem->getParameter( "CurrentAnimation" );

	// Control variables
	moving = false;	
	special = false;
	attackNumber = 0;
	continueAttacking = false;
	newAttack = false;
	attackHited = false;
	live = true;
	timeDeath = 0;
	timeRunning = 0;

	mSpecialLength = mEntity->getAnimationState("Special")->getLength();

	// Fire bounding box
	mFireOBBoxRenderable = new OBBoxRenderable("OBBoxManualMaterial_Player");
	mFireOBBoxRenderable->setupVertices(mFireMesh->getBoundingBox());
	mFireOBBoxRenderable->setVisible(mIsDebugEnabled);

	// Hide attack's grids
	hideGrids();
}

void Player::finalizeEntity()
{
	if(mSceneManager)
	{
		if(mSceneManager->hasEntity("fireMesh"))
			mSceneManager->destroyEntity("fireMesh");

		if(mSceneManager->hasEntity("redWyvernPhysicMesh"))
			mSceneManager->destroyEntity("redWyvernPhysicMesh");
	}

	// Always call base method before!
	EntityInterface::finalizeEntity();
}

void Player::updateEntity(const float elapsedSeconds)
{
	// Activate current animation
	if ( !live ) 
	{
		mCurrentAnimation->setValue( PLAYER_DIE );
		timeDeath += elapsedSeconds;
	}
	else if( special )
	{
		mCurrentAnimation->setValue( PLAYER_SPECIAL );
	}
	else if( attackNumber  == 1 )
	{
		attackA1();
	}
	else if( attackNumber  == 2 )
	{
		attackA2();
	}
	else if( attackNumber  == 3 )
	{
		attackA3();
	}
	else if( moving )
	{
		mCurrentAnimation->setValue( PLAYER_RUN );
		if( timeRunning > 0.5 )
		{
			//if( mDust->getState() == ParticleUniverse::ParticleSystem::ParticleSystemState::PSS_STOPPED )
			mDust->start();
			timeRunning = 0;
		}
		else
			timeRunning += elapsedSeconds;
	}
	else
	{
		mCurrentAnimation->setValue( PLAYER_IDDLE );
	}	

	// Check if death animation is finish (plus 1 second)
	if( mCurrentAnimation->getFloatValue() == PLAYER_DIE )
	{
		if( mEntity->getAnimationState("Die")->getTimePosition() +  elapsedSeconds > mEntity->getAnimationState("Die")->getLength() )
			special = false;
	}
	// Check if special attack animation is finish
	else if( mCurrentAnimation->getFloatValue() == PLAYER_SPECIAL )
	{
		if ( mEntity->getAnimationState("Special")->getTimePosition() +  elapsedSeconds > mEntity->getAnimationState("Special")->getLength() )
		{
			special = false;
		}
	}
	// Check if attack animation is finish
	else if( mCurrentAnimation->getFloatValue() == PLAYER_ATTACKA1 ) 
	{
		if( mEntity->getAnimationState("AttackA_01")->getTimePosition() +  elapsedSeconds > mEntity->getAnimationState("AttackA_01")->getLength() )
		{
			hideGrids();
			if ( continueAttacking )
			{
				attackNumber = 2;
				continueAttacking = false;
				newAttack = true;
			}
			else
			{
				attackNumber = 0;
			}
		}
	}
	else if( mCurrentAnimation->getFloatValue() == PLAYER_ATTACKA2 )
	{
		if( mEntity->getAnimationState("AttackA_02")->getTimePosition() +  elapsedSeconds > mEntity->getAnimationState("AttackA_02")->getLength() )
		{
			hideGrids();
			if ( continueAttacking )
			{
				attackNumber = 3;
				continueAttacking = false;
				newAttack = true;
			}
			else
			{
				attackNumber = 0;
			}
		}
	}
	else if( mCurrentAnimation->getFloatValue() == PLAYER_ATTACKA3 )
	{
		if( mEntity->getAnimationState("AttackA_03")->getTimePosition() +  elapsedSeconds > mEntity->getAnimationState("AttackA_03")->getLength() )
		{
			hideGrids();
			attackNumber = 0;
		}
	}	

	// If player death time is bigger than its animations, dont add time
	if( timeDeath <= mEntity->getAnimationState("Die")->getLength())
	{
		mAnimationSystem->update( elapsedSeconds );
	}

	// Drunk state timer
	if( mDrunkTime > 0 )
		mDrunkTime -= elapsedSeconds;
	else
	{
		drunk(1.0f, 0.0f);
	}
}

void Player::setMoving(bool move)
{
	moving = move;
}

void Player::setFireBreath(ParticleUniverse::ParticleSystem* pSystem)
{
	// Special attack particle system (fire breath)
	mFireBreath = pSystem;

	// Attach to bone	
	mBreathPoint = mEntity->attachObjectToBone("llengua1", mFireBreath);
	
	// Physics and debug
	mEntity->attachObjectToBone("llengua1", mFireMesh);
	mEntity->attachObjectToBone("llengua1", mFireOBBoxRenderable);
}

void Player::setDust(ParticleUniverse::ParticleSystem* pSystem)
{
	mDust = pSystem;

	// Attach to chind scene node
	mSceneNode->createChildSceneNode(Vector3(0, -getHeight(), 0))->attachObject(mDust);
}

void Player::attackA()
{
	if (!special)
	{
		if ( attackNumber == 0 )
		{
  			attackNumber = 1;
			newAttack = true;
		}
		else if ( attackNumber == 1 )
		{
			continueAttacking = true;
		}
		else if ( attackNumber == 2 )
		{
			continueAttacking = true;
		}
		else if ( attackNumber == 3 )
		{
			continueAttacking = true;
		}
	}
}

void Player::attackA1()
{
	mCurrentAnimation->setValue( PLAYER_ATTACKA1 );

	// Show current attack's grids
	mEntity->getSubEntity("grid1")->setVisible(true);
	mEntity->getSubEntity("grid2")->setVisible(true);
	mEntity->getSubEntity("grid3")->setVisible(true);
}

void Player::attackA2()
{
	mCurrentAnimation->setValue( PLAYER_ATTACKA2 );

	// Show current attack's grids
	mEntity->getSubEntity("grid4")->setVisible(true);
	mEntity->getSubEntity("grid5")->setVisible(true);
	mEntity->getSubEntity("grid6")->setVisible(true);
}

void Player::attackA3()
{
	mCurrentAnimation->setValue( PLAYER_ATTACKA3 );

	// Show current attack's grids
	mEntity->getSubEntity("grid7")->setVisible(true);
	mEntity->getSubEntity("grid8")->setVisible(true);
}

void Player::attackSpecial()
{
	if( mSpecial >= SPECIAL_COST)
	{
		if( attackNumber == 0 && !newAttack && !special)
		{
			if(!mGodMode) // DEBUG : in god mode you can do all the specials you want!
			{
				mSpecial -= SPECIAL_COST;
			}

			special = true;	
			newAttack = true;
			mFireBreath->startAndStopFade(mEntity->getAnimationState("Special")->getLength());
		}
	}
}

void Player::die()
{
	// Kill player
	live = false;
	mLife = -1;
}

// Drunk when drinks beer, resistance and damage modified
void Player::drunk(float multiplier, float time)
{
	mDrunkMult = multiplier;
	mDrunkTime = time;	
}

void Player::setDebugEnabled(bool isDebugEnabled)
{
	if(mIsDebugEnabled != isDebugEnabled)
	{
		mIsDebugEnabled = isDebugEnabled;

		getGeometry()->getMovableObject()->setVisible(mIsDebugEnabled);

		if(mFireOBBoxRenderable)
			mFireOBBoxRenderable->setVisible(mIsDebugEnabled);
		
		if(mFireMesh)
			mFireMesh->setVisible(mIsDebugEnabled);
	}
}

void Player::hideGrids()
{
	mEntity->getSubEntity("grid1")->setVisible(false);
	mEntity->getSubEntity("grid2")->setVisible(false);
	mEntity->getSubEntity("grid3")->setVisible(false);
	mEntity->getSubEntity("grid4")->setVisible(false);
	mEntity->getSubEntity("grid5")->setVisible(false);
	mEntity->getSubEntity("grid6")->setVisible(false);
	mEntity->getSubEntity("grid7")->setVisible(false);
	mEntity->getSubEntity("grid8")->setVisible(false);
}

void Player::setLife(float life)
{
	mLife = life;
	if( mLife > mMaxLife)
		mLife = mMaxLife;
	if( mLife < 0)
		mLife = 0;
}

void Player::setSpecial(float special)
{
	mSpecial = special;
	if( mSpecial > mMaxSpecial)
		mSpecial = mMaxSpecial;
	if( mSpecial < 0)
		mSpecial = 0;
}

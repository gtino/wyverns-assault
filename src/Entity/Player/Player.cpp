#include "..\..\..\include\Entity\Player\Player.h"

using namespace WyvernsAssault;

Player::Player(Ogre::String name) 
: EntityInterface(name)
, PhysicsInterface()
, mOBBoxRenderable(0)
, mFireOBBoxRenderable(0)
, mIsDebugEnabled(false)
, mFireMesh(0)
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

	// Animation system
	mAnimationSystem = new tecnofreak::ogre::AnimationSystem( mEntity );
	mAnimationSystem->loadAnimationTree( "data/animations/wyvern.xml" );
	mCurrentAnimation = mAnimationSystem->getParameter( "CurrentAnimation" );

	// Control variables
	moving = false;	
	special = false;
	attacking = 0;
	continueAttacking = false;
	newAttack = false;
	live = true;
	timeDeath = 0;
	
	// Bounding Box
	mOBBoxRenderable = new OBBoxRenderable("OBBoxManualMaterial_Player");
	mOBBoxRenderable->setupVertices(mEntity->getBoundingBox());
	mOBBoxRenderable->setVisible(mIsDebugEnabled);
	mSceneNode->attachObject(mOBBoxRenderable);
	
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
	}

	// Always call base method before!
	EntityInterface::finalizeEntity();
}

void Player::updateEntity(const float elapsedSeconds)
{
	// Activate current animation
	if ( !live ) 
	{
		mCurrentAnimation->setValue( DIE );
		timeDeath += elapsedSeconds;
	}
	else if( special )
	{
		mCurrentAnimation->setValue( SPECIAL );
	}
	else if( attacking  == 1 )
	{
		attackA1();
	}
	else if( attacking  == 2 )
	{
		attackA2();
	}
	else if( attacking  == 3 )
	{
		attackA3();
	}
	else if( moving )
	{
		mCurrentAnimation->setValue( RUN );
	}
	else
	{
		mCurrentAnimation->setValue( IDDLE );
	}	

	// Check if death animation is finish (plus 1 second)
	if( mCurrentAnimation->getFloatValue() == DIE )
	{
		if( mEntity->getAnimationState("Die")->getTimePosition() +  elapsedSeconds > mEntity->getAnimationState("Die")->getLength() )
			special = false;
	}
	// Check if special attack animation is finish
	else if( mCurrentAnimation->getFloatValue() == SPECIAL )
	{
		if ( mEntity->getAnimationState("Special")->getTimePosition() +  elapsedSeconds > mEntity->getAnimationState("Special")->getLength() )
		{
			special = false;
		}
	}
	// Check if attack animation is finish
	else if( mCurrentAnimation->getFloatValue() == ATTACKA1 ) 
	{
		if( mEntity->getAnimationState("AttackA_01")->getTimePosition() +  elapsedSeconds > mEntity->getAnimationState("AttackA_01")->getLength() )
		{
			hideGrids();
			if ( continueAttacking )
			{
				attacking = 2;
				continueAttacking = false;
				newAttack = true;
			}
			else
				attacking = 0;
		}
	}
	else if( mCurrentAnimation->getFloatValue() == ATTACKA2 )
	{
		if( mEntity->getAnimationState("AttackA_02")->getTimePosition() +  elapsedSeconds > mEntity->getAnimationState("AttackA_02")->getLength() )
		{
			hideGrids();
			if ( continueAttacking )
			{
				attacking = 3;
				continueAttacking = false;
				newAttack = true;

				// Camera effect on third attack for bigger impact
				//CameraManager::getSingletonPtr()->shake(1);
			}
			else
				attacking = 0;
		}
	}
	else if( mCurrentAnimation->getFloatValue() == ATTACKA3 )
	{
		if( mEntity->getAnimationState("AttackA_03")->getTimePosition() +  elapsedSeconds > mEntity->getAnimationState("AttackA_03")->getLength() )
		{
			hideGrids();
			if ( continueAttacking )
			{
				attacking = 1;
				continueAttacking = false;
				newAttack = true;
			}
			else
				attacking = 0;
		}
	}	

	// If player death time is bigger than its animations, dont add time
	if( timeDeath <= mEntity->getAnimationState("Die")->getLength())
	{
		mAnimationSystem->update( elapsedSeconds );
	}
}

void Player::setFireBreath(ParticleUniverse::ParticleSystem* fireBreath)
{
	// Special attack particle system (fire breath)
	mFireBreath = fireBreath;

	// Attach to bone	
	mBreathPoint = mEntity->attachObjectToBone("llengua1", mFireBreath);
	
	mEntity->attachObjectToBone("llengua1", mFireMesh);
	mEntity->attachObjectToBone("llengua1", mFireOBBoxRenderable);	
}

void Player::setPosition(Ogre::Vector3 position)
{	
	mSceneNode->setPosition(position);
}

void Player::move(Real x, Real y, Real z)
{
	// Moving
	if(Vector3(x, y, z) != Vector3::ZERO)
	{
		moving = true;
	}
	// Iddle
	else
	{
		moving = false;
		attacking = 0;
	}
}

void Player::attackA()
{
	if (!special)
	{
		if ( attacking == 0 )
		{
  			attacking = 1;
			newAttack = true;
		}
		else if ( attacking == 1 )
		{
			continueAttacking = true;
		}
		else if ( attacking == 2 )
		{
			continueAttacking = true;
		}
		else if ( attacking == 3 )
		{
			continueAttacking = true;
		}
	}
}

void Player::attackA1()
{
	mCurrentAnimation->setValue( ATTACKA1 );

	// Show current attack's grids
	mEntity->getSubEntity("grid1")->setVisible(true);
	mEntity->getSubEntity("grid2")->setVisible(true);
	mEntity->getSubEntity("grid3")->setVisible(true);
}

void Player::attackA2()
{
	mCurrentAnimation->setValue( ATTACKA2 );

	// Show current attack's grids
	mEntity->getSubEntity("grid4")->setVisible(true);
	mEntity->getSubEntity("grid5")->setVisible(true);
	mEntity->getSubEntity("grid6")->setVisible(true);
}

void Player::attackA3()
{
	mCurrentAnimation->setValue( ATTACKA3 );

	// Show current attack's grids
	mEntity->getSubEntity("grid7")->setVisible(true);
	mEntity->getSubEntity("grid8")->setVisible(true);
}

void Player::attackSpecial()
{
	if( attacking == 0 && !newAttack && !special)
	{
		special = true;	
		newAttack = true;
		mFireBreath->startAndStopFade(mEntity->getAnimationState("Special")->getLength());
	}
}

void Player::Die()
{
	// Kill player
	live = false;
}

void Player::setDebugEnabled(bool isDebugEnabled)
{
	if(mIsDebugEnabled != isDebugEnabled)
	{
		mIsDebugEnabled = isDebugEnabled;
		
		if(mOBBoxRenderable)
			mOBBoxRenderable->setVisible(mIsDebugEnabled);

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
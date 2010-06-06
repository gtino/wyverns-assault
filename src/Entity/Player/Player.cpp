#include "..\..\..\include\Entity\Player\Player.h"

using namespace WyvernsAssault;

Player::Player(Ogre::String name) 
: EntityInterface()
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

void Player::initializeEntity(Ogre::Entity* mesh, Ogre::SceneNode* sceneNode, SceneManager* sceneManager)
{
	mSceneManager = sceneManager;
	mSceneNode = sceneNode;
	mMesh = mesh;

	mFireMesh = mSceneManager->createEntity("fireMesh", "redWyvernFireCone.mesh");
	mFireMesh->setVisible(mIsDebugEnabled);

	// Animation system
	mAnimationSystem = new tecnofreak::ogre::AnimationSystem( mMesh );
	mAnimationSystem->loadAnimationTree( "data/animations/wyvern.xml" );
	mCurrentAnimation = mAnimationSystem->getParameter( "CurrentAnimation" );

	// Control variables
	moving = false;	
	special = false;
	attacking = 0;
	continueAttacking = false;
	live = true;
	timeDeath = 0;
	
	// Bounding Box
	mOBBoxRenderable = new OBBoxRenderable("OBBoxManualMaterial_Player");
	mOBBoxRenderable->setupVertices(mMesh->getBoundingBox());
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
	if(mOBBoxRenderable)
	{
		delete mOBBoxRenderable;
		mOBBoxRenderable = NULL;
	}
	if(mFireOBBoxRenderable)
	{
		delete mFireOBBoxRenderable;
		mFireOBBoxRenderable = NULL;
	}
	if ( mAnimationSystem )
	{
		delete mAnimationSystem;
		mAnimationSystem = NULL;
	}
	if(mSceneNode)
	{
		mSceneManager->destroySceneNode(mSceneNode);
	}
	if(mMesh)
	{
		mSceneManager->destroyEntity(mMesh);
	}
	if(mFireMesh)
	{
		mSceneManager->destroyEntity(mFireMesh);
	}
}

void Player::setFireBreath(ParticleUniverse::ParticleSystem* fireBreath)
{
	// Special attack particle system (fire breath)
	mFireBreath = fireBreath;

	// Attach to bone	
	mBreathPoint = mMesh->attachObjectToBone("llengua1", mFireBreath);
	
	mMesh->attachObjectToBone("llengua1", mFireMesh);
	mMesh->attachObjectToBone("llengua1", mFireOBBoxRenderable);	
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
	mMesh->getSubEntity("grid1")->setVisible(true);
	mMesh->getSubEntity("grid2")->setVisible(true);
	mMesh->getSubEntity("grid3")->setVisible(true);
}

void Player::attackA2()
{
	mCurrentAnimation->setValue( ATTACKA2 );

	// Show current attack's grids
	mMesh->getSubEntity("grid4")->setVisible(true);
	mMesh->getSubEntity("grid5")->setVisible(true);
	mMesh->getSubEntity("grid6")->setVisible(true);
}

void Player::attackA3()
{
	mCurrentAnimation->setValue( ATTACKA3 );

	// Show current attack's grids
	mMesh->getSubEntity("grid7")->setVisible(true);
	mMesh->getSubEntity("grid8")->setVisible(true);
}

void Player::attackSpecial()
{
	if( attacking == 0)
	{
		special = true;	
		mFireBreath->startAndStopFade(mMesh->getAnimationState("Special")->getLength());
	}
}

void Player::Die()
{
	// Kill player
	live = false;
}

void Player::updateAnimation(float elapsedSeconds)
{
	// Check if special attack animation is finish
	if( mCurrentAnimation->getFloatValue() == SPECIAL && mMesh->getAnimationState("Special")->getTimePosition() +  elapsedSeconds > mMesh->getAnimationState("Special")->getLength() )
	{
		special = false;
	}
	// Check if attack animation is finish
	if( mCurrentAnimation->getFloatValue() == ATTACKA1 && mMesh->getAnimationState("AttackA_01")->getTimePosition() +  elapsedSeconds > mMesh->getAnimationState("AttackA_01")->getLength() )
	{
		hideGrids();
		if ( continueAttacking )
		{
			attacking = 2;
			continueAttacking = false;
		}
		else
			attacking = 0;
	}
	if( mCurrentAnimation->getFloatValue() == ATTACKA2 && mMesh->getAnimationState("AttackA_02")->getTimePosition() +  elapsedSeconds > mMesh->getAnimationState("AttackA_02")->getLength() )
	{
		hideGrids();
		if ( continueAttacking )
		{
			attacking = 3;
			continueAttacking = false;

			// Camera effect on third attack for bigger impact
			CameraManager::getSingletonPtr()->shake(1);
		}
		else
			attacking = 0;
	}
	if( mCurrentAnimation->getFloatValue() == ATTACKA3 && mMesh->getAnimationState("AttackA_03")->getTimePosition() +  elapsedSeconds > mMesh->getAnimationState("AttackA_03")->getLength() )
	{
		hideGrids();
		if ( continueAttacking )
		{
			attacking = 1;
			continueAttacking = false;
		}
		else
			attacking = 0;
	}
	// Check if death animation is finish (plus 1 second)
	if( mCurrentAnimation->getFloatValue() == DIE && mMesh->getAnimationState("Die")->getTimePosition() +  elapsedSeconds > mMesh->getAnimationState("Die")->getLength() )
	{
		special = false;
	}

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

	// If player death time is bigger than its animations, dont add time
	if( timeDeath <= mMesh->getAnimationState("Die")->getLength())
	{
		mAnimationSystem->update( elapsedSeconds );
	}
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
	mMesh->getSubEntity("grid1")->setVisible(false);
	mMesh->getSubEntity("grid2")->setVisible(false);
	mMesh->getSubEntity("grid3")->setVisible(false);
	mMesh->getSubEntity("grid4")->setVisible(false);
	mMesh->getSubEntity("grid5")->setVisible(false);
	mMesh->getSubEntity("grid6")->setVisible(false);
	mMesh->getSubEntity("grid7")->setVisible(false);
	mMesh->getSubEntity("grid8")->setVisible(false);
}
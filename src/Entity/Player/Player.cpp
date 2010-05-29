#include "..\..\..\include\Entity\Player\Player.h"

using namespace WyvernsAssault;

Player::Player(Ogre::String name) :
mOBBoxRenderable(0),
mIsDebugEnabled(false)
{
}

Player::~Player()
{
	finalizeEntity();
}

void Player::initializeEntity(Ogre::Entity* mesh, Ogre::SceneNode* sceneNode)
{
	mSceneNode = sceneNode;
	mMesh = mesh;

	// Animation system
	mAnimationSystem = new tecnofreak::ogre::AnimationSystem( mMesh );
	mAnimationSystem->loadAnimationTree( "data/animations/wyvern.xml" );
	mCurrentAnimation = mAnimationSystem->getParameter( "CurrentAnimation" );

	moving = false;	
	special = false;
	attacking = 0;
	continueAttacking = false;
	
	// Bounding Box
	mOBBoxRenderable = new OBBoxRenderable("OBBoxManualMaterial_Player");

	mOBBoxRenderable->setupVertices(mMesh->getBoundingBox());
	mOBBoxRenderable->setVisible(mIsDebugEnabled);
	mSceneNode->attachObject(mOBBoxRenderable);

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
	if ( mAnimationSystem )
	{
		delete mAnimationSystem;
		mAnimationSystem = NULL;
	}
}

void Player::setFireBreath(ParticleUniverse::ParticleSystem* fireBreath)
{
	// Special attack particle system (fire breath)
	mFireBreath = fireBreath;

	// Attach to bone	
	mBreathPoint = mMesh->attachObjectToBone("llengua1", mFireBreath);
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

	// Activate current animation
	if( special )
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
	
	mAnimationSystem->update( elapsedSeconds );	
}

void Player::setDebugEnabled(bool isDebugEnabled)
{
	if(mIsDebugEnabled != isDebugEnabled)
	{
		mIsDebugEnabled = isDebugEnabled;
		
		if(mOBBoxRenderable)
			mOBBoxRenderable->setVisible(mIsDebugEnabled);
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
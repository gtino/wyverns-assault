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
	if( attacking == 0)
	{
  		attacking = 1;
	}
	else if( attacking == 1)
	{
  		attacking = 2;
	}
	else if( attacking == 2)
	{
  		attacking = 3;
	}
	else
	{
		attacking = 1;
	}
}

void Player::attackA1()
{
	/*mAttackA1->setWeight(1);
	mAttackA1->setEnabled(true);

	// Show current attack's grids
	mMesh->getSubEntity("grid1")->setVisible(true);
	mMesh->getSubEntity("grid2")->setVisible(true);
	mMesh->getSubEntity("grid3")->setVisible(true);*/
}

void Player::attackA2()
{
	/*mAttackA2->setWeight(1);
	mAttackA2->setEnabled(true);

	// Show current attack's grids
	mMesh->getSubEntity("grid4")->setVisible(true);
	mMesh->getSubEntity("grid5")->setVisible(true);
	mMesh->getSubEntity("grid6")->setVisible(true);*/
}

void Player::attackA3()
{
	/*mAttackA3->setWeight(1);
	mAttackA3->setEnabled(true);

	// Show current attack's grids
	mMesh->getSubEntity("grid7")->setVisible(true);
	mMesh->getSubEntity("grid8")->setVisible(true);*/
}

void Player::attackSpecial()
{
	special = true;	
	mFireBreath->startAndStopFade(mMesh->getAnimationState("Special")->getLength() + 0.2);
}

void Player::updateAnimation(float elapsedSeconds)
{
	/*if(attacking != 0)
	{
		if(attacking == 1 || mAttackA1->getEnabled())
		{
			attackA1();
			mAttackA1->addTime(elapsedSeconds);
		}
		if((attacking == 2 && !mAttackA1->getEnabled()) || mAttackA2->getEnabled())
		{
			attackA2();
			mAttackA2->addTime(elapsedSeconds);
		}
		if((attacking == 3 && !mAttackA1->getEnabled() && !mAttackA2->getEnabled()) || mAttackA3->getEnabled())
		{
			attackA3();
			mAttackA3->addTime(elapsedSeconds);
		}		

		mRun->setWeight(0);		
		mSpecial->setWeight(0);		
		mIddle->setWeight(0);
	}
	else if(moving)
	{
		mRun->addTime(elapsedSeconds);

		mRun->setWeight(1);
		mAttackA1->setWeight(0);
		mSpecial->setWeight(0);		
		mIddle->setWeight(0);
	}
	else if(special)
	{	
		mSpecial->addTime(elapsedSeconds);

		mRun->setWeight(0);
		mAttackA1->setWeight(0);
		mSpecial->setWeight(0.9);
		mIddle->setWeight(0);
	}
	else
	{
		mIddle->addTime(elapsedSeconds);
		
		mRun->setWeight(0);
		mAttackA1->setWeight(0);
		mSpecial->setWeight(0);
		mIddle->setWeight(1);
	}

	// Attack animation finished
	if(attacking != 0)
	{
		if(mAttackA1->getEnabled() && mAttackA1->getTimePosition() + elapsedSeconds > mAttackA1->getLength())
		{
			mAttackA1->setEnabled(false);
			mAttackA1->setWeight(0);
			mAttackA1->setTimePosition(0);
			if(attacking == 1) attacking = 0;
			hideGrids();
		}
		else if(mAttackA2->getEnabled() && mAttackA2->getTimePosition() + elapsedSeconds > mAttackA2->getLength())
		{
			mAttackA2->setEnabled(false);
			mAttackA2->setWeight(0);
			mAttackA2->setTimePosition(0);
			if(attacking == 2) attacking = 0;
			hideGrids();
		}
		else if(mAttackA3->getEnabled() && mAttackA3->getTimePosition() + elapsedSeconds > mAttackA3->getLength())
		{
 			mAttackA3->setEnabled(false);
			mAttackA3->setWeight(0);
			mAttackA3->setTimePosition(0);
			if(attacking == 3) attacking = 0;
			hideGrids();
		}
	}

	// Special attack animation finished
	if(mSpecial->getTimePosition() + elapsedSeconds > mSpecial->getLength())
	{
		special = false;
		mSpecial->setEnabled(false);
	}*/

	if( mMesh->getAnimationState("Special")->getTimePosition() +  elapsedSeconds > mMesh->getAnimationState("Special")->getLength())
	{
		special = false;
	}
	if(mMesh->getAnimationState("AttackA_01")->getTimePosition() +  elapsedSeconds > mMesh->getAnimationState("AttackA_01")->getLength()
		&& attacking == 1)
	{
		attacking = 0;
		hideGrids();
	}
	else if(mMesh->getAnimationState("AttackA_02")->getTimePosition() +  elapsedSeconds > mMesh->getAnimationState("AttackA_02")->getLength()
		&& attacking == 2)
	{
		attacking = 0;
		hideGrids();
	}
	else if(mMesh->getAnimationState("AttackA_03")->getTimePosition() +  elapsedSeconds > mMesh->getAnimationState("AttackA_03")->getLength()
		&& attacking == 3)
	{
		attacking = 0;
		hideGrids();
	}

	if( moving )
	{
		mCurrentAnimation->setValue( RUN );
	}
	else if( special )
	{
		mCurrentAnimation->setValue( SPECIAL );
	}
	else if( attacking  == 1 )
	{
		mCurrentAnimation->setValue( ATTACKA1 );

		mMesh->getSubEntity("grid1")->setVisible(true);
		mMesh->getSubEntity("grid2")->setVisible(true);
		mMesh->getSubEntity("grid3")->setVisible(true);
	}
	else if( attacking  == 2 )
	{
		mCurrentAnimation->setValue( ATTACKA2 );
		mMesh->getSubEntity("grid4")->setVisible(true);
		mMesh->getSubEntity("grid5")->setVisible(true);
		mMesh->getSubEntity("grid6")->setVisible(true);

	}
	else if( attacking  == 3 )
	{
		mCurrentAnimation->setValue( ATTACKA3 );

		mMesh->getSubEntity("grid7")->setVisible(true);
		mMesh->getSubEntity("grid8")->setVisible(true);
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
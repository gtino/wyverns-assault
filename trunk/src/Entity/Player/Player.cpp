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

	// Animations
	mIddle = mMesh->getAnimationState("Iddle_02");
	mIddle->setEnabled(true);
	mRun = mMesh->getAnimationState("Run");
	mRun->setEnabled(true);
	mAttackA = mMesh->getAnimationState("AttackA_01");
	mAttackA->setEnabled(true);
	mSpecial = mMesh->getAnimationState("Special");
	mSpecial->setEnabled(true);

	mesh->getSkeleton()->setBlendMode(SkeletonAnimationBlendMode::ANIMBLEND_CUMULATIVE);

	moving = false;
	attacking = false;
	special = false;
	mDirection = Vector3::ZERO;
	
	// Bounding Box
	mOBBoxRenderable = new OBBoxRenderable("OBBoxManualMaterial_Player");

	mOBBoxRenderable->setupVertices(mMesh->getBoundingBox());
	mOBBoxRenderable->setVisible(mIsDebugEnabled);
	mSceneNode->attachObject(mOBBoxRenderable);
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
	else if(moving)
	{
		moving = false;
		mIddle->setTimePosition(0);
	}
}

void Player::attackA()
{
	attacking = true;
	mAttackA->setEnabled(true);
}

void Player::attackSpecial()
{
	special = true;
	mSpecial->setEnabled(true);
	mFireBreath->startAndStopFade(mSpecial->getLength());
}

void Player::updateAnimation(float elapsedSeconds)
{
	if(moving && attacking)
	{
		mRun->addTime(elapsedSeconds);
		mAttackA->addTime(elapsedSeconds);

		mRun->setWeight(0.2);
		mAttackA->setWeight(1);
		mSpecial->setWeight(0);
		mIddle->setWeight(0);
	}
	else if(moving)
	{
		mRun->addTime(elapsedSeconds);

		mRun->setWeight(1);
		mAttackA->setWeight(0);
		mSpecial->setWeight(0);		
		mIddle->setWeight(0);
	}
	else if(attacking)
	{
		mAttackA->addTime(elapsedSeconds);

		mRun->setWeight(0);
		mAttackA->setWeight(1);
		mSpecial->setWeight(0);		
		mIddle->setWeight(0);
	}
	else if(special)
	{	
		mSpecial->addTime(elapsedSeconds);

		mRun->setWeight(0);
		mAttackA->setWeight(0);
		mSpecial->setWeight(0.9);
		mIddle->setWeight(0);
	}
	else
	{
		mIddle->addTime(elapsedSeconds);
		
		mRun->setWeight(0);
		mAttackA->setWeight(0);
		mSpecial->setWeight(0);
		mIddle->setWeight(1);
	}

	// Attack animation finished
	if(mAttackA->getTimePosition() + elapsedSeconds > mAttackA->getLength())
	{
		attacking = false;
		mAttackA->setEnabled(false);
		
	}
	// Special attack animation finished
	if(mSpecial->getTimePosition() + elapsedSeconds > mSpecial->getLength())
	{
		special = false;
		mSpecial->setEnabled(false);
	}
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
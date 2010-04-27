#include "..\..\..\include\Entity\Player\Player.h"

using namespace WyvernsAssault;

Player::Player(Ogre::String name)
{
}

Player::~Player()
{
	finalize();
}

void Player::initialize(Ogre::Entity* mesh, Ogre::SceneNode* sceneNode)
{
	mSceneNode = sceneNode;
	mMesh = mesh;

	// Animations
	mIddle = mesh->getAnimationState("Iddle_01");
	mIddle->setEnabled(true);
	mRun = mesh->getAnimationState("Run");
	mRun->setEnabled(true);
	mAttackA = mesh->getAnimationState("Attack_A_01");
	mAttackA->setEnabled(true);
	//mAttackA->setLoop(false);

	attacking = false;
	mDirection = Vector3::ZERO;
}

void Player::finalize()
{
}

void Player::setPosition(Ogre::Vector3 position)
{	
	mSceneNode->setPosition(position);
}

void Player::move(Real x, Real y, Real z)
{
	// Direction vector
	mDirection = Vector3(x, y, z);

	if(attacking && mDirection != Vector3::ZERO)
	{
 		mIddle->setWeight(0);
		mAttackA->setWeight(1);
		mRun->setWeight(0.1);
	}
	else if(attacking)
	{
		mIddle->setWeight(0);
		mAttackA->setWeight(1);
		mRun->setWeight(0);
	}
	else if(mDirection != Vector3::ZERO)
	{
		mIddle->setWeight(0);
		mAttackA->setWeight(0);
		mRun->setWeight(1);
	}
	else
	{
		mIddle->setWeight(1);
		mAttackA->setWeight(0);
		mRun->setWeight(0);
	}
}

void Player::attackA()
{
	attacking = true;

	mIddle->setWeight(0);
	mAttackA->setWeight(1);
	mRun->setWeight(0);	
}

void Player::updateAnimation(float elapsedSeconds)
{
	mRun->addTime(elapsedSeconds);
	if(attacking)
	{
		mAttackA->addTime(elapsedSeconds);
	}
	else
	{
		mIddle->addTime(elapsedSeconds);
	}
	if(mAttackA->getTimePosition() + elapsedSeconds > mAttackA->getLength())
	{
		attacking = false;
	}
}
#include "..\..\..\include\Entity\Player\Player.h"

using namespace WyvernsAssault;

Player::Player(Ogre::String name)
{
}

Player::~Player()
{
	finalize();
}

void Player::initialize(String name, Ogre::Entity* mesh, Ogre::SceneNode* sceneNode, Ogre::SceneManager* sceneManager)
{
	mSceneNode = sceneNode;
	mMesh = mesh;
	mSceneManager = sceneManager;

	// Animations
	mIddle = mesh->getAnimationState("Iddle_01");
	mIddle->setEnabled(true);
	mRun = mesh->getAnimationState("Run");
	mRun->setEnabled(true);
	mAttackA = mesh->getAnimationState("Attack_A_01");
	mAttackA->setEnabled(true);
	mSpecial = mesh->getAnimationState("Special");
	mSpecial->setEnabled(true);

	mesh->getSkeleton()->setBlendMode(SkeletonAnimationBlendMode::ANIMBLEND_CUMULATIVE);

	moving = false;
	attacking = false;
	special = false;
	mDirection = Vector3::ZERO;

	// Special attack particle system (fire breath)
	mFireBreath = ParticleUniverse::ParticleSystemManager::getSingletonPtr()->createParticleSystem("fireBreath", "WyvernsAssault/DragonBreath", mSceneManager);
	// Attach to bone	
	Entity* playerMesh = mSceneManager->getEntity(name);
	mBreathPoint = playerMesh->attachObjectToBone("bone25", mFireBreath);
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
	// Moving
	if(Vector3(x, y, z) != Vector3::ZERO)
	{
		moving = true;
	}
	// Iddle
	else
	{
		moving = false;
	}
}

void Player::attackA()
{
	attacking = true;
}

void Player::attackSpecial()
{
	special = true;
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
	else if(moving && special)
	{
		mRun->addTime(elapsedSeconds);
		mSpecial->addTime(elapsedSeconds);

		mRun->setWeight(0.2);
		mAttackA->setWeight(0);
		mSpecial->setWeight(0.9);		
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
		
	}
	// Special attack animation finished
	if(mSpecial->getTimePosition() + elapsedSeconds > mSpecial->getLength())
	{
		special = false;
	}
}
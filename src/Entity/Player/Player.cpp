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
	mAnimationState = mesh->getAnimationState("Iddle_01");
	mAnimationState->setEnabled(true);
	mAnimationState->setLoop(true);
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
	mDirection = Vector3(x, y, -z);

	if (mDirection != Vector3::ZERO)
	{
		mAnimationState = mMesh->getAnimationState("Run");
		mAnimationState->setEnabled(true);
		mAnimationState->setLoop(true);
	}
	else
	{
		mAnimationState = mMesh->getAnimationState("Iddle_01");
		mAnimationState->setEnabled(true);
		mAnimationState->setLoop(true);
	}
}

void Player::updateAnimation(float elapsedSeconds)
{
	mAnimationState->addTime(elapsedSeconds);
}
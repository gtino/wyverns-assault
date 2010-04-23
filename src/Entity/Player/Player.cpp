#include "..\..\..\include\Entity\Player\Player.h"

using namespace WyvernsAssault;

Player::Player(Ogre::String name) :
mMesh(NULL),
mSceneNode(NULL)
{
}

Player::~Player()
{
	finalize();
}

void Player::initialize(Ogre::Entity* mesh, Ogre::SceneNode* sceneNode, Ogre::SceneNode* sceneNode_model,Ogre::SceneNode* autoTrackingNode)
{
	mSceneNode = sceneNode;
	mSceneNode_model = sceneNode_model;
	mMesh = mesh;
	mAutoTrackingNode = autoTrackingNode;

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
	mAutoTrackingNode->setPosition(position * Vector3::UNIT_Y);
}

void Player::move(Real x, Real y, Real z)
{
	// Direction vector
	Vector3 direction = Vector3(x, y, -z);

	if (direction != Vector3::ZERO)
	{
		// Change autotrack axis for facing movement direction
		mSceneNode->setAutoTracking(true, mAutoTrackingNode, direction);

		// Translate forward (allways move forward into facing direction)
		mSceneNode->translate(Vector3(0, 0, 1) * SPEED, Ogre::Node::TransformSpace::TS_LOCAL);

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
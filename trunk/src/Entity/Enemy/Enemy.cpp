#include "..\..\..\include\Entity\Enemy\Enemy.h"

using namespace WyvernsAssault;

Enemy::Enemy(Ogre::String name) :
mMesh(NULL),
mSceneNode(NULL)
{
}

Enemy::~Enemy()
{
}

void Enemy::initialize(Ogre::Entity* mesh, Ogre::SceneNode* sceneNode)
{
	mSceneNode = sceneNode;
	mMesh = mesh;

	// Animations
	//mAnimationState = mMesh->getAnimationState("Iddle_01");
	//mAnimationState->setEnabled(true);
	//mAnimationState->setLoop(true);
}

void Enemy::finalize()
{
}

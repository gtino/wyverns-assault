#include "..\..\..\include\Entity\Player\PlayerManager.h"

using namespace WyvernsAssault;

PlayerManager::PlayerManager()
{
	initializeVariables();
}

PlayerManager::~PlayerManager()
{

}

void PlayerManager::initializeVariables()
{
	mSceneManager		=	NULL;	
	mPlayerMesh			=	NULL;
	mPlayerSceneNode	=	NULL;
}

void PlayerManager::initialize(String name, String mesh, SceneManager *levelSceneManager, Vector3 position)
{
	mSceneManager = levelSceneManager;

	// Create player
	mPlayerMesh = mSceneManager->createEntity(name, mesh);
	mPlayerSceneNode = mSceneManager->getRootSceneNode()->createChildSceneNode(name);
	mPlayerSceneNode->attachObject(mPlayerMesh);

	// Positioning
	mPlayerSceneNode->setPosition(position);
	mPlayerSceneNode->setScale(4, 4, 4);

	// Center node for player movement and camera targeting
	SceneNode* centerNode = mSceneManager->getRootSceneNode()->createChildSceneNode("Center");
	centerNode->setPosition(position * Vector3::UNIT_Y);
	/*Entity* cube = mSceneManager->createEntity("CenterCube", "axes.mesh");
	centerNode->attachObject(cube);	
	centerNode->setScale(100, 100, 100);*/

	mPlayerSceneNode->setAutoTracking(true, centerNode, Vector3::UNIT_X);
	
	// Animations
	/*mPlayerAnimation = mPlayerMesh->getAnimationState("Run");
	mPlayerAnimation->setEnabled(true);*/
}

void PlayerManager::finalize()
{
	initializeVariables();
}


void PlayerManager::move(Real x, Real y, Real z)
{
	// Direction vector
	Vector3 mDirection = Vector3(x, y, -z);

	// Change autotrack axis for facing movement direction
	mPlayerSceneNode->setAutoTracking(true, mSceneManager->getSceneNode("Center"), mDirection);

	// Translate forward (allways move forward into facing direction)
	mPlayerSceneNode->translate(Vector3(0, 0, 1)*SPEED, Ogre::Node::TransformSpace::TS_LOCAL);

	/*
	// Player rotation to face current direction
	Vector3 mDirection = Vector3(x, y, z);
	Vector3 src = mPlayerSceneNode->getOrientation() * Vector3::UNIT_Z;
	src.normalise();
	if ((1.0f + src.dotProduct(mDirection)) < 0.0001f) 
	{
		mPlayerSceneNode->yaw(Degree(180));
	}
	else
	{
		Ogre::Quaternion quat = src.getRotationTo(mDirection);
		mPlayerSceneNode->rotate(quat);
	}
	// Player move
	mPlayerSceneNode->translate((mDirection*SPEED)*src);
	*/
}

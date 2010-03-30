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
}

void PlayerManager::finalize()
{
	initializeVariables();
}


void PlayerManager::move(Real x, Real y, Real z)
{
	// Player rotation to face current direction
	Vector3 mDirection = Vector3(x, y, z);
	Vector3 src = mPlayerSceneNode->getOrientation() * Vector3::UNIT_Z;
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
	mPlayerSceneNode->translate(SPEED * x, SPEED * y, SPEED * z);
}

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
	mPlayerLookNode		=	NULL;
	mPlayerTargetNode	=	NULL;
}

void PlayerManager::initialize(String name, String mesh, SceneManager *levelSceneManager, Vector3 position)
{
	mSceneManager = levelSceneManager;

	// Create player entity
	mPlayerMesh = mSceneManager->createEntity(name, mesh);

	// Create nodes
	mPlayerSceneNode = mSceneManager->getRootSceneNode()->createChildSceneNode(name);
	mPlayerTargetNode = mSceneManager->getRootSceneNode()->createChildSceneNode(name + "_target");
	mPlayerLookNode = mPlayerSceneNode->createChildSceneNode(name + "_look", Vector3(0, 0, -OFFSET));

	// Attach player entity mesh to player node
	mPlayerSceneNode->attachObject(mPlayerMesh);

	// Positioning
	mPlayerSceneNode->setPosition(position);
	mPlayerTargetNode->setPosition(mPlayerLookNode->_getDerivedPosition());
	
	// PlayerNode looks at playerTargetNode
	mPlayerSceneNode->setAutoTracking(true, mPlayerTargetNode);

	// Temporal scale and rotate - DELETE
	mPlayerSceneNode ->scale(0.2,0.2,0.2);
}

void PlayerManager::finalize()
{
	initializeVariables();
}


void PlayerManager::move(Real x, Real y, Real z)
{
	mPlayerSceneNode->translate(SPEED * x, SPEED * y, SPEED * z);
	mPlayerTargetNode->setPosition(mPlayerLookNode->_getDerivedPosition());
}

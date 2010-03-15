#include "..\..\..\include\Entity\Player\PlayerManager.h"

using namespace WyvernsAssault;

PlayerManager::PlayerManager()
{
	initializeVariables();
}

PlayerManager::~PlayerManager()
{

}

void PlayerManager::setPosition(Ogre::Vector3 position)
{
	// Constrain movement outside boundaries - TEMP
	if (position.x < 0) position.x = 0;
	if (position.z > -10) position.z = -10;
	playerSceneNode->setPosition(position);
};

void PlayerManager::setRotation(Ogre::Radian x, Ogre::Radian y, Ogre::Radian z)
{
	if (x.valueRadians() != 0) playerSceneNode->roll(x);
	if (y.valueRadians() != 0) playerSceneNode->yaw(y);
	if (z.valueRadians() != 0) playerSceneNode->pitch(z);
}

void PlayerManager::initializeVariables()
{
	mSceneMgr = NULL;
	playerMesh = NULL;
	playerSceneNode = NULL;
}

void PlayerManager::initialize(Ogre::String name, Ogre::String mesh, Ogre::SceneManager *levelSceneManager, Ogre::Vector3 position)
{

	mSceneMgr = levelSceneManager;
	playerMesh = mSceneMgr->createEntity(name,mesh);
	playerSceneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	playerSceneNode->attachObject(playerMesh);
	playerSceneNode->scale(0.15,0.15,0.15);
	playerSceneNode->yaw(Ogre::Radian(Ogre::Degree(90)));
	setPosition(position);
}

void PlayerManager::finalize()
{
//	playerSceneNode->detachAllObjects();
//	mSceneMgr->getRootSceneNode()->removeAndDestroyChild(playerSceneNode->getName());
//	mSceneMgr->destroyEntity(playerMesh);
	initializeVariables();
}

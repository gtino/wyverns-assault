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
	playerSceneNode->scale(0.5,0.5,0.5);
	playerSceneNode->yaw(Ogre::Radian(0.90));
	setPosition(position);
}

void PlayerManager::finalize()
{
	playerSceneNode->detachAllObjects();
	mSceneMgr->getRootSceneNode()->removeAndDestroyChild(playerSceneNode->getName());
	mSceneMgr->destroyEntity(playerMesh);
	initializeVariables();
}

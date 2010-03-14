#include "..\..\..\include\Entity\Enemy\EnemyManager.h"

using namespace WyvernsAssault;

EnemyManager::EnemyManager()
{
	initializeVariables();
}

EnemyManager::~EnemyManager()
{

}

void EnemyManager::initializeVariables()
{
	mSceneMgr = NULL;
	enemyMesh = NULL;
	enemySceneNode = NULL;
}

void EnemyManager::initialize(Ogre::String name, Ogre::String mesh, Ogre::SceneManager *levelSceneManager, Ogre::Vector3 position)
{

	mSceneMgr = levelSceneManager;
	enemyMesh = mSceneMgr->createEntity(name,mesh);
	enemySceneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	enemySceneNode->attachObject(enemyMesh);
	//enemySceneNode->scale(0.5,0.5,0.5);
	//enemySceneNode->yaw(Ogre::Radian(0.90));
	setPosition(position);
}

void EnemyManager::finalize()
{
	enemySceneNode->detachAllObjects();
	mSceneMgr->getRootSceneNode()->removeAndDestroyChild(enemySceneNode->getName());
	mSceneMgr->destroyEntity(enemyMesh);
	initializeVariables();
}

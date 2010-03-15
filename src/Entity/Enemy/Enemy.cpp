#include "..\..\..\include\Entity\Enemy\Enemy.h"

using namespace WyvernsAssault;

Enemy::Enemy()
{
	initializeVariables();
}

Enemy::~Enemy()
{

}

void Enemy::initializeVariables()
{
	mSceneMgr = NULL;
	enemyMesh = NULL;
	enemySceneNode = NULL;
}

void Enemy::initialize(Ogre::String name, Ogre::String mesh, Ogre::SceneManager *levelSceneManager, Ogre::Vector3 position)
{

	mSceneMgr = levelSceneManager;
	enemyMesh = mSceneMgr->createEntity(name,mesh);
	enemySceneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	enemySceneNode->attachObject(enemyMesh);
	//enemySceneNode->scale(0.2,0.2,0.2);
	enemySceneNode->yaw(Ogre::Radian(Ogre::Degree(-90)));
	setPosition(position);
}

void Enemy::finalize()
{
	enemySceneNode->detachAllObjects();
	mSceneMgr->getRootSceneNode()->removeAndDestroyChild(enemySceneNode->getName());
	mSceneMgr->destroyEntity(enemyMesh);
	initializeVariables();
}

#include "..\..\..\include\Entity\Enemy\EnemyManager.h"

using namespace WyvernsAssault;

EnemyManager::EnemyManager(Ogre::SceneManager* sceneManager) :
mCount(0),
mId(0)
{
	mSceneManager = sceneManager;
}

EnemyManager::~EnemyManager()
{

}

void EnemyManager::finalize()
{
	this->unLoad();
}

void EnemyManager::unLoad()
{
	mEnemyList.clear();
}

EnemyPtr EnemyManager::createEnemy(EnemyTypes type)
{
	Ogre::String mesh;

	switch(type)
	{
	case Naked:
		mesh = Ogre::String("EnemyNaked.mesh");
		break;
	default:
		mesh = Ogre::String("EnemyNaked.mesh");
		break;
	}

	Ogre::String name = createUniqueId();

	return createEnemy(name, mesh);
}

EnemyPtr EnemyManager::createEnemy(Ogre::String name, Ogre::String mesh)
{
	Ogre::Entity* enemyMesh = mSceneManager->createEntity(name,mesh);
	Ogre::SceneNode* enemySceneNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
	
	enemySceneNode->attachObject(enemyMesh);
	enemySceneNode->yaw(Ogre::Radian(Ogre::Degree(-90)));

	EnemyPtr enemy = EnemyPtr(new Enemy(name));
	enemy->initialize();

	mEnemyList.push_back(enemy);
	mEnemyMap[name] = enemy;

	mCount++;

	return enemy;
}

Ogre::String EnemyManager::createUniqueId()
{
	Ogre::StringStream countStrStr;
	countStrStr << mId;

	Ogre::String uniqueId = "Enemy_" + countStrStr.str();

	mId++;

	return uniqueId;
}

EnemyPtr EnemyManager::getEnemy(Ogre::String name)
{
	return mEnemyMap[name];
}

bool EnemyManager::removeEnemy(Ogre::String name)
{
	mEnemyMap.erase(name);

	mSceneManager->destroyEntity(name);//getRootSceneNode()->removeChild(name);

	return true;
}

/* 
* TODO
*/
void EnemyManager::update(const float elpasedSeconds)
{
	
}
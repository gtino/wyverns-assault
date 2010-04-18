#include "..\..\..\include\Entity\Enemy\EnemysManager.h"

using namespace WyvernsAssault;

EnemysManager::EnemysManager(Ogre::SceneManager* sceneManager) :
mCount(0)
{
	mSceneManager = sceneManager;
}

EnemysManager::~EnemysManager()
{

}

void EnemysManager::finalize()
{
	this->unLoad();
}

void EnemysManager::unLoad()
{
	mEnemyList.clear();
}

Enemy* EnemysManager::createEnemy(EnemyTypes type)
{
	Enemy* pEnemy = new Enemy();

	Ogre::StringStream typeStrStr;
	typeStrStr << type;

	Ogre::StringStream countStrStr;
	countStrStr << mCount;

	Ogre::String mesh = Ogre::String("EnemyNaked.mesh");

	//switch(type)
	//{
	//case Naked:
	//	mesh = String("EnemyNaked.mesh");
	//	break;
	//default:
	//	break;
	//}

	Ogre::String name = "Enemy_" + typeStrStr.str() + "." + countStrStr.str();

	pEnemy->initialize(name, mesh, mSceneManager, Ogre::Vector3(0,0,0));

	mCount++;

	mEnemyList.push_back(pEnemy);
	mEnemyMap[name] = pEnemy;

	return pEnemy;
}

Enemy* EnemysManager::getEnemy(Ogre::String name)
{
	return mEnemyMap[name];
}

bool EnemysManager::removeEnemy(Ogre::String name)
{
	mSceneManager->destroyEntity(name);//getRootSceneNode()->removeChild(name);

	return true;
}

EnemyList& EnemysManager::getList()
{
	return mEnemyList;
}

/* TODO
*/
void EnemysManager::processEnemyIA()
{
	
}
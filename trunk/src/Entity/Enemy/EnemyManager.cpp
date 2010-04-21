#include "..\..\..\include\Entity\Enemy\EnemyManager.h"

using namespace WyvernsAssault;

// BEGIN SINGLETON
template<> EnemyManager* Ogre::Singleton<EnemyManager>::ms_Singleton = 0;
EnemyManager* EnemyManager::getSingletonPtr(void)
{
    return ms_Singleton;
}
EnemyManager& EnemyManager::getSingleton(void)
{  
    assert( ms_Singleton );  return ( *ms_Singleton );  
}
// END SINGLETON

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

// --------------------------------
// Lua Enemy Lib
// --------------------------------
LUA_BEGIN_BINDING(EnemyManager::enemylib)
LUA_BIND("create", EnemyManager::createEnemy)
LUA_BIND("getPosition", EnemyManager::getEnemyPosition)
LUA_BIND("setPosition", EnemyManager::setEnemyPosition)
LUA_BIND("setState", EnemyManager::setEnemyState)
LUA_BIND("remove", EnemyManager::removeEnemy)
LUA_END_BINDING()

int EnemyManager::createEnemy(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 1, the enemy type

	int type = luaL_checkint(L, 1);

	EnemyPtr enemy = EnemyManager::getSingleton().createEnemy((EnemyTypes)type);

	lua_pushstring(L,enemy->getName().c_str());

	/* return the number of results */
	return 1;
}

int EnemyManager::getEnemyPosition(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 1;
}

int EnemyManager::setEnemyPosition(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 1;
}

int EnemyManager::setEnemyState(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 1;
}

int EnemyManager::removeEnemy(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 1;
}

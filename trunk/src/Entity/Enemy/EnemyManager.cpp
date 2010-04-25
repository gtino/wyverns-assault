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
	finalize();
}

void EnemyManager::initialize()
{
}

void EnemyManager::finalize()
{
	mEnemyList.clear();
	mEnemyMap.clear();
}

EnemyPtr EnemyManager::createEnemy(EnemyTypes type)
{
	Ogre::String mesh;

	switch(type)
	{
	case Naked:
		mesh = Ogre::String("naked.mesh");
		break;
	case Wizard:
		mesh = Ogre::String("wizard.mesh");
		break;
	default:
		mesh = Ogre::String("naked.mesh");
		break;
	}

	Ogre::String name = createUniqueId();

	return createEnemy(type, name, mesh);
}

EnemyPtr EnemyManager::createEnemy(EnemyTypes type, Ogre::String name, Ogre::String mesh)
{
	// Enemy name == Mesh Name!
	Ogre::Entity* enemyMesh = mSceneManager->createEntity(name, mesh);
	Ogre::SceneNode* enemySceneNode = mSceneManager->getRootSceneNode()->createChildSceneNode();

	enemySceneNode->attachObject(enemyMesh);
	
	// Balloon billboard
	Ogre::BillboardSet* mBalloonSet = mSceneManager->createBillboardSet(name + "_BillboardSet");
	enemySceneNode->attachObject(mBalloonSet);

	EnemyPtr enemy = EnemyPtr(new Enemy(type));
	enemy->initializeEntity(enemyMesh, enemySceneNode);
	enemy->setBillboardSet(mBalloonSet);

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

int EnemyManager::getCount()
{
	return mEnemyList.size();
}

EnemyPtr EnemyManager::getEnemy(int index)
{
	return mEnemyList[index];
}


EnemyPtr EnemyManager::getEnemy(Ogre::String name)
{
	return mEnemyMap[name];
}

bool EnemyManager::removeEnemy(Ogre::String name)
{
	//
	// TODO : maybe we don't really need a list, and we can just use a map...
	//
	EnemyPtr enemyToErase = mEnemyMap[name];

	mEnemyMap.erase(name);
	
	EnemyListIterator it = find(mEnemyList.begin(), mEnemyList.end(), enemyToErase);
	
	if( it != mEnemyList.end() )
		mEnemyList.erase(it);

	mSceneManager->destroyEntity(name);//getRootSceneNode()->removeChild(name);

	return true;
}

/* 
* TODO
*/
void EnemyManager::update(const float elapsedSeconds)
{
	for(int i = 0; i < mEnemyList.size() ; i++)
	{
		EnemyPtr enemy =  mEnemyList[i];

		enemy->updateLogic(L,elapsedSeconds);
		enemy->updatePhysics(elapsedSeconds);
		enemy->updateEntity(elapsedSeconds);
	}
}

// --------------------------------
// Lua Enemy Lib
// --------------------------------
LUA_BEGIN_BINDING(EnemyManager::enemylib)
LUA_BIND("create", EnemyManager::createEnemy)
LUA_BIND("getCount", EnemyManager::getEnemyCount)
LUA_BIND("getName",EnemyManager::getEnemyName)
LUA_BIND("getPosition", EnemyManager::getEnemyPosition)
LUA_BIND("setPosition", EnemyManager::setEnemyPosition)
LUA_BIND("setState", EnemyManager::setEnemyState)
LUA_BIND("setTarget", EnemyManager::setEnemyTarget)
LUA_BIND("getStateTimeout", EnemyManager::getEnemyStateTimeout)
LUA_BIND("isHurt", EnemyManager::isEnemyHurt)
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

int EnemyManager::getEnemyCount(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 0

	Ogre::Real count = EnemyManager::getSingleton().getCount();

	lua_pushnumber(L,count);

	/* return the number of results */
	return 1;
}

int EnemyManager::getEnemyName(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 1, the enemy idx
	int index = luaL_checkint(L, 1);

	EnemyPtr enemy = EnemyManager::getSingleton().getEnemy(index);

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

	// n should be 2, the enemy idx
	Ogre::String name = luaL_checkstring(L, 1);
	int state = luaL_checkint(L, 2);

	EnemyPtr enemy = EnemyManager::getSingleton().getEnemy(name);

	if(state == 1)
	{
		enemy->setMaterialName("Skin/Green");
	}
	else if(state == 2)
	{
		enemy->setMaterialName("Skin/Yellow");
	}
	else if(state == 3)
	{
		enemy->setMaterialName("Skin/Blue");
	}
	else if(state == 4)
	{
		enemy->setMaterialName("Skin/Red");
	}
	else
	{
		enemy->setMaterialName("Skin");
	}

	/* return the number of results */
	return 1;
}

int EnemyManager::getEnemyStateTimeout(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 1
	Ogre::String name = luaL_checkstring(L, 1);

	EnemyPtr enemy = EnemyManager::getSingleton().getEnemy(name);

	lua_pushnumber(L,enemy->getStateTimeout());

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

int EnemyManager::setEnemyTarget(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 2
	Ogre::String enemyId = luaL_checkstring(L, 1);
	Ogre::String targetId = luaL_checkstring(L, 2);

	SceneManager* sceneManager = EnemyManager::getSingleton().getSceneManager();

	EnemyPtr enemy = EnemyManager::getSingleton().getEnemy(enemyId);
	SceneNode* target = sceneManager->getEntity(targetId)->getParentSceneNode();

	enemy->setTarget(target);

	/* return the number of results */
	return 0;
}

int EnemyManager::isEnemyHurt(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	Ogre::String enemyId = luaL_checkstring(L, 1);

	EnemyPtr enemy = EnemyManager::getSingleton().getEnemy(enemyId);

	lua_pushboolean(L,enemy->isHurt());

	/* return the number of results */
	return 1;
}
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

EnemyManager::EnemyManager(Ogre::SceneManager* sceneManager) 
: mEnemyNode(0)
, mCount(0)
, mId(0)
, mIsDebugEnabled(false)
{
	mSceneManager = sceneManager;
}

EnemyManager::~EnemyManager()
{
	finalize();
}

void EnemyManager::initialize()
{
	mEnemyNode = mSceneManager->getRootSceneNode()->createChildSceneNode(ENEMY_NODE_NAME);
}

void EnemyManager::finalize()
{
	mEnemyList.clear();
	mEnemyMap.clear();

	Utils::Destroy(mSceneManager,ENEMY_NODE_NAME);
	mEnemyNode = NULL;
}

EnemyPtr EnemyManager::createEnemy(Enemy::EnemyTypes type)
{
	Ogre::String mesh;

	switch(type)
	{
	case Enemy::EnemyTypes::Naked:
		mesh = Ogre::String("naked.mesh");
		break;
	case Enemy::EnemyTypes::Wizard:
		mesh = Ogre::String("wizard.mesh");
		break;
	default:
		mesh = Ogre::String("naked.mesh");
		break;
	}

	Ogre::String name = createUniqueId();

	return createEnemy(type, name, mesh);
}

EnemyPtr EnemyManager::createEnemy(Enemy::EnemyTypes type, Ogre::String name, Ogre::String mesh)
{
	// Enemy name == Mesh Name!
	Ogre::Entity* enemyMesh = mSceneManager->createEntity(name, mesh);
	enemyMesh->setQueryFlags(SceneManager::ENTITY_TYPE_MASK);
	Ogre::SceneNode* enemySceneNode = mEnemyNode->createChildSceneNode("Enemy_"+name+"_Node");

	enemySceneNode->attachObject(enemyMesh);

	EnemyPtr enemy = EnemyPtr(new Enemy(name, type));

	enemy->initializeEntity(enemyMesh, enemySceneNode, mSceneManager);

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
	{
		mEnemyList.erase(it);
	}
	
	// Fire the event
	EnemyRemovedEventPtr e = EnemyRemovedEventPtr(new EnemyRemovedEvent(enemyToErase) );
	raiseEvent(e);

	return true;
}

/* 
* TODO
*/
void EnemyManager::update(const float elapsedSeconds)
{
	EnemyList deadEnemies;

	for(int i = 0; i < mEnemyList.size() ; i++)
	{
		EnemyPtr enemy =  mEnemyList[i];

		// Is dead, this means that we can delete him!
		if(enemy->getEnemyState() == Enemy::EnemyStates::Dead)
		{
			// Add them to the 'to delete' list
			deadEnemies.push_back(enemy);
		}	
		else // Still alive, so we have to update him!
		{
			enemy->updateLogic(L,elapsedSeconds);
			//enemy->updatePhysics(elapsedSeconds);
			enemy->updateEntity(elapsedSeconds); // this updates animations too!
		}
	}

	// Now we have to remove them and notify other listeners with the 'dead/remove' event!
	for(int i = 0; i < deadEnemies.size(); i++)
	{
		EnemyPtr e = deadEnemies[i];
		removeEnemy(e->getName());
	}

	deadEnemies.clear();
}

void EnemyManager::setDebugEnabled(bool isDebugEnabled)
{
	if(mIsDebugEnabled != isDebugEnabled)
	{
		mIsDebugEnabled = isDebugEnabled;

		for(int i = 0; i < mEnemyList.size() ; i++)
		{
			EnemyPtr enemy =  mEnemyList[i];
			enemy->setDebugEnabled(mIsDebugEnabled);
		}
	}
}

// --------------
// Event handlers
// --------------
void EnemyManager::registerHandlers()
{
	boost::shared_ptr<EnemyManager> this_ = shared_from_this();

	registerHandler(EventHandlerPtr(new EventHandler<EnemyManager,CollisionEvent>(this_,&EnemyManager::handleCollisionEvent)),EventTypes::Collision);
	registerHandler(EventHandlerPtr(new EventHandler<EnemyManager,EnemyHitEvent>(this_,&EnemyManager::handleEnemyHitEvent)),EventTypes::EnemyHit);
	registerHandler(EventHandlerPtr(new EventHandler<EnemyManager,EnemySpecialHitEvent>(this_,&EnemyManager::handleEnemySpecialHitEvent)),EventTypes::EnemySpecialHit);
}

void EnemyManager::unregisterHandlers()
{

}

void EnemyManager::handleCollisionEvent(CollisionEventPtr evt)
{
// TODO
}

void EnemyManager::handleEnemyHitEvent(EnemyHitEventPtr evt)
 {
	EnemyPtr enemy = evt->getEnemy();
	PlayerPtr player = evt->getPlayer();

	enemy->hit(evt->getDamage());

	if(enemy->isDying())
	{
		EnemyKillEventPtr eKill = EnemyKillEventPtr(new EnemyKillEvent(enemy, player));
		raiseEvent(eKill);
	}
}


void EnemyManager::handleEnemySpecialHitEvent(EnemySpecialHitEventPtr evt)
 {
	EnemyPtr enemy = evt->getEnemy();
	PlayerPtr player = evt->getPlayer();

	enemy->hit(evt->getDamage());

	if(enemy->isDying())
	{
		EnemyKillEventPtr eKill = EnemyKillEventPtr(new EnemyKillEvent(enemy, player));
		raiseEvent(eKill);
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
LUA_BIND("isDying", EnemyManager::isEnemyDying)
LUA_BIND("remove", EnemyManager::removeEnemy)
LUA_END_BINDING()

int EnemyManager::createEnemy(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 1, the enemy type

	int type = luaL_checkint(L, 1);

	EnemyPtr enemy = EnemyManager::getSingleton().createEnemy((Enemy::EnemyTypes)type);

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

int EnemyManager::isEnemyDying(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	Ogre::String enemyId = luaL_checkstring(L, 1);

	EnemyPtr enemy = EnemyManager::getSingleton().getEnemy(enemyId);

	lua_pushboolean(L,enemy->isDying());

	/* return the number of results */
	return 1;
}
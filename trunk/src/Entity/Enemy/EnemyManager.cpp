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
, mId(0)
, mIsDebugEnabled(false)
, mCurrentGameArea(-1)
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
	mEnemyMapList.clear();

	Utils::Destroy(mSceneManager,ENEMY_NODE_NAME);
	mEnemyNode = NULL;
}

// Random enemies
EnemyPtr EnemyManager::createEnemy(Enemy::EnemyTypes type)
{
	Ogre::String mesh;

	int subType = rand();

	switch(type)
	{
		case Enemy::EnemyTypes::Chicken:
			mesh = Ogre::String("chicken.mesh");
			break;
		case Enemy::EnemyTypes::Knight:			
			if( subType%3 == 0 )
				mesh = Ogre::String("knighA.mesh");
			else if( subType%3 == 1 )
				mesh = Ogre::String("knighB.mesh");
			else
				mesh = Ogre::String("knighC.mesh");
			break;
		case Enemy::EnemyTypes::Wizard:
			mesh = Ogre::String("wizard.mesh");
			break;
		case Enemy::EnemyTypes::Naked:
			mesh = Ogre::String("naked.mesh");
			break;
	}

	Ogre::String name = createUniqueId();

	Ogre::Entity* enemyMesh = mSceneManager->createEntity(name, mesh);
	enemyMesh->setCastShadows(true);
	enemyMesh->setQueryFlags(SceneManager::ENTITY_TYPE_MASK);

	Ogre::SceneNode* sceneNode = mEnemyNode->createChildSceneNode(name + "_Node");
	sceneNode->attachObject(enemyMesh);

	Enemy::EnemyParameters params;

	// Get standar parameters for enemy type

	return createEnemy(type, name, enemyMesh, sceneNode, params, mCurrentGameArea);
}

// Fixed enemies from xml
EnemyPtr EnemyManager::createEnemy(Enemy::EnemyTypes type, Ogre::String name, Ogre::Entity* mesh, Ogre::SceneNode* sceneNode, Enemy::EnemyParameters params, int gameArea)
{
	// Enemy name == Mesh Name!
	Ogre::Entity* enemyMesh = mesh;
	
	sceneNode->attachObject(enemyMesh);

	EnemyPtr enemy = EnemyPtr(new Enemy(name, type, params));
	enemy->initializeEntity(enemyMesh, sceneNode, mSceneManager);

	// Die mesh and animation
	Ogre::Entity* enemyDieMesh = NULL;
	Ogre::AnimationState* enemyDieAnimation = NULL;

	// Attach die mesh to node if exists and set to enemy
	if( params.dieMesh != "" )
	{
		enemyDieMesh = mSceneManager->createEntity(name + "Die", params.dieMesh);
		sceneNode->attachObject(enemyDieMesh);			
		enemy->setDieMesh(enemyDieMesh);		

		// Set die animation if exists
		if( params.dieAnimation != "" )
		{
			enemyDieAnimation = enemyDieMesh->getAnimationState(params.dieAnimation);
			enemy->setDieAnimation(enemyDieAnimation);
		}
	}

	// Store enemy in its game area
	mEnemyMapList[gameArea].push_back(enemy);

	// Set visible false by default
	enemy->setVisible(false);

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
	int count = 0;

	for( int i = 0; i < mEnemyMapList[mCurrentGameArea].size(); i++ )
	{
		if( mEnemyMapList[mCurrentGameArea][i]->getEnemyType() != Enemy::EnemyTypes::Chicken && 
			mEnemyMapList[mCurrentGameArea][i]->getEnemyType() != Enemy::EnemyTypes::Cow )
			count++;
	}

	return count;
}

int EnemyManager::getCount(int gameArea)
{
	int count = 0;

	for( int i = 0; i < mEnemyMapList[gameArea].size(); i++ )
	{
		if( mEnemyMapList[gameArea][i]->getEnemyType() != Enemy::EnemyTypes::Chicken && 
			mEnemyMapList[gameArea][i]->getEnemyType() != Enemy::EnemyTypes::Cow )
			count++;
	}

	return count;
}

EnemyPtr EnemyManager::getEnemy(int index)
{
	return mEnemyMapList[mCurrentGameArea][index];
}

EnemyPtr EnemyManager::getEnemy(int index, int gameArea)
{
	return mEnemyMapList[gameArea][index];
}

EnemyPtr EnemyManager::getEnemy(Ogre::String name)
{
	// Search enemy in current game area enemies list
	for( int i = 0; i < mEnemyMapList[mCurrentGameArea].size(); i++ )
	{
		EnemyPtr enemy = mEnemyMapList[mCurrentGameArea][i];

		if( enemy->getName() == name )
			return mEnemyMapList[mCurrentGameArea][i];
	}

	// Search in others game areas lists
	for( EnemyMapListIterator it = mEnemyMapList.begin(); it != mEnemyMapList.end(); ++it )
	{
		EnemyList list = it->second;

		for( int i = 0; i < list.size(); i++ )
		{
			EnemyPtr enemy = list[i];

			if( enemy->getName() == name )
				return list[i];
		}
	}
}

EnemyPtr EnemyManager::getEnemy(Ogre::String name, int gameArea)
{
	for( int i = 0; i < mEnemyMapList[gameArea].size(); i++ )
	{
		EnemyPtr enemy = mEnemyMapList[gameArea][i];

		if( enemy->getName() == name )
			return mEnemyMapList[gameArea][i];
	}
}

bool EnemyManager::removeEnemy(Ogre::String name)
{
	EnemyPtr enemyToErase = getEnemy(name);
	
	EnemyListIterator it = find(mEnemyMapList[mCurrentGameArea].begin(), mEnemyMapList[mCurrentGameArea].end(), enemyToErase);
	
	if( it != mEnemyMapList[mCurrentGameArea].end() )
	{
		mEnemyMapList[mCurrentGameArea].erase(it);
		return true;
	}

	return false;
}

void EnemyManager::update(const float elapsedSeconds)
{
	for(int i = 0; i < mEnemyMapList[mCurrentGameArea].size() ; i++)
	{
		EnemyPtr enemy =  mEnemyMapList[mCurrentGameArea][i];

		enemy->updateLogic(L,elapsedSeconds);		
		enemy->updateEntity(elapsedSeconds); // this updates animations too!
		
		// Attack cooldown
		if( enemy->getAttackTimeout() == 0 )
		{
			if( enemy->attackStart() )
			{
				//If attack comes from a wizard
				if(enemy->getEnemyType() == Enemy::EnemyTypes::Wizard){
					//Projectile event 
					ProjectileFireEventPtr projEvt = ProjectileFireEventPtr(new ProjectileFireEvent(enemy));
					EVENTS_FIRE(projEvt);
				}
				EnemyAttackEventPtr evt = EnemyAttackEventPtr(new EnemyAttackEvent(enemy));
				EVENTS_FIRE(evt);				

				enemy->setAttackTimeout(elapsedSeconds);
			}
		}
		else
		{
			enemy->setAttackTimeout(enemy->getAttackTimeout() + elapsedSeconds);
			if( enemy->getAttackTimeout() > enemy->getAttackCooldown() )
			{
				enemy->setAttackTimeout(0);
				enemy->attackFinished();
				enemy->setAttackHited(false);
			}
		}
	}

	// If game area is cleared raise event
	if( getCount() == 0 )
	{
		GameAreaClearedEventPtr evt = GameAreaClearedEventPtr(new GameAreaClearedEvent(mCurrentGameArea));
		EVENTS_FIRE(evt);
	}
}

void EnemyManager::setDebugEnabled(bool isDebugEnabled)
{
	if(mIsDebugEnabled != isDebugEnabled)
	{
		mIsDebugEnabled = isDebugEnabled;

		for(int i = 0; i < mEnemyMapList[mCurrentGameArea].size() ; i++)
		{
			EnemyPtr enemy =  mEnemyMapList[mCurrentGameArea][i];
			enemy->setDebugEnabled(mIsDebugEnabled);
		}
	}
}

// Visibility
void EnemyManager::setEnemiesVisible(bool visibility, int gameArea)
{
	for( int i = 0; i < mEnemyMapList[gameArea].size(); i++ )
	{
		mEnemyMapList[gameArea][i]->setVisible(visibility);
	}
}

// --------------
// Event handlers
// --------------
EVENTS_BEGIN_REGISTER_HANDLERS(EnemyManager)
	EVENTS_REGISTER_HANDLER(EnemyManager, Collision)
	EVENTS_REGISTER_HANDLER(EnemyManager, EnemyHit)
	EVENTS_REGISTER_HANDLER(EnemyManager, EnemyKilled)
	EVENTS_REGISTER_HANDLER(EnemyManager, EnemyRemove)
	EVENTS_REGISTER_HANDLER(EnemyManager, EnemyCustom);
	EVENTS_REGISTER_HANDLER(EnemyManager, GameAreaChanged);
EVENTS_END_REGISTER_HANDLERS()

EVENTS_BEGIN_UNREGISTER_HANDLERS(EnemyManager)
	EVENTS_UNREGISTER_HANDLER(EnemyManager, Collision)
	EVENTS_UNREGISTER_HANDLER(EnemyManager, EnemyHit)
	EVENTS_UNREGISTER_HANDLER(EnemyManager, EnemyKilled)
	EVENTS_UNREGISTER_HANDLER(EnemyManager, EnemyRemove)
	EVENTS_UNREGISTER_HANDLER(EnemyManager, EnemyCustom);
	EVENTS_UNREGISTER_HANDLER(EnemyManager, GameAreaChanged);
EVENTS_END_UNREGISTER_HANDLERS()

EVENTS_DEFINE_HANDLER(EnemyManager, Collision)
{
// TODO
}

EVENTS_DEFINE_HANDLER(EnemyManager, EnemyHit)
{
	Debug::Out("EnemyManager : handleEnemyHitEvent");

	EnemyPtr enemy = evt->getEnemy();
	PlayerPtr player = evt->getPlayer();

	enemy->hit(evt->getDamage());

	if(enemy->isDying())
	{
		EnemyKilledEventPtr eKill = EnemyKilledEventPtr(new EnemyKilledEvent(enemy, player));
 		EVENTS_FIRE(eKill);
	}
}

EVENTS_DEFINE_HANDLER(EnemyManager, EnemyKilled)
{
	Debug::Out("EnemyManager : handleEnemyKilledEvent");

	EnemyPtr enemy = evt->getEnemy();
	PlayerPtr player = evt->getPlayer();

	// Stop the enemy from rotating and moving
	enemy->stop();

	if( player->isSpecial() )
	{
		enemy->setMaterialName("Skin/Black");
		enemy->setBurning(true);
		EnemyCustomEventPtr eCustom = EnemyCustomEventPtr(new EnemyCustomEvent(enemy));
		EVENTS_FIRE_AFTER(eCustom, 1.0f);
	}
	else
	{
		if( enemy->getEnemyType() == Enemy::EnemyTypes::Chicken )
		{
			enemy->dieSwitch();
		}
		else 
		{
			if( (rand()%6) == 0 )
				enemy->dieToCamera();
			else
			{				
				if( enemy->hasDieAnimation() )
				{
					enemy->dieSwitch();
					enemy->setDieMaterialName("Skin/Red");
				}
				else
					enemy->setMaterialName("Skin/Red");
			}
		}

		EnemyRemoveEventPtr eRemove = EnemyRemoveEventPtr(new EnemyRemoveEvent(enemy));
 		EVENTS_FIRE_AFTER(eRemove, 5.0f);
	}
}

EVENTS_DEFINE_HANDLER(EnemyManager, EnemyRemove)
{
	Debug::Out("EnemyManager : handleEnemyRemoveEvent");

	EnemyPtr e = evt->getEnemy();
	removeEnemy(e->getName());
}

EVENTS_DEFINE_HANDLER(EnemyManager, EnemyCustom)
{
	Debug::Out("EnemyManager : handleEnemyCustomEvent");

	EnemyPtr enemy = evt->getEnemy();

	if( enemy->isBurning())
		enemy->setVisible(false);

	EnemyRemoveEventPtr eRemove = EnemyRemoveEventPtr(new EnemyRemoveEvent(enemy));
	EVENTS_FIRE_AFTER(eRemove, 5.0f);
}

EVENTS_DEFINE_HANDLER(EnemyManager, GameAreaChanged)
{
	Debug::Out("EnemyManager : handleGameAreaChangedEvent");

	mCurrentGameArea = evt->getActualArea();

	setEnemiesVisible(false, evt->getPreviousArea());
	setEnemiesVisible(true, evt->getActualArea());
}

// --------------------------------
// Lua Enemy Lib
// --------------------------------
LUA_BEGIN_BINDING(EnemyManager, enemylib)
LUA_BIND(EnemyManager, create)
LUA_BIND(EnemyManager, getCount)
LUA_BIND(EnemyManager, getName)
LUA_BIND(EnemyManager, getPosition)
LUA_BIND(EnemyManager, setPosition)
LUA_BIND(EnemyManager, setState)
LUA_BIND(EnemyManager, setTarget)
LUA_BIND(EnemyManager, getStateTimeout)
LUA_BIND(EnemyManager, isHurt)
LUA_BIND(EnemyManager, isDying)
LUA_BIND(EnemyManager, remove)
LUA_END_BINDING()

//
// Load lua scripts that will be used by this manager
//
LUA_BEGIN_LOAD_SCRIPTS(EnemyManager)
LUA_LOAD_SCRIPT(".\\data\\scripts\\EnemyLogic.lua")
LUA_END_LOAD_SCRIPTS()

LUA_DEFINE_FUNCTION(EnemyManager, create)
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

LUA_DEFINE_FUNCTION(EnemyManager, getCount)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 0

	Ogre::Real count = EnemyManager::getSingleton().getCount();

	lua_pushnumber(L,count);

	/* return the number of results */
	return 1;
}

LUA_DEFINE_FUNCTION(EnemyManager, getName)
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

LUA_DEFINE_FUNCTION(EnemyManager, getPosition)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 1;
}

LUA_DEFINE_FUNCTION(EnemyManager,setPosition)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 1;
}

LUA_DEFINE_FUNCTION(EnemyManager,setState)
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

LUA_DEFINE_FUNCTION(EnemyManager,getStateTimeout)
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

LUA_DEFINE_FUNCTION(EnemyManager,remove)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 1;
}

LUA_DEFINE_FUNCTION(EnemyManager,setTarget)
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

LUA_DEFINE_FUNCTION(EnemyManager,isHurt)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	Ogre::String enemyId = luaL_checkstring(L, 1);

	EnemyPtr enemy = EnemyManager::getSingleton().getEnemy(enemyId);

	lua_pushboolean(L,enemy->isHurt());

	/* return the number of results */
	return 1;
}

LUA_DEFINE_FUNCTION(EnemyManager,isDying)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	Ogre::String enemyId = luaL_checkstring(L, 1);

	EnemyPtr enemy = EnemyManager::getSingleton().getEnemy(enemyId);

	lua_pushboolean(L,enemy->isDying());

	/* return the number of results */
	return 1;
}
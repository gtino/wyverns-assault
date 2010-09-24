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
, mCurrentLevel(-1)
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

// Enemy parameters
Enemy::EnemyParameters EnemyManager::createParameters(int difficult, Enemy::EnemyTypes type)
{
	Enemy::EnemyParameters params;

	if( type == Enemy::EnemyTypes::Chicken )
	{
		params.animationTree = "data/animations/chicken.xml";
		params.life = 5.0;
		params.points = 25.0;
		params.speed = 20.0;
		params.damage = 0.0;
		params.specialDamage = 0.0;
		params.height = 2.0;
		params.attackCooldown = 0.5;
		params.dieMesh = "chickenDie.mesh";
		params.dieAnimation = "";
	}
	else if( type == Enemy::EnemyTypes::Cow )
	{
		params.animationTree = "data/animations/cow.xml";
		params.life = 10.0;
		params.points = 50.0;
		params.speed = 15.0;
		params.damage = 0.0;
		params.specialDamage = 0.0;
		params.height = 2.0;
		params.attackCooldown = 0.5;
		params.dieMesh = "cowDie.mesh";
		params.dieAnimation = "";
	}
	else if( type == Enemy::EnemyTypes::Naked )
	{
		params.animationTree = "data/animations/naked.xml";
		params.life = 35.0;
		params.points = 150.0;
		params.speed = 50.0;
		params.damage = 0.0;
		params.specialDamage = 0.0;
		params.height = 13.0;
		params.attackCooldown = 0.5;
		params.dieMesh = "nakedDie.mesh";
		params.dieAnimation = "Die";
	}
	else if( type == Enemy::EnemyTypes::Wizard )
	{
		params.animationTree = "data/animations/wizard.xml";
		params.life = 150.0;
		params.points = 250.0;
		params.speed = 50.0;
		params.damage = 3.0;
		params.specialDamage = 6.0;
		params.height = 13.0;
		params.attackCooldown = 0.5;
		params.dieMesh = "nakedDie.mesh";
		params.dieAnimation = "Die";
	}
	else if( type == Enemy::EnemyTypes::Peasant )
	{
		params.animationTree = "data/animations/peasant.xml";
		params.life = 50.0;
		params.points = 100.0;
		params.speed = 50.0;
		params.damage = 0.0;
		params.specialDamage = 0.0;
		params.height = 13.0;
		params.attackCooldown = 0.5;
		params.dieMesh = "nakedDie.mesh";
		params.dieAnimation = "Die";
	}
	else
	{
		params.animationTree = "data/animations/knight.xml";
		params.life = 75.0 * difficult;
		params.points = 100.0 * difficult;
		params.speed = 50.0;
		params.damage = 1.0 * difficult;
		params.specialDamage = 0.0;
		params.height = 13.0;
		params.attackCooldown = 0.5;
		params.dieMesh = "nakedDie.mesh";
		params.dieAnimation = "Die";
	}	

	return params;
}

// Random enemies
EnemyPtr EnemyManager::createEnemy(Enemy::EnemyTypes type, int difficult, Vector3 position)
{
	Ogre::String mesh;
	Ogre::String material;

	int subType = rand();

	switch(type)
	{
		case Enemy::EnemyTypes::Chicken:
			mesh = Ogre::String("chicken.mesh");
			break;
		case Enemy::EnemyTypes::Cow:
			mesh = Ogre::String("cow.mesh");
			break;
		case Enemy::EnemyTypes::Knight:			
			if( subType%3 == 0 )
			{
				mesh = Ogre::String("knightA.mesh");
				subType = rand();
				if( subType%5 == 0 )
					material = "knightA_material";
				else if( subType%5 == 1 )
					material = "knightA_material_2";
				else if( subType%5 == 2 )
					material = "knightA_material_3";
				else if( subType%5 == 3 )
					material = "knightA_material_4";
				else
					material = "knightA_material_5";
			}
			else if( subType%3 == 1 )
			{
				mesh = Ogre::String("knightB.mesh");
				subType = rand();
				if( subType%5 == 0 )
					material = "knightB_material";
				else if( subType%5 == 1 )
					material = "knightB_material_2";
				else if( subType%5 == 2 )
					material = "knightB_material_3";
				else if( subType%5 == 3 )
					material = "knightB_material_4";
				else
					material = "knightB_material_5";
			}
			else
			{
				mesh = Ogre::String("knightC.mesh");
				subType = rand();
				if( subType%5 == 0 )
					material = "knightC_material";
				else if( subType%5 == 1 )
					material = "knightC_material_2";
				else if( subType%5 == 2 )
					material = "knightC_material_3";
				else if( subType%5 == 3 )
					material = "knightC_material_4";
				else
					material = "knightC_material_5";
			}
			break;
		case Enemy::EnemyTypes::Wizard:
			mesh = Ogre::String("wizard.mesh");
			break;
		case Enemy::EnemyTypes::Naked:
			mesh = Ogre::String("naked.mesh");
			break;
		case Enemy::EnemyTypes::Peasant:
			mesh = Ogre::String("peasant.mesh");
			break;
		case Enemy::EnemyTypes::Woman:
			mesh = Ogre::String("princess.mesh");
			break;
	}

	Ogre::String name = createUniqueId();
	Ogre::Entity* enemyMesh = mSceneManager->createEntity(name, mesh);
	enemyMesh->setCastShadows(true);
	enemyMesh->setQueryFlags(SceneManager::ENTITY_TYPE_MASK);

	if( type == Enemy::EnemyTypes::Knight ) 
		enemyMesh->setMaterialName(material);

	Ogre::SceneNode* sceneNode = mEnemyNode->createChildSceneNode(name + "_Node", position);

	// Get standar parameters for enemy type
	Enemy::EnemyParameters params = createParameters(difficult, type);	

	// Create enemy
	EnemyPtr enemy = createEnemy(type, name, enemyMesh, sceneNode, params, mCurrentGameArea, true);
	
	return enemy;
}

// Fixed enemies from xml
EnemyPtr EnemyManager::createEnemy(Enemy::EnemyTypes type, Ogre::String name, Ogre::Entity* mesh, Ogre::SceneNode* sceneNode, Enemy::EnemyParameters params, int gameArea, bool visible)
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
	// Set die animation if its in same mesh
	else
	{
		// Set die animation if exists
		if( params.dieAnimation != "" )
		{
			enemyDieAnimation = enemyMesh->getAnimationState(params.dieAnimation);
			enemy->setDieAnimation(enemyDieAnimation);
		}
	}

	// Store enemy in its game area
	mEnemyMapList[gameArea].push_back(enemy);

	// Set visible false by default
	enemy->setVisible(visible);

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
		// Don' count cows and chickens
		if( mEnemyMapList[mCurrentGameArea][i]->getEnemyType() != Enemy::EnemyTypes::Chicken && 
			mEnemyMapList[mCurrentGameArea][i]->getEnemyType() != Enemy::EnemyTypes::Cow )
		{
			// Count alive enemies
			if( !mEnemyMapList[mCurrentGameArea][i]->isDying() )
				count++;
		}
	}

	return count;
}

int EnemyManager::getCount(int gameArea)
{
	int count = 0;

	for( int i = 0; i < mEnemyMapList[gameArea].size(); i++ )
	{
		// Don't count cows and chickens
		if( mEnemyMapList[gameArea][i]->getEnemyType() != Enemy::EnemyTypes::Chicken && 
			mEnemyMapList[gameArea][i]->getEnemyType() != Enemy::EnemyTypes::Cow )
		{
			// Count alive enemies
			if( !mEnemyMapList[mCurrentGameArea][i]->isDying() )
				count++;
		}
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
			return enemy;
	}

	// Search in others game areas lists
	for( EnemyMapListIterator it = mEnemyMapList.begin(); it != mEnemyMapList.end(); ++it )
	{
		EnemyList list = it->second;

		for( int i = 0; i < list.size(); i++ )
		{
			EnemyPtr enemy = list[i];

			if( enemy->getName() == name )
				return enemy;
		}
	}

	EnemyPtr enemy;

	return enemy;
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

	if( enemyToErase == NULL )
		return false;
	
	EnemyListIterator it = find(mEnemyMapList[mCurrentGameArea].begin(), mEnemyMapList[mCurrentGameArea].end(), enemyToErase);
	
	if( it != mEnemyMapList[mCurrentGameArea].end() )
	{
		mEnemyMapList[mCurrentGameArea].erase(it);
		return true;
	}

	return false;
}

void EnemyManager::killAllEnemies(PlayerPtr player, int gameArea)
{
	// Current game area
	if( gameArea == -1 )
		gameArea = mCurrentGameArea;

	for( int i = 0; i <  mEnemyMapList[gameArea].size(); i++ )
	{
		EnemyKilledEventPtr kill = EnemyKilledEventPtr(new EnemyKilledEvent(mEnemyMapList[mCurrentGameArea][i], player));
		EVENTS_FIRE(kill);
		EnemyRemoveEventPtr remove = EnemyRemoveEventPtr(new EnemyRemoveEvent(mEnemyMapList[mCurrentGameArea][i]));
		EVENTS_FIRE_AFTER(remove, 2.0);
	}
}

void EnemyManager::update(const float elapsedSeconds)
{
	for(int i = 0; i < mEnemyMapList[mCurrentGameArea].size() ; i++)
	{
		EnemyPtr enemy =  mEnemyMapList[mCurrentGameArea][i];

		enemy->updateLogic(L,elapsedSeconds);		
		enemy->updateEntity(elapsedSeconds); // this updates animations too!
		
		// Physic debugg control
		enemy->setDebugEnabled(mIsDebugEnabled);

		// Attack cooldown
		if( enemy->getAttackTimeout() == 0 )
		{
			if( enemy->attackStart() )
			{
				//If attack comes from ranged
				if( enemy->isRanged() )
				{
					//Projectile event 
					ProjectileFireEventPtr projEvt = ProjectileFireEventPtr(new ProjectileFireEvent( enemy, enemy->getTarget()->getPosition() ));
					EVENTS_FIRE_AFTER(projEvt, 0.75);
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

		// Womans logic
		if( enemy->getEnemyType() == Enemy::EnemyTypes::Woman )
		{
			if( enemy->getEnemyState() == Enemy::EnemyStates::Love && enemy->hasItem() )
			{
				EnemyCreateItemEventPtr evt = EnemyCreateItemEventPtr(new EnemyCreateItemEvent(enemy, mCurrentGameArea));
				EVENTS_FIRE_AFTER(evt, 0.5);
				enemy->itemDroped();
			}
			if( enemy->getEnemyState() == Enemy::EnemyStates::Patrol )
			{
				enemy->setVisible(false);
				EnemyRemoveEventPtr evt = EnemyRemoveEventPtr(new EnemyRemoveEvent(enemy));
				EVENTS_FIRE(evt);
			}
		}

		// Bonus game area, hide ballon
		if( mCurrentGameArea == 3 )
			enemy->hideBalloon();
	}

	// If game area is cleared raise event
	if( getCount() <= 0 )
	{
		GameAreaEnemiesDeathEventPtr evt = GameAreaEnemiesDeathEventPtr(new GameAreaEnemiesDeathEvent(mCurrentLevel, mCurrentGameArea));
		EVENTS_FIRE(evt);
	}
}

void EnemyManager::setDebugEnabled(bool isDebugEnabled)
{
	if(mIsDebugEnabled != isDebugEnabled)
	{
		mIsDebugEnabled = isDebugEnabled;
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
	EVENTS_REGISTER_HANDLER(EnemyManager, EnemyCustom)
	EVENTS_REGISTER_HANDLER(EnemyManager, EnemyCreation)
	EVENTS_REGISTER_HANDLER(EnemyManager, GameAreaChanged)
	EVENTS_REGISTER_HANDLER(EnemyManager, GameAreaCleared)
EVENTS_END_REGISTER_HANDLERS()

EVENTS_BEGIN_UNREGISTER_HANDLERS(EnemyManager)
	EVENTS_UNREGISTER_HANDLER(EnemyManager, Collision)
	EVENTS_UNREGISTER_HANDLER(EnemyManager, EnemyHit)
	EVENTS_UNREGISTER_HANDLER(EnemyManager, EnemyKilled)
	EVENTS_UNREGISTER_HANDLER(EnemyManager, EnemyRemove)
	EVENTS_UNREGISTER_HANDLER(EnemyManager, EnemyCustom)
	EVENTS_UNREGISTER_HANDLER(EnemyManager, EnemyCreation)
	EVENTS_UNREGISTER_HANDLER(EnemyManager, GameAreaChanged)
	EVENTS_UNREGISTER_HANDLER(EnemyManager, GameAreaCleared)
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

		// If player burns a chicken will drop an item
		if( enemy->getEnemyType() == Enemy::EnemyTypes::Chicken || enemy->getEnemyType() == Enemy::EnemyTypes::Cow )
		{
			EnemyCreateItemEventPtr eItem = EnemyCreateItemEventPtr(new EnemyCreateItemEvent(enemy, mCurrentGameArea));
			EVENTS_FIRE_AFTER(eItem, 1.0f);
			
			enemy->itemDroped();
		}
	}
	else
	{
		if( enemy->getEnemyType() == Enemy::EnemyTypes::Chicken || enemy->getEnemyType() == Enemy::EnemyTypes::Cow )
		{
			enemy->dieSwitch();
		}
		else 
		{
			if( (rand()%6) == 0 )
				enemy->dieToCamera();
			else
			{				
				if( enemy->hasDieMesh() )
				{
					enemy->dieSwitch();
					if( enemy->getEnemyType() != Enemy::EnemyTypes::BatteringRam )
						enemy->setDieMaterialName("Skin/Red");
				}
				else if ( !enemy->hasDieAnimation() )
					enemy->setMaterialName("Skin/Red");

				// If enemy has an item and its not flying will drop it
				if( enemy->hasItem() && enemy->getEnemyType() != Enemy::EnemyTypes::Woman )
				{	
					EnemyCreateItemEventPtr eItem = EnemyCreateItemEventPtr(new EnemyCreateItemEvent(enemy, mCurrentGameArea));
					EVENTS_FIRE_AFTER(eItem, 1.0f);
						
					enemy->itemDroped();
				}
			}
		}
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
}

EVENTS_DEFINE_HANDLER(EnemyManager, EnemyCreation)
{
	Debug::Out("EnemyManager : handleEnemyCreationEvent");

	EnemyPtr enemy = createEnemy(evt->getType(), evt->getDifficult(), evt->getPosition());

	EnemyPhysicsEventPtr e = EnemyPhysicsEventPtr(new EnemyPhysicsEvent(enemy, evt->getGameArea()));
	EVENTS_FIRE(e);
}

EVENTS_DEFINE_HANDLER(EnemyManager, GameAreaChanged)
{
	Debug::Out("EnemyManager : handleGameAreaChangedEvent");

	mCurrentGameArea = evt->getActualArea();
	mCurrentLevel = evt->getLevel();

	setEnemiesVisible(false, evt->getPreviousArea());
	setEnemiesVisible(true, evt->getActualArea());

	// Remove enemies from list when game area cleared
	mEnemyMapList[evt->getPreviousArea()].clear();
}

EVENTS_DEFINE_HANDLER(EnemyManager, GameAreaCleared)
{
	Debug::Out("EnemyManager : handleGameAreaClearedEvent");

	// Game area cleared by time, remove enemies
	if( evt->getType() == 1 )
		mEnemyMapList[evt->getGameArea()].clear();
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
	
	Vector3 position;

	EnemyPtr enemy = EnemyManager::getSingleton().createEnemy((Enemy::EnemyTypes)type, 1, position);

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
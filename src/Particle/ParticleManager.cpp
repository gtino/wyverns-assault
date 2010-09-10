#include "..\include\Particle\ParticleManager.h"

using namespace WyvernsAssault;

// BEGIN SINGLETON
template<> ParticleManager* Ogre::Singleton<ParticleManager>::ms_Singleton = 0;
ParticleManager* ParticleManager::getSingletonPtr(void)
{
    return ms_Singleton;
}
ParticleManager& ParticleManager::getSingleton(void)
{  
    assert( ms_Singleton );  return ( *ms_Singleton );  
}
// END SINGLETON

ParticleManager::ParticleManager(SceneManager* sceneManager)
: mId(0)
, mTimer(0.0)
{
	this->mSceneManager = sceneManager;
	mParticleSystem = NULL;
	mParticleSystemManager = NULL;
}

ParticleManager::~ParticleManager()
{
	finalize();
}

/** Initialize the particle manager */
void ParticleManager::initialize()
{
	// Get particle universe particle system manager
	mParticleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();

	// Create blood lens particle system
	mParticleSystem = mParticleSystemManager->createParticleSystem("bloodLens", "WyvernsAssault/BloodLens", mSceneManager);
	mSceneManager->getRootSceneNode()->createChildSceneNode("bloodLensNode", Vector3::ZERO)->attachObject(mParticleSystem);
}


/** Finalize the particle manager */
void ParticleManager::finalize()
{
	mParticleSystemManager->destroyAllParticleSystems(mSceneManager);

	mSceneManager->destroySceneNode("bloodLensNode");

	mParticleSystem = NULL;
	mParticleSystemManager = NULL;
	mParticleSystemMap.clear();
}

void ParticleManager::update(const float elapsedSeconds)
{
	mTimer = mTimer + elapsedSeconds;

	for(ParticleSystemMapIterator it = mParticleSystemMap.begin(); it != mParticleSystemMap.end(); ++it )
	{
		float x = fmodf(mTimer, it->first);

		if( fmodf(mTimer, it->first) < 1.0 )
		{
			ParticleUniverse::ParticleSystem* pSystem = it->second;
			pSystem->start();
		}
	}
}

// Create particle system
ParticleUniverse::ParticleSystem* ParticleManager::create(String script)
{
	return mParticleSystemManager->createParticleSystem(this->createUniqueId(), script, mSceneManager);
}

// Create particle system, add to node and start it
void ParticleManager::add(SceneNode* node, String script, Real repeat)
{
	ParticleUniverse::ParticleSystem* pSystem = mParticleSystemManager->createParticleSystem(this->createUniqueId(), script, mSceneManager);
	node->attachObject( pSystem );
	pSystem->start();

	if( repeat > 0 )
	{
		mParticleSystemMap[repeat] = pSystem;
	}
}

void ParticleManager::add(SceneNode* node, String id, String script, Real repeat)
{
	ParticleUniverse::ParticleSystem* pSystem = mParticleSystemManager->createParticleSystem(id + "_particle", script, mSceneManager);
	node->attachObject( pSystem );
	pSystem->start();
}

// Remove particle system from node and stop it (only usable with non-unique ID particle systems)
void ParticleManager::remove(String id)
{
	ParticleUniverse::ParticleSystem* pSystem = mParticleSystemManager->getParticleSystem(id + "_particle");
	pSystem->stop();
	pSystem->detachFromParent();
	mParticleSystemManager->destroyParticleSystem(pSystem, mSceneManager);
}

// Blood on camera lens
void ParticleManager::bloodLens()
{
	Vector3 position = mSceneManager->getCamera(GAME_CAMERA)->getParentSceneNode()->getPosition();
	Vector3 direction = mSceneManager->getCamera(GAME_CAMERA)->getDirection();
	SceneNode* node = mSceneManager->getSceneNode("bloodLensNode");
	node->setPosition(position + direction * 170);	
	ParticleUniverse::ParticleSystem* particles = mParticleSystemManager->getParticleSystem("bloodLens");
	particles->startAndStopFade(5);
}

// Wizard magic bolt
void ParticleManager::magicBolt(SceneNode* node)
{
	String name = node->getName();
	ParticleUniverse::ParticleSystem* particles;
	if( mParticleSystemManager->getParticleSystem(name + "_magicBolt") == NULL)
	{
		particles = mParticleSystemManager->createParticleSystem( name + "_magicBolt", "WyvernsAssault/MagicBolt", mSceneManager);
		node->attachObject( particles );
	}
	else
	{
		particles = mParticleSystemManager->getParticleSystem( name + "_magicBolt");
	}	
	particles->start();
}

Ogre::String ParticleManager::createUniqueId()
{
	Ogre::StringStream countStrStr;
	countStrStr << mId;

	Ogre::String uniqueId = "Particle_" + countStrStr.str();

	mId++;

	return uniqueId;
}

// --------------
// Event handlers
// --------------
EVENTS_BEGIN_REGISTER_HANDLERS(ParticleManager)
	EVENTS_REGISTER_HANDLER(ParticleManager,PlayerAttack)
	EVENTS_REGISTER_HANDLER(ParticleManager,PlayerHit)
	EVENTS_REGISTER_HANDLER(ParticleManager,EnemyAttack)
	EVENTS_REGISTER_HANDLER(ParticleManager,EnemyHit)
	EVENTS_REGISTER_HANDLER(ParticleManager,EnemyKilled)
	EVENTS_REGISTER_HANDLER(ParticleManager,EnemyCustom)
	EVENTS_REGISTER_HANDLER(ParticleManager,ItemCreation)
	EVENTS_REGISTER_HANDLER(ParticleManager,ItemCatch)
	EVENTS_REGISTER_HANDLER(ParticleManager,ProjectileUpdate)
	EVENTS_REGISTER_HANDLER(ParticleManager,ProjectileHit)
	EVENTS_REGISTER_HANDLER(ParticleManager,ObjectHit)
	EVENTS_REGISTER_HANDLER(ParticleManager,ObjectKilled)
	EVENTS_REGISTER_HANDLER(ParticleManager,ObjectCustom)
EVENTS_END_REGISTER_HANDLERS()

EVENTS_BEGIN_UNREGISTER_HANDLERS(ParticleManager)
	EVENTS_UNREGISTER_HANDLER(ParticleManager,PlayerAttack)
	EVENTS_UNREGISTER_HANDLER(ParticleManager,PlayerHit)
	EVENTS_UNREGISTER_HANDLER(ParticleManager,EnemyAttack)
	EVENTS_UNREGISTER_HANDLER(ParticleManager,EnemyHit)
	EVENTS_UNREGISTER_HANDLER(ParticleManager,EnemyKilled)
	EVENTS_UNREGISTER_HANDLER(ParticleManager,EnemyCustom)
	EVENTS_UNREGISTER_HANDLER(ParticleManager,ItemCreation)
	EVENTS_UNREGISTER_HANDLER(ParticleManager,ItemCatch)
	EVENTS_UNREGISTER_HANDLER(ParticleManager,ProjectileUpdate)
	EVENTS_UNREGISTER_HANDLER(ParticleManager,ProjectileHit)
	EVENTS_UNREGISTER_HANDLER(ParticleManager,ObjectHit)
	EVENTS_UNREGISTER_HANDLER(ParticleManager,ObjectKilled)
	EVENTS_UNREGISTER_HANDLER(ParticleManager,ObjectCustom)
EVENTS_END_REGISTER_HANDLERS()


EVENTS_DEFINE_HANDLER(ParticleManager, PlayerAttack)
{
	PlayerPtr player = evt->getPlayer();

	SceneNode* sceneNode = player->_getSceneNode();

	//this->blast(sceneNode);
}

EVENTS_DEFINE_HANDLER(ParticleManager, PlayerHit)
{
	PlayerPtr player = evt->getPlayer();

	SceneNode* sceneNode = player->_getSceneNode();

	this->add(sceneNode, "WyvernsAssault/Impact");
}

EVENTS_DEFINE_HANDLER(ParticleManager, EnemyAttack)
{
	EnemyPtr enemy = evt->getEnemy();

	SceneNode* sceneNode = enemy->_getSceneNode();

	//this->swing(sceneNode);
}

EVENTS_DEFINE_HANDLER(ParticleManager, EnemyHit)
{
	EnemyPtr enemy = evt->getEnemy();
	PlayerPtr player = evt->getPlayer();	

	if( player->isSpecial() )
	{
		this->add(enemy->_getSceneNode(), "WyvernsAssault/FireHit");
	}
	else
	{
		if( enemy->getEnemyType() == Enemy::EnemyTypes::BatteringRam )
			this->add(enemy->_getSceneNode(), "WyvernsAssault/Hit");
		else
		{
			this->add(enemy->_getSceneNode(), "WyvernsAssault/BloodHit");	
			this->add(enemy->_getSceneNode(), "WyvernsAssault/Hit");	
		}
	}
}

EVENTS_DEFINE_HANDLER(ParticleManager, EnemyKilled)
{
	EnemyPtr enemy = evt->getEnemy();
	PlayerPtr player = evt->getPlayer();	

	if( !enemy->isBurning() )
	{
		if( enemy->getEnemyType() == Enemy::EnemyTypes::Chicken )
			this->add(enemy->_getSceneNode(), "WyvernsAssault/ChickenKill");

		if( enemy->getEnemyType() == Enemy::EnemyTypes::Cow )
			this->add(enemy->_getSceneNode(), "WyvernsAssault/CowKill");

		else if( enemy->isFlying() )
		{
			this->add(enemy->_getSceneNode(), "WyvernsAssault/BloodHit");
			this->bloodLens();
		}
		else
		{
			if( enemy->getEnemyType() == Enemy::EnemyTypes::BatteringRam )
				this->add(enemy->_getSceneNode(), "WyvernsAssault/Hit");
			else
				this->add(enemy->_getSceneNode(), "WyvernsAssault/BloodKill");
		}
	}
}

EVENTS_DEFINE_HANDLER(ParticleManager, EnemyCustom)
{
	EnemyPtr enemy = evt->getEnemy();

	if( enemy->isBurning())
	{
		if( enemy->getEnemyType() == Enemy::EnemyTypes::BatteringRam )
			this->add(enemy->_getSceneNode(), "WyvernsAssault/FireKillObject");
		else
			this->add(enemy->_getSceneNode(), "WyvernsAssault/FireKill");
	}
}

EVENTS_DEFINE_HANDLER(ParticleManager, ItemCreation)
{
	ItemPtr item = evt->getItem();

	this->add(item->_getSceneNode(), item->getName() + "_glow", "WyvernsAssault/ItemGlow");

	if ( item->getType() == Item::ItemTypes::Life )
		this->add(item->_getSceneNode(), item->getName(), "WyvernsAssault/Heart");
	else if ( item->getType() == Item::ItemTypes::Drunk )
		this->add(item->_getSceneNode(), item->getName(), "WyvernsAssault/Skull");
	else if ( item->getType() == Item::ItemTypes::Points )
		this->add(item->_getSceneNode(), item->getName(), "WyvernsAssault/Money");
}

EVENTS_DEFINE_HANDLER(ParticleManager, ItemCatch)
{
	ItemPtr item = evt->getItem();

	SceneNode* sceneNode = item->_getSceneNode();

	this->remove(item->getName() + "_glow");
	this->remove(item->getName());
	this->add(sceneNode, "WyvernsAssault/GlowShort");
}

EVENTS_DEFINE_HANDLER(ParticleManager, ProjectileUpdate)
{
	ProjectilePtr projectile = evt->getProjectile();

	// Add particles to projectile
	if( evt->getType() == Enemy::EnemyTypes::Wizard )
		this->add(projectile->_getSceneNode(), projectile->getName(), "WyvernsAssault/MagicBolt");
}

EVENTS_DEFINE_HANDLER(ParticleManager, ProjectileHit)
{
	PlayerPtr player = evt->getPlayer();

	SceneNode* sceneNode = player->_getSceneNode();

	this->add(sceneNode, "WyvernsAssault/Impact");
}

EVENTS_DEFINE_HANDLER(ParticleManager, ObjectHit)
{
	ObjectPtr object = evt->getObject();
	PlayerPtr player = evt->getPlayer();	

	if( player->isSpecial() )
		this->add(object->_getSceneNode(), "WyvernsAssault/FireHitObject");
	else
		this->add(object->_getSceneNode(), "WyvernsAssault/Hit");	
}

EVENTS_DEFINE_HANDLER(ParticleManager, ObjectKilled)
{
	ObjectPtr object = evt->getObject();
	PlayerPtr player = evt->getPlayer();	

	if( !object->isBurning() )		
		this->add(object->_getSceneNode(), "WyvernsAssault/Hit");
}

EVENTS_DEFINE_HANDLER(ParticleManager, ObjectCustom)
{
	ObjectPtr object = evt->getObject();

	if( object->isBurning())
		this->add(object->_getSceneNode(), "WyvernsAssault/FireKillObject");
}

// --------------------------------
// Lua Particle Lib
// --------------------------------
LUA_BEGIN_BINDING(ParticleManager, particlelib)
LUA_BIND(ParticleManager, createSystem)
LUA_BIND(ParticleManager, removeSystem)
LUA_END_BINDING()

//
// Load lua scripts that will be used by this manager
//
LUA_BEGIN_LOAD_SCRIPTS(ParticleManager)
// 
// TODO : Load scripts if needed
//
LUA_END_LOAD_SCRIPTS()

LUA_DEFINE_FUNCTION(ParticleManager, createSystem)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 1;
}

LUA_DEFINE_FUNCTION(ParticleManager, removeSystem)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 1;
}

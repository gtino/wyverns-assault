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

	// Create "all-time" particle systems
	mParticleSystem = mParticleSystemManager->createParticleSystem("somkeA", "WyvernsAssault/Smoke", mSceneManager);
	mSceneManager->getRootSceneNode()->createChildSceneNode("smokeNodeA",Vector3(-545,690,490))->attachObject(mParticleSystem);
	mParticleSystem->start();

	mParticleSystem = mParticleSystemManager->createParticleSystem("smokeB", "WyvernsAssault/Smoke", mSceneManager);	
	mSceneManager->getRootSceneNode()->createChildSceneNode("smokeNodeB",Vector3(-615,690,450))->attachObject(mParticleSystem);
	mParticleSystem->start();

	mParticleSystem = mParticleSystemManager->createParticleSystem("bloodLens", "WyvernsAssault/BloodLens", mSceneManager);
	mSceneManager->getRootSceneNode()->createChildSceneNode("bloodLensNode", Vector3::ZERO)->attachObject(mParticleSystem);
}


/** Finalize the particle manager */
void ParticleManager::finalize()
{
	mParticleSystemManager->destroyAllParticleSystems(mSceneManager);

	mParticleSystem = NULL;
	mParticleSystemManager = NULL;

	mSceneManager->destroySceneNode("smokeNodeA");
	mSceneManager->destroySceneNode("smokeNodeB");
	mSceneManager->destroySceneNode("bloodLensNode");
}

void ParticleManager::update(const float elapsedSeconds)
{

}

/** Create particle system function */		
ParticleUniverse::ParticleSystem* ParticleManager::create(String id, String script)
{
	return mParticleSystemManager->createParticleSystem(id, script, mSceneManager);
}

/** Add particle system to node and start it */
void ParticleManager::add(SceneNode* node, String id, String script)
{
	ParticleUniverse::ParticleSystem* pSystem = mParticleSystemManager->createParticleSystem(id + "_particle", script, mSceneManager);
	node->attachObject( pSystem );
	pSystem->start();
}

/** Remove particle system from node and stop it */
void ParticleManager::remove(SceneNode* node, String id)
{
	ParticleUniverse::ParticleSystem* pSystem = mParticleSystemManager->getParticleSystem(id + "_particle");
	pSystem->stop();
	pSystem->detachFromParent();
	mParticleSystemManager->destroyParticleSystem(pSystem, mSceneManager);
}

/** Blood particles */
void ParticleManager::bloodHit(SceneNode* node)
{
	String name = node->getName();
	ParticleUniverse::ParticleSystem* particles;
	if( mParticleSystemManager->getParticleSystem(name + "_bloodHit") == NULL)
	{
		particles = mParticleSystemManager->createParticleSystem( name + "_bloodHit", "WyvernsAssault/BloodHit", mSceneManager);
		node->attachObject( particles );
	}
	else
	{
		particles = mParticleSystemManager->getParticleSystem( name + "_bloodHit");
	}	
	particles->startAndStopFade(2);
}

void ParticleManager::bloodKill(SceneNode* node)
{
	String name = node->getName();
	ParticleUniverse::ParticleSystem* particles;
	if( mParticleSystemManager->getParticleSystem(name + "_bloodKill") == NULL)
	{
		particles = mParticleSystemManager->createParticleSystem( name + "_bloodKill", "WyvernsAssault/BloodKill", mSceneManager);
		node->attachObject( particles );
	}
	else
	{
		particles = mParticleSystemManager->getParticleSystem( name + "_bloodKill");
	}	
	particles->startAndStopFade(2);
}

void ParticleManager::bloodLens()
{
	Vector3 position = mSceneManager->getCamera(GAME_CAMERA)->getParentSceneNode()->getPosition();
	Vector3 direction = mSceneManager->getCamera(GAME_CAMERA)->getDirection();
	SceneNode* node = mSceneManager->getSceneNode("bloodLensNode");
	node->setPosition(position + direction * 170);	
	ParticleUniverse::ParticleSystem* particles = mParticleSystemManager->getParticleSystem("bloodLens");
	particles->startAndStopFade(5);
}

/** Hit particle */
void ParticleManager::hit(SceneNode* node)
{
	String name = node->getName();
	ParticleUniverse::ParticleSystem* particles;
	if( mParticleSystemManager->getParticleSystem(name + "_hit") == NULL)
	{
		particles = mParticleSystemManager->createParticleSystem( name + "_hit", "WyvernsAssault/Hit", mSceneManager);
		node->attachObject( particles );
	}
	else
	{
		particles = mParticleSystemManager->getParticleSystem( name + "_hit");
	}	
	particles->startAndStopFade(1);
}

/** Fire hit particle */
void ParticleManager::fireHit(SceneNode* node)
{
	String name = node->getName();
	ParticleUniverse::ParticleSystem* particles;
	if( mParticleSystemManager->getParticleSystem(name + "_fireHit") == NULL)
	{
		particles = mParticleSystemManager->createParticleSystem( name + "_fireHit", "WyvernsAssault/FireHit", mSceneManager);
		node->attachObject( particles );
	}
	else
	{
		particles = mParticleSystemManager->getParticleSystem( name + "_fireHit");
	}	
	particles->startAndStopFade(2);
}

/** Fire kill particle */
void ParticleManager::fireKill(SceneNode* node)
{
	String name = node->getName();
	ParticleUniverse::ParticleSystem* particles;
	if( mParticleSystemManager->getParticleSystem(name + "_fireKill") == NULL)
	{
		particles = mParticleSystemManager->createParticleSystem( name + "_fireKill", "WyvernsAssault/FireKill", mSceneManager);
		node->attachObject( particles );
	}
	else
	{
		particles = mParticleSystemManager->getParticleSystem( name + "_fireKill");
	}	
	particles->startAndStopFade(2);
}

/** Chicken kill particle **/
void ParticleManager::chickenKill(SceneNode* node)
{
	String name = node->getName();
	ParticleUniverse::ParticleSystem* particles;
	if( mParticleSystemManager->getParticleSystem(name + "_chickeKill") == NULL)
	{
		particles = mParticleSystemManager->createParticleSystem( name + "_chickeKill", "WyvernsAssault/ChickenKill", mSceneManager);
		node->attachObject( particles );
	}
	else
	{
		particles = mParticleSystemManager->getParticleSystem( name + "_chickeKill");
	}	
	particles->startAndStopFade(2);
}

/** Glow particle */
void ParticleManager::glow(SceneNode* node)
{
	String name = node->getName();
	ParticleUniverse::ParticleSystem* particles;
	if( mParticleSystemManager->getParticleSystem(name + "_glow") == NULL)
	{
		particles = mParticleSystemManager->createParticleSystem( name + "_glow", "WyvernsAssault/GlowShort", mSceneManager);
		node->attachObject( particles );
	}
	else
	{
		particles = mParticleSystemManager->getParticleSystem( name + "_glow");
	}	
	particles->startAndStopFade(2);
}


// --------------
// Event handlers
// --------------
EVENTS_BEGIN_REGISTER_HANDLERS(ParticleManager)
	EVENTS_REGISTER_HANDLER(ParticleManager,EnemyHit)
	EVENTS_REGISTER_HANDLER(ParticleManager,EnemyKilled)
	EVENTS_REGISTER_HANDLER(ParticleManager,ItemCatch)
EVENTS_END_REGISTER_HANDLERS()

EVENTS_BEGIN_UNREGISTER_HANDLERS(ParticleManager)
	EVENTS_UNREGISTER_HANDLER(ParticleManager,EnemyHit)
	EVENTS_UNREGISTER_HANDLER(ParticleManager,EnemyKilled)
	EVENTS_UNREGISTER_HANDLER(ParticleManager,ItemCatch)
EVENTS_END_REGISTER_HANDLERS()

EVENTS_DEFINE_HANDLER(ParticleManager,EnemyHit)
{
	EnemyPtr enemy = evt->getEnemy();
	PlayerPtr player = evt->getPlayer();	

	if( enemy->getEnemyType() == Enemy::EnemyTypes::Chicken )
		this->chickenKill(enemy->_getSceneNode());
	else
	{
		if( player->isSpecial() )
			this->fireHit(enemy->_getSceneNode());
		else
		{
			this->bloodHit(enemy->_getSceneNode());	
			this->hit(enemy->_getSceneNode());	
		}
	}
}

EVENTS_DEFINE_HANDLER(ParticleManager, EnemyKilled)
{
	EnemyPtr enemy = evt->getEnemy();
	PlayerPtr player = evt->getPlayer();	

	if( enemy->getEnemyType() == Enemy::EnemyTypes::Chicken )
		this->chickenKill(enemy->_getSceneNode());
	else
	{
		if( player->isSpecial() )
			this->fireKill(enemy->_getSceneNode());
		else
			this->bloodKill(enemy->_getSceneNode());	
	}
}

EVENTS_DEFINE_HANDLER(ParticleManager, ItemCatch)
{
	ItemPtr item = evt->getItem();

	SceneNode* sceneNode = item->_getSceneNode();

	this->remove(sceneNode, item->getName());
	this->glow(sceneNode);
}

// --------------------------------
// Lua Camera Lib
// --------------------------------
LUA_BEGIN_BINDING(ParticleManager::particlelib)
LUA_BIND("create", ParticleManager::luaCreateSystem)
LUA_BIND("remove", ParticleManager::luaRemoveSystem)
LUA_END_BINDING()

int ParticleManager::luaCreateSystem(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 1;
}

int ParticleManager::luaRemoveSystem(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 1;
}

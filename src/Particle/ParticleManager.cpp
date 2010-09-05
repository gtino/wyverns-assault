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

	mParticleSystem = mParticleSystemManager->createParticleSystem("fireCampA", "WyvernsAssault/FireCamp", mSceneManager);	
	mSceneManager->getRootSceneNode()->createChildSceneNode("fireCampA",Vector3(522, 0, 720))->attachObject(mParticleSystem);
	mParticleSystem->start();

	mParticleSystem = mParticleSystemManager->createParticleSystem("fireCampB", "WyvernsAssault/FireCamp", mSceneManager);	
	mSceneManager->getRootSceneNode()->createChildSceneNode("fireCampB",Vector3(594.5, 0, 787))->attachObject(mParticleSystem);
	mParticleSystem->start();

	mParticleSystem = mParticleSystemManager->createParticleSystem("fireCampC", "WyvernsAssault/FireCamp", mSceneManager);	
	mSceneManager->getRootSceneNode()->createChildSceneNode("fireCampC",Vector3(1171.5, 0, 178))->attachObject(mParticleSystem);
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
	particles->start();
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
	particles->start();
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
	particles->start();
}

void ParticleManager::impact(SceneNode* node)
{
	String name = node->getName();
	ParticleUniverse::ParticleSystem* particles;
	if( mParticleSystemManager->getParticleSystem(name + "_impact") == NULL)
	{
		particles = mParticleSystemManager->createParticleSystem( name + "_impact", "WyvernsAssault/Impact", mSceneManager);
		node->attachObject( particles );
	}
	else
	{
		particles = mParticleSystemManager->getParticleSystem( name + "_impact");
	}	
	particles->start();
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
	particles->start();
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
	particles->start();
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
	particles->start();
}

/** Cow kill particle **/
void ParticleManager::cowKill(SceneNode* node)
{
	String name = node->getName();
	ParticleUniverse::ParticleSystem* particles;
	if( mParticleSystemManager->getParticleSystem(name + "_cowKill") == NULL)
	{
		particles = mParticleSystemManager->createParticleSystem( name + "_cowKill", "WyvernsAssault/CowKill", mSceneManager);
		node->attachObject( particles );
	}
	else
	{
		particles = mParticleSystemManager->getParticleSystem( name + "_cowKill");
	}	
	particles->start();
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
	particles->start();
}

/** Attack particles **/
void ParticleManager::blast(SceneNode* node)
{
	String name = node->getName();
	ParticleUniverse::ParticleSystem* particles;
	if( mParticleSystemManager->getParticleSystem(name + "_blast") == NULL)
	{
		particles = mParticleSystemManager->createParticleSystem( name + "_blast", "WyvernsAssault/Blast", mSceneManager);
		node->attachObject( particles );
	}
	else
	{
		particles = mParticleSystemManager->getParticleSystem( name + "_blast");
	}	
	particles->start();
}

void ParticleManager::swing(SceneNode* node)
{
	String name = node->getName();
	ParticleUniverse::ParticleSystem* particles;
	if( mParticleSystemManager->getParticleSystem(name + "_swing") == NULL)
	{
		particles = mParticleSystemManager->createParticleSystem( name + "_swing", "WyvernsAssault/Swing", mSceneManager);
		node->attachObject( particles );
	}
	else
	{
		particles = mParticleSystemManager->getParticleSystem( name + "_swing");
	}	
	particles->start();
}

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
	EVENTS_REGISTER_HANDLER(ParticleManager,ProjectileFire)
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
	EVENTS_UNREGISTER_HANDLER(ParticleManager,ProjectileFire)
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

	this->impact(sceneNode);
}

EVENTS_DEFINE_HANDLER(ParticleManager, EnemyAttack)
{
	EnemyPtr enemy = evt->getEnemy();

	SceneNode* sceneNode = enemy->_getSceneNode();

	//this->swing(sceneNode);
}

EVENTS_DEFINE_HANDLER(ParticleManager,EnemyHit)
{
	EnemyPtr enemy = evt->getEnemy();
	PlayerPtr player = evt->getPlayer();	

	if( player->isSpecial() )
	{
		this->fireHit(enemy->_getSceneNode());
	}
	else
	{
		this->bloodHit(enemy->_getSceneNode());	
		this->hit(enemy->_getSceneNode());	
	}
}

EVENTS_DEFINE_HANDLER(ParticleManager, EnemyKilled)
{
	EnemyPtr enemy = evt->getEnemy();
	PlayerPtr player = evt->getPlayer();	

	if( !enemy->isBurning() )
	{
		if( enemy->getEnemyType() == Enemy::EnemyTypes::Chicken )
			this->chickenKill(enemy->_getSceneNode());

		if( enemy->getEnemyType() == Enemy::EnemyTypes::Cow )
			this->cowKill(enemy->_getSceneNode());

		else if( enemy->isFlying() )
		{
			this->bloodHit(enemy->_getSceneNode());
			this->bloodLens();
		}
		else
			this->bloodKill(enemy->_getSceneNode());
	}
}

EVENTS_DEFINE_HANDLER(ParticleManager, EnemyCustom)
{
	EnemyPtr enemy = evt->getEnemy();

	if( enemy->isBurning())
		this->fireKill(enemy->_getSceneNode());
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

	this->remove(sceneNode, item->getName()+ "_glow");
	this->remove(sceneNode, item->getName());
	this->glow(sceneNode);
}

EVENTS_DEFINE_HANDLER(ParticleManager, ProjectileFire)
{
	//this->magicBolt(sceneNode);
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

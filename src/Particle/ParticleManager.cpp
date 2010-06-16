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
	particles->startAndStopFade(1);
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
	particles->startAndStopFade(1);
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

/** Hit particle */
void ParticleManager::hitSpecial(SceneNode* node)
{
	String name = node->getName();
	ParticleUniverse::ParticleSystem* particles;
	if( mParticleSystemManager->getParticleSystem(name + "_hitSpecial") == NULL)
	{
		particles = mParticleSystemManager->createParticleSystem( name + "_hitSpecial", "WyvernsAssault/FireHit", mSceneManager);
		node->attachObject( particles );
	}
	else
	{
		particles = mParticleSystemManager->getParticleSystem( name + "_hitSpecial");
	}	
	particles->startAndStopFade(1);
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
	particles->startAndStopFade(1);
}


// --------------
// Event handlers
// --------------
void ParticleManager::registerHandlers()
{
	boost::shared_ptr<ParticleManager> this_ = shared_from_this();

	registerHandler(EventHandlerPtr(new EventHandler<ParticleManager,EnemyHitEvent>(this_,&ParticleManager::handleEnemyHitEvent)),EventTypes::EnemyHit);
	registerHandler(EventHandlerPtr(new EventHandler<ParticleManager,EnemyKillEvent>(this_,&ParticleManager::handleEnemyKillEvent)),EventTypes::EnemyKill);
}

void ParticleManager::unregisterHandlers()
{
}

void ParticleManager::handleEnemyHitEvent(EnemyHitEventPtr evt)
{
	EnemyPtr enemy = evt->getEnemy();
	PlayerPtr player = evt->getPlayer();	

	if( player->isSpecial() )
	{
		this->hitSpecial(enemy->_getSceneNode());
	}
	else
	{
		this->bloodHit(enemy->_getSceneNode());	
		this->hit(enemy->_getSceneNode());	
	}
}

void ParticleManager::handleEnemyKillEvent(EnemyKillEventPtr evt)
{
	EnemyPtr enemy = evt->getEnemy();
	PlayerPtr player = evt->getPlayer();	

	if( player->isSpecial() )
	{
		this->hitSpecial(enemy->_getSceneNode());
	}
	else
		this->bloodKill(enemy->_getSceneNode());	
}
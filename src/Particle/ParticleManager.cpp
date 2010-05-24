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

	/** EJEMPLO **/
	mParticleSystem = mParticleSystemManager->createParticleSystem("somkeA", "WyvernsAssault/Smoke", mSceneManager);
	mSceneManager->getRootSceneNode()->createChildSceneNode("smokeNodeA",Vector3(-545,690,490))->attachObject(mParticleSystem);
	mParticleSystem->start();

	mParticleSystem = mParticleSystemManager->createParticleSystem("smokeB", "WyvernsAssault/Smoke", mSceneManager);	
	mSceneManager->getRootSceneNode()->createChildSceneNode("smokeNodeB",Vector3(-615,690,450))->attachObject(mParticleSystem);
	mParticleSystem->start();

	mParticleSystem = mParticleSystemManager->createParticleSystem("bloodHit", "WyvernsAssault/BloodHit", mSceneManager);	
	mSceneManager->getRootSceneNode()->createChildSceneNode("bloodHitNode", Vector3::ZERO)->attachObject(mParticleSystem);	

	mParticleSystem = mParticleSystemManager->createParticleSystem("bloodKill", "WyvernsAssault/BloodKill", mSceneManager);	
	mSceneManager->getRootSceneNode()->createChildSceneNode("bloodKillNode", Vector3::ZERO)->attachObject(mParticleSystem);	
	/*************/

}


/** Finalize the particle manager */
void ParticleManager::finalize()
{
	ParticleUniverse::ParticleSystemManager::getSingletonPtr()->destroyAllParticleSystems(mSceneManager);

	mParticleSystem = NULL;
	mParticleSystemManager = NULL;
}

void ParticleManager::update(const float elapsedSeconds)
{

}

/** Create particle system function */		
void ParticleManager::createParticle(String name)
{

}

/** Blood particles */
void ParticleManager::bloodHit(Vector3 position)
{
	SceneNode* bloodNode = mSceneManager->getSceneNode("bloodHitNode");
	bloodNode->setPosition(position);
	ParticleUniverse::ParticleSystem* blood = mParticleSystemManager->getParticleSystem("bloodHit");
	blood->startAndStopFade(1);
}

void ParticleManager::bloodKill(Vector3 position)
{
	SceneNode* bloodNode = mSceneManager->getSceneNode("bloodKillNode");
	bloodNode->setPosition(position);
	ParticleUniverse::ParticleSystem* blood = mParticleSystemManager->getParticleSystem("bloodKill");
	blood->startAndStopFade(1);
}

// --------------
// Event handlers
// --------------
void ParticleManager::registerHandlers()
{
	boost::shared_ptr<ParticleManager> this_ = shared_from_this();

	registerHandler(EventHandlerPtr(new EventHandler<ParticleManager,EnemyHitEvent>(this_,&ParticleManager::handleEnemyHitEvent)),EventTypes::EnemyHit);
}

void ParticleManager::unregisterHandlers()
{
}

void ParticleManager::handleEnemyHitEvent(EnemyHitEventPtr evt)
{
	EnemyPtr enemy = evt->getEnemy();
	PlayerPtr player = evt->getPlayer();	

	// The player has just hit the enemy	
	this->bloodHit(enemy->getPosition());
}
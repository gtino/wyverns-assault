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

	mParticleSystem = mParticleSystemManager->createParticleSystem("bloodLens", "WyvernsAssault/BloodLens", mSceneManager);
	mSceneManager->getRootSceneNode()->createChildSceneNode("bloodLensNode", Vector3::ZERO)->attachObject(mParticleSystem);

	mParticleSystem = mParticleSystemManager->createParticleSystem("hit", "WyvernsAssault/Hit", mSceneManager);
	mSceneManager->getRootSceneNode()->createChildSceneNode("hitNode", Vector3::ZERO)->attachObject(mParticleSystem);
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
ParticleUniverse::ParticleSystem* ParticleManager::create(String id, String material)
{
	return ParticleUniverse::ParticleSystemManager::getSingletonPtr()->createParticleSystem(id, material, mSceneManager);
}

/** Blood particles */
void ParticleManager::bloodHit(Vector3 position)
{
	SceneNode* node = mSceneManager->getSceneNode("bloodHitNode");
	node->setPosition(position);
	ParticleUniverse::ParticleSystem* particles = mParticleSystemManager->getParticleSystem("bloodHit");
	particles->startAndStopFade(5);
}

void ParticleManager::bloodKill(Vector3 position)
{
	SceneNode* node = mSceneManager->getSceneNode("bloodKillNode");
	node->setPosition(position);
	ParticleUniverse::ParticleSystem* particles = mParticleSystemManager->getParticleSystem("bloodKill");
	particles->startAndStopFade(5);
}

void ParticleManager::bloodLens()
{
	Vector3 position = mSceneManager->getCamera("GameCamera")->getParentSceneNode()->getPosition();
	Vector3 direction = mSceneManager->getCamera("GameCamera")->getDirection();
	SceneNode* node = mSceneManager->getSceneNode("bloodLensNode");
	node->setPosition(position + direction * 200);	
	ParticleUniverse::ParticleSystem* particles = mParticleSystemManager->getParticleSystem("bloodLens");
	particles->startAndStopFade(5);
}

/** Hit particle */

void ParticleManager::hit(Vector3 position)
{
	SceneNode* node = mSceneManager->getSceneNode("hitNode");
	node->setPosition(position);
	ParticleUniverse::ParticleSystem* particles = mParticleSystemManager->getParticleSystem("hit");
	particles->startAndStopFade(5);
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
	if(player->isAttacking())
	{
		this->bloodHit(enemy->getPosition());
		this->hit(enemy->getPosition());
	}
}

void ParticleManager::handlePlayerHitEvent(PlayerHitEventPtr evt)
{
	EnemyPtr enemy = evt->getEnemy();
	PlayerPtr player = evt->getPlayer();	

	// The player has just hit the enemy	
	this->hit(player->getPosition());
}
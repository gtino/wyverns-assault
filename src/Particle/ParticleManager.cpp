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
	/*************/
}


/** Finalize the particle manager */
void ParticleManager::finalize()
{
	// Stop, detach and destroy particle systems

	/** EJEMPLO - No funciona !! **/
	/*mParticleSystem = mParticleSystemManager->getParticleSystem("somkeA");
	mParticleSystem->stop();
	mSceneManager->getSceneNode("smokeNodeA")->detachObject(mParticleSystem, mSceneManager);
	mParticleSystemManager->destroyParticleSystem(mParticleSystem);

	mParticleSystem = mParticleSystemManager->getParticleSystem("somkeB");
	mParticleSystem->stop();
	mSceneManager->getSceneNode("smokeNodeB")->detachObject(mParticleSystem, mSceneManager);
	mParticleSystemManager->destroyParticleSystem(mParticleSystem);*/
	/*************/

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

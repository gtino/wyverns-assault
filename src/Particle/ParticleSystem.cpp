#include "..\include\Particle\ParticleManager.h"

using namespace WyvernsAssault;

WyvernsAssault::ParticleSystem::ParticleSystem(Ogre::String id, Ogre::String script)
{
}

WyvernsAssault::ParticleSystem::~ParticleSystem()
{
}
		
void WyvernsAssault::ParticleSystem::start(void)
{
	mParticleSystem->start();
}

void WyvernsAssault::ParticleSystem::startAndStopFade(Ogre::Real stopTime)
{
	mParticleSystem->startAndStopFade(stopTime);
}

void WyvernsAssault::ParticleSystem::stop(void)
{
	mParticleSystem->stop();
}

void WyvernsAssault::ParticleSystem::update(const float elapsedSeconds)
{

}
	
void WyvernsAssault::ParticleSystem::attachTo(SceneNode* node)
{
}

void WyvernsAssault::ParticleSystem::detachFrom(SceneNode* node)
{
}
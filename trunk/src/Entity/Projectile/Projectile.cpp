#include "..\..\..\include\Entity\Projectile\Projectile.h"

using namespace WyvernsAssault;

Projectile::Projectile(Ogre::String name, Ogre::Vector3 init, Ogre::Vector3 finish)
: EntityInterface(name)
, PhysicsInterface()
, mOBBoxRenderable(0)
, aliveTime(0)
, live(true)
, mIsDebugEnabled(false)
{
	initPoint = init;
	finishPoint = finish;
}

Projectile::~Projectile()
{
	finalizeEntity();
}

void Projectile::initializeEntity(Ogre::Entity* entity, Ogre::SceneNode* sceneNode, Ogre::SceneManager* sceneManager)
{

	// Always call base method before!
	EntityInterface::initializeEntity(entity, sceneNode, sceneManager);

	// Set physic body
	initializePhysics(entity->getName(), Vector3(10,10,10),"OBBoxManualMaterial_Enemy");
	sceneNode->attachObject(getGeometry("collision")->getMovableObject());

	// Set direction of projectile
	mDirection = finishPoint - initPoint;
	mDirection.normalise();
}

void Projectile::finalizeEntity()
{
	// Always call base method before!
	EntityInterface::finalizeEntity();
}

void Projectile::setDebugEnabled(bool isDebugEnabled)
{
	if(mIsDebugEnabled != isDebugEnabled)
	{
		mIsDebugEnabled = isDebugEnabled;
		getGeometry("collision")->getMovableObject()->setVisible(mIsDebugEnabled);
	}
}

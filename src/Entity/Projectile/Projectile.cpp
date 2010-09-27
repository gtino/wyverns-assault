#include "..\..\..\include\Entity\Projectile\Projectile.h"

using namespace WyvernsAssault;

Projectile::Projectile(Ogre::String name, Ogre::Vector3 init, Ogre::Vector3 finish)
: EntityInterface(name)
, PhysicsInterface()
, mOBBoxRenderable(0)
, mAliveTime(0.0)
, mDuration(PROJECTILE_LIVE_TIME)
, mLive(true)
, mDamage(0.0)
, mSpeed(PROJECTILE_SPEED)
, mIsDebugEnabled(false)
{
	mInitPoint = init;
	mFinishPoint = finish;
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
	mOBBoxRenderable = new OBBoxRenderable("OBBoxManualMaterial_Enemy");
	initializePhysics( entity->getName(), Vector3(10,10,10), "OBBoxManualMaterial_Enemy" );	
	sceneNode->attachObject( getGeometry(PhysicBoxType::body)->getMovableObject() );

	// Set direction of projectile
	mDirection = mFinishPoint - mInitPoint;
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
		getGeometry(PhysicBoxType::body)->getMovableObject()->setVisible(mIsDebugEnabled);
	}
}

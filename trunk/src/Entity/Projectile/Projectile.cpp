#include "..\..\..\include\Entity\Projectile\Projectile.h"

using namespace WyvernsAssault;

Projectile::Projectile(Ogre::String name, Ogre::Vector3 init, Ogre::Vector3 finish)
: EntityInterface(name)
, PhysicsInterface()
, mOBBoxRenderable(0)
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
	initializePhysics(Vector3(20,20,20));

	// Set direction of projectile
	mDirection = finishPoint - initPoint;

	// Bounding Box
	//mOBBoxRenderable = new OBBoxRenderable("OBBoxManualMaterial_Object");
	//mOBBoxRenderable->setupVertices(entity->getBoundingBox());
	//mOBBoxRenderable->setVisible(mIsDebugEnabled);
	//mSceneNode->attachObject(mOBBoxRenderable);
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

		if(mOBBoxRenderable)
			mOBBoxRenderable->setVisible(mIsDebugEnabled);
	}
}

#include "..\..\include\Scenario\Object.h"

using namespace WyvernsAssault;

Object::Object(Ogre::String name, ObjectTypes type) 
: EntityInterface(name)
, PhysicsInterface()
, mOBBoxRenderable(0)
, mIsDebugEnabled(false)
{
	mType = type;
}


Object::~Object()
{
	finalizeEntity();
}

void Object::initializeEntity(Ogre::Entity* entity, Ogre::SceneNode* sceneNode, Ogre::SceneManager* sceneManager)
{
	// Always call base method before!
	EntityInterface::initializeEntity(entity, sceneNode, sceneManager);

	if(mType == WyvernsAssault::ObjectTypes::DynamicObject){
		// Bounding Box
		mOBBoxRenderable = new OBBoxRenderable("OBBoxManualMaterial_Object");

		mOBBoxRenderable->setupVertices(mEntity->getBoundingBox());
		mOBBoxRenderable->setVisible(mIsDebugEnabled);
		mSceneNode->attachObject(mOBBoxRenderable);
	}
}

void Object::finalizeEntity()
{
	// Always call base method before!
	EntityInterface::finalizeEntity();
}

ObjectTypes Object::StringToType (Ogre::String string)
{
	const char* str = string.c_str();

	if(strcmp ( "Default", str ) == 0) return ObjectTypes::Default;

	return ObjectTypes::Default;
}

void Object::setDebugEnabled(bool isDebugEnabled)
{
	if(mIsDebugEnabled != isDebugEnabled)
	{
		mIsDebugEnabled = isDebugEnabled;

		if(mOBBoxRenderable)
			mOBBoxRenderable->setVisible(mIsDebugEnabled);
	}
}

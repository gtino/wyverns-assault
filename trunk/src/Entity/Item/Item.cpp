#include "..\..\..\include\Entity\Item\Item.h"

using namespace WyvernsAssault;

Item::Item(Ogre::String name, Item::ItemTypes type, Item::ItemParameters params) 
: EntityInterface(name)
, PhysicsInterface()
, mOBBoxRenderable(0)
, mIsDebugEnabled(false)
{
	mType = type;
	mParameters = params;
}

Item::~Item()
{
	finalizeEntity();
}

void Item::initializeEntity(Ogre::Entity* entity, Ogre::SceneNode* sceneNode, Ogre::SceneManager* sceneManager)
{
	// Always call base method before!
	EntityInterface::initializeEntity(entity, sceneNode, sceneManager);
	
	// Initialize item physics
	initializePhysics(entity->getName(), Vector3(20,20,20),"OBBoxManualMaterial_Item");
	sceneNode->attachObject(getGeometry()->getMovableObject());

}

void Item::finalizeEntity()
{
	// Always call base method before!
	EntityInterface::finalizeEntity();
}

void Item::updateEntity(const float elapsedSeconds)
{
	mAnimationState = mEntity->getAnimationState("Iddle");
	mAnimationState->setEnabled(true);
	mAnimationState->addTime(elapsedSeconds);
	return;
}

void Item::updateLogic(const float elapsedSeconds)
{
}

void Item::caught()
{
	mEntity->setVisible(false);
}

Item::ItemTypes Item::StringToType (Ogre::String string)
{
	const char* str = string.c_str();

	if(strcmp ( "Life", str ) == 0) return Item::ItemTypes::Life;	
	if(strcmp ( "Points", str ) == 0) return Item::ItemTypes::Points;
	if(strcmp ( "Drunk", str ) == 0) return Item::ItemTypes::Drunk;

	return Item::ItemTypes::Life;
}


void Item::setDebugEnabled(bool isDebugEnabled)
{
	if(mIsDebugEnabled != isDebugEnabled)
	{
		mIsDebugEnabled = isDebugEnabled;
		getGeometry()->getMovableObject()->setVisible(mIsDebugEnabled);
	}
}
#include "..\..\..\include\Entity\Item\Item.h"

using namespace WyvernsAssault;

Item::Item(Ogre::String name, Item::ItemTypes type) 
: EntityInterface(name)
, PhysicsInterface()
, mLife(0.0f)
, mSpecial(0.0f)
, mScore(0.0f)
, mOBBoxRenderable(0)
, mIsDebugEnabled(false)
, mStateTimeout(0.0f)
, catched(false)
{
	mType = type;
}

Item::~Item()
{
	finalizeEntity();
}

void Item::initializeEntity(Ogre::Entity* entity, Ogre::SceneNode* sceneNode, Ogre::SceneManager* sceneManager)
{
	// Always call base method before!
	EntityInterface::initializeEntity(entity, sceneNode, sceneManager);

	switch(mType)
	{
	case Item::ItemTypes::LiveBig :
		mLife = 100;
		mScore = 100;
		break;
	case Item::ItemTypes::LiveMedium :
		mLife = 50;
		mScore = 50;
		break;
	case Item::ItemTypes::LiveSmall :
		mLife = 25;
		mScore = 25;
		break;
	case Item::ItemTypes::PowerBig :
		mSpecial = 100;
		mScore = 100;
		break;
	case Item::ItemTypes::PowerMedium :
		mSpecial = 50;
		mScore = 50;
		break;
	case Item::ItemTypes::PowerSmall :
		mSpecial = 25;
		mScore = 25;
		break;
	case Item::ItemTypes::ScoreBig :
		mScore = 500;
		break;
	case Item::ItemTypes::ScoreSmall :
		mScore = 250;
		break;
	default:
		mLife = 0;
		mSpecial = 0;
		mScore = 0;
	}

	// Bounding Box
	mOBBoxRenderable = new OBBoxRenderable("OBBoxManualMaterial_Enemy");

	mOBBoxRenderable->setupVertices(mEntity->getBoundingBox());
	mOBBoxRenderable->setVisible(mIsDebugEnabled);
	mSceneNode->attachObject(mOBBoxRenderable);

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
	if(catched)
	{
		mStateTimeout += elapsedSeconds;
	}
}

void Item::caught()
{
	mEntity->setVisible(false);
	catched = true;
}

Item::ItemTypes Item::StringToType (Ogre::String string)
{
	const char* str = string.c_str();

	if(strcmp ( "LiveSmall", str ) == 0) return Item::ItemTypes::LiveSmall;

	return Item::ItemTypes::LiveSmall;
}


void Item::setDebugEnabled(bool isDebugEnabled)
{
	if(mIsDebugEnabled != isDebugEnabled)
	{
		mIsDebugEnabled = isDebugEnabled;

		if(mOBBoxRenderable)
			mOBBoxRenderable->setVisible(mIsDebugEnabled);
	}
}
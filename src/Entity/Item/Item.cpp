#include "..\..\..\include\Entity\Item\Item.h"

using namespace WyvernsAssault;

Item::Item(Ogre::String name, Item::ItemTypes type) 
: EntityInterface(name)
, PhysicsInterface()
, LogicInterface()
, mState(Item::ItemStates::Initial)
, mLife(0.0f)
, mSpecial(0.0f)
, mScore(0.0f)
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

void Item::caught()
{
	mEntity->setVisible(false);
}

void Item::updateLogic(lua_State *L, const float elapsedSeconds)
{
	///* the function name */
	lua_getglobal(L,"runItemLogic"/*ItemLogicList[mType].function*/);
	///* push arguments */
	lua_pushstring(L, getName().c_str());
	lua_pushnumber(L, mState);

	///* call the function with 1 argument, return 1 result */
	lua_call(L, 2, 1);

	///* get the result */
	Item::ItemStates newState = (Item::ItemStates)luaL_checkint(L, -1);
	lua_pop(L, 1);

	// state is the new state for the player
	// Do something if and only if the state has changed!
	if(newState != mState)
	{
		switch(newState)
		{
		case Initial:
			break;
		case Catch:
			break;
		case Caught:
			break;
		}

		mState = newState;
		mStateTimeout = 0.0f;
	}
	else
	{
		mStateTimeout += elapsedSeconds;
	}
}

Item::ItemTypes Item::StringToType (Ogre::String string)
{
	const char* str = string.c_str();

	if(strcmp ( "LiveSmall", str ) == 0) return Item::ItemTypes::LiveSmall;

	return Item::ItemTypes::LiveSmall;
}

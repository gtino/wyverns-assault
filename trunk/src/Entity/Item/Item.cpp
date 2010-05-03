#include "..\..\..\include\Entity\Item\Item.h"

using namespace WyvernsAssault;

//Only general item logic implemented
/*
static struct ItemLogic 
{
	ItemTypes type;
	const char* function;
} ItemLogicList[] = {
	{EnemyTypes::Naked, "runNakedLogic"},
	{EnemyTypes::Soldier, "runSoldierLogic"},
	{EnemyTypes::Wizard, "runWizardLogic"},
	{EnemyTypes::Knight, "runKnightLogic"},
	{EnemyTypes::Peasant, "runPeasantLogic"},
	{EnemyTypes::Woman,	"runWomanLogic"}
};
*/

ItemTypes Item::StringToType (Ogre::String string)
{
	const char* str = string.c_str();

	if(strcmp ( "LiveSmall", str ) == 0) return ItemTypes::LiveSmall;

	return ItemTypes::LiveSmall;
}

Item::Item(ItemTypes type) : mState(ItemStates::Modest)
{
	mType = type;
	mAnimationState = 0;
}

Item::~Item()
{
}

void Item::setBillboardSet(BillboardSet* balloonSet)
{
	mBalloonSet = balloonSet;
	mBalloonSet->setDefaultDimensions(15.0,15.0);
	mBalloonSet->setMaterialName("Balloons/Initial");

	Vector3 balloonPosition(0, 15, 0);
	mBalloon = mBalloonSet->createBillboard(balloonPosition);
}

void Item::updateEntity(const float elapsedSeconds)
{
	mAnimationState = mMesh->getAnimationState("Iddle");
	mAnimationState->setEnabled(true);
	mAnimationState->addTime(elapsedSeconds);
	return;
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
	ItemStates newState = (ItemStates)luaL_checkint(L, -1);
	lua_pop(L, 1);

	// state is the new state for the player
	// Do something if and only if the state has changed!
	if(newState != mState)
	{
		switch(newState)
		{
		case ItemStates::Modest:
			mBalloonSet->setVisible(false);
			mBalloonSet->setMaterialName("Balloons/Idle");
			break;
		case ItemStates::Attention:
			mBalloonSet->setVisible(true);
			mBalloonSet->setMaterialName("Balloons/Sleeping");
			break;
		case ItemStates::Catch:
			mBalloonSet->setVisible(true);
			mBalloonSet->setMaterialName("Balloons/What");
			break;
		default:
			mBalloonSet->setVisible(false);
			mBalloonSet->setMaterialName("Balloons/Initial");
			break;
		}

		mState = newState;
	}


}

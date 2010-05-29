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

	if(newState == ItemStates::Catch){
		//Remove Item
		mState = newState;
	}
}

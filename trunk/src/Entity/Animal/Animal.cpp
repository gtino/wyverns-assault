#include "..\..\..\include\Entity\Animal\Animal.h"

using namespace WyvernsAssault;

static struct AnimalLogic 
{
	AnimalTypes type;
	const char* function;
} AnimalLogicList[] = {
	{AnimalTypes::Chicken, "runChickenLogic"},
	{AnimalTypes::Cow, "runCowLogic"}
};

AnimalTypes Animal::StringToType (Ogre::String string)
{
	const char* str = string.c_str();

	if(strcmp ( "Chicken", str ) == 0) return AnimalTypes::Chicken;
	if(strcmp ( "Cow", str ) == 0) return AnimalTypes::Cow;

	return AnimalTypes::Chicken;
}

Animal::Animal(AnimalTypes type) :
mStateTimeout(0.0f),
mState(AnimalStates::Initial_animal),
mMaxLife(100.0f),
mLife(100.0f),
mDirection(Vector3::ZERO)
{
	mType = type;
	mAnimationState = 0;
}

Animal::~Animal()
{
	finalizeEntity();
}

void Animal::initializeEntity(Ogre::Entity* mesh, Ogre::SceneNode* sceneNode)
{
	mSceneNode = sceneNode;
	mMesh = mesh;
}

void Animal::finalizeEntity()
{	
}

void Animal::setTarget(SceneNode* target)
{
	//if(target)
	//	mSceneNode->setAutoTracking(true,target);

	mTarget = target;
}

void Animal::updateEntity(const float elapsedSeconds)
{
	mAnimationState = mMesh->getAnimationState("Iddle");
	mAnimationState->setEnabled(true);
	mAnimationState->addTime(elapsedSeconds);
	return;
}

void Animal::updateLogic(lua_State *L, const float elapsedSeconds)
{
	///* the function name */
	lua_getglobal(L,AnimalLogicList[mType].function);
	///* push arguments */
	lua_pushstring(L, getName().c_str());
	lua_pushnumber(L, mState);

	///* call the function with 1 argument, return 1 result */
	lua_call(L, 2, 1);

	///* get the result */
	AnimalStates newState = (AnimalStates)luaL_checkint(L, -1);
	lua_pop(L, 1);

	// state is the new state for the player
	// Do something if and only if the state has changed!
	if(newState != mState)
	{
		switch(newState)
		{
		case AnimalStates::Idle_animal:
			mSpeed = 0.0f;
			mDirection = Vector3::ZERO;
			break;
		case AnimalStates::Alert_animal:
			mSpeed = ANIMAL_SPEED_FAST;
			chase();
			break;
		default:
			mSpeed = 0.0f;
			mDirection = Vector3::ZERO;
			mTarget = 0;
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

bool Animal::isHurt()
{
	return (mLife / mMaxLife * 100.0f) < 15.0f;
}

void Animal::chase()
{
	if(mTarget)
	{
		Vector3 direction = mTarget->getPosition() - mSceneNode->getPosition();

		direction.normalise();

		move(direction);
	}
}

void Animal::move(Real x, Real y, Real z)
{
	// Direction vector
	Vector3 direction = Vector3(x, y, -z);

	move(direction);
}

void Animal::move(Vector3 to)
{
	mDirection = to;

	if (mDirection != Vector3::ZERO)
	{
		// Change autotrack axis for facing movement direction
		//mSceneNode->setAutoTracking(false, mAutoTrackingNode, mDirection);

		if(mAnimationState)
		{
			mAnimationState = mMesh->getAnimationState("Run");
			mAnimationState->setEnabled(true);
			mAnimationState->setLoop(true);
		}
	}
	else
	{
		if(mAnimationState)
		{
			mAnimationState = mMesh->getAnimationState("Iddle_01");
			mAnimationState->setEnabled(true);
			mAnimationState->setLoop(true);
		}
	}
}
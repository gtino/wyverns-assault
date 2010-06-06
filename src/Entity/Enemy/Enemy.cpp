#include "..\..\..\include\Entity\Enemy\Enemy.h"

using namespace WyvernsAssault;

static struct EnemyLogic 
{
	EnemyTypes type;
	const char* function;
} EnemyLogicList[] = {
	{EnemyTypes::Naked, "runNakedLogic"},
	{EnemyTypes::Soldier, "runSoldierLogic"},
	{EnemyTypes::Wizard, "runWizardLogic"},
	{EnemyTypes::Knight, "runKnightLogic"},
	{EnemyTypes::Peasant, "runPeasantLogic"},
	{EnemyTypes::Woman,	"runWomanLogic"},
	{EnemyTypes::Chicken, "runAnimalLogic"}
};

EnemyTypes Enemy::StringToType (Ogre::String string)
{
	const char* str = string.c_str();

	if(strcmp ( "Naked", str ) == 0) return EnemyTypes::Naked;
	if(strcmp ( "Soldier", str ) == 0) return EnemyTypes::Soldier;
	if(strcmp ( "Wizard", str ) == 0) return EnemyTypes::Wizard;
	if(strcmp ( "Knight", str ) == 0) return EnemyTypes::Knight;
	if(strcmp ( "Peasant", str ) == 0) return EnemyTypes::Peasant;
	if(strcmp ( "Woman", str ) == 0) return EnemyTypes::Woman;
	if(strcmp ( "Chicken", str) == 0) return EnemyTypes::Chicken;

	return EnemyTypes::Naked;
}

Enemy::Enemy(EnemyTypes type) :
mStateTimeout(0.0f),
mState(EnemyStates::Initial),
mMaxLife(100.0f),
mLife(100.0f),
mDirection(Vector3::ZERO),
mOBBoxRenderable(0),
mIsDebugEnabled(false)
{
	mType = type;
	mAnimationState = 0;
}

Enemy::~Enemy()
{
	finalizeEntity();
}

void Enemy::initializeEntity(Ogre::Entity* mesh, Ogre::SceneNode* sceneNode)
{
	mSceneNode = sceneNode;
	mMesh = mesh;
	mLastDirection = Vector3(0,0,0);

	// Bounding Box
	mOBBoxRenderable = new OBBoxRenderable("OBBoxManualMaterial_Enemy");

	mOBBoxRenderable->setupVertices(mMesh->getBoundingBox());
	mOBBoxRenderable->setVisible(mIsDebugEnabled);
	mSceneNode->attachObject(mOBBoxRenderable);
}

void Enemy::finalizeEntity()
{	
	if(mOBBoxRenderable)
	{
	delete mOBBoxRenderable;
	mOBBoxRenderable = NULL;
	}

	//mBalloonSet->removeBillboard(mBalloon);
	//mBalloonSet->detachFromParent();
}

void Enemy::setBillboardSet(BillboardSet* balloonSet)
{
	mBalloonSet = balloonSet;
	mBalloonSet->setDefaultDimensions(15.0,15.0);
	mBalloonSet->setMaterialName("Balloons/Initial");

	Vector3 balloonPosition(0, 15, 0);
	mBalloon = mBalloonSet->createBillboard(balloonPosition);
	//mBalloon->setColour(ColourValue::White);
}

void Enemy::setTarget(SceneNode* target)
{
	//if(target)
	//	mSceneNode->setAutoTracking(true,target);

	mTarget = target;
}

void Enemy::updateEntity(const float elapsedSeconds)
{
	if(mType == EnemyTypes::Chicken){
		Vector3 mDirection = this->getLastDirection();
		if (mDirection != Vector3::ZERO)
		{
			mAnimationState = mMesh->getAnimationState("Run");
			mAnimationState->setEnabled(true);
			mAnimationState->addTime(elapsedSeconds);
		}
		else
		{
			mAnimationState = mMesh->getAnimationState("Iddle");
			mAnimationState->setEnabled(true);
			mAnimationState->addTime(elapsedSeconds);
		}
	}
	return;
}

void Enemy::updateLogic(lua_State *L, const float elapsedSeconds)
{
	///* the function name */
	lua_getglobal(L,EnemyLogicList[mType].function);
	///* push arguments */
	lua_pushstring(L, getName().c_str());
	lua_pushnumber(L, mState);

	///* call the function with 1 argument, return 1 result */
	lua_call(L, 2, 1);

	///* get the result */
	EnemyStates newState = (EnemyStates)luaL_checkint(L, -1);
	lua_pop(L, 1);

	// state is the new state for the player
	// Do something if and only if the state has changed!
	if(newState != mState)
	{
		switch(newState)
		{
		case EnemyStates::Idle:
			mBalloonSet->setVisible(false);
			mBalloonSet->setMaterialName("Balloons/Idle");
			mSpeed = 0.0f;
			mDirection = Vector3::ZERO;
			break;
		case EnemyStates::Sleeping:
			mBalloonSet->setVisible(true);
			mBalloonSet->setMaterialName("Balloons/Sleeping");
			mSpeed = 0.0f;
			mDirection = Vector3::ZERO;
			break;
		case EnemyStates::What:
			mBalloonSet->setVisible(true);
			mBalloonSet->setMaterialName("Balloons/What");
			mSpeed = ENEMY_SPEED_SLOW;
			mTarget = 0;
			mDirection = Vector3::ZERO;
			break;
		case EnemyStates::Alert:
			mBalloonSet->setVisible(true);
			mBalloonSet->setMaterialName("Balloons/Alert");
			mSpeed = ENEMY_SPEED_FAST;
			chase();
			break;
		case EnemyStates::Rage:
			mBalloonSet->setVisible(true);
			mBalloonSet->setMaterialName("Balloons/Rage");
			mSpeed = 0.0f;
			break;
		case EnemyStates::Love:
			mBalloonSet->setVisible(true);
			mBalloonSet->setMaterialName("Balloons/Love");
			mSpeed = 0.0f;
			mDirection = Vector3::ZERO;
			break;
		case EnemyStates::Fear:
			mBalloonSet->setVisible(true);
			mBalloonSet->setMaterialName("Balloons/Fear");
			mSpeed = ENEMY_SPEED_FAST;
			mTarget = 0;
			break;
		case EnemyStates::Magic:
			mBalloonSet->setVisible(true);
			mBalloonSet->setMaterialName("Balloons/Magic");
			mSpeed = 0.0f;
			mDirection = Vector3::ZERO;
			break;
		case EnemyStates::Patrol:
			mBalloonSet->setVisible(true);
			mBalloonSet->setMaterialName("Balloons/Patrol");
			mSpeed = ENEMY_SPEED_MEDIUM;
			mDirection = Vector3::ZERO;
			mTarget = 0;
			break;
		default:
			mBalloonSet->setVisible(false);
			mBalloonSet->setMaterialName("Balloons/Initial");
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

	if((mStateTimeout > ENEMY_BILLBOARD_SHOW_TIME) && (mState != EnemyStates::Sleeping))
	{
		mBalloonSet->setVisible(false);
	}
}

bool Enemy::isHurt()
{
	return (mLife / mMaxLife * 100.0f) < 15.0f;
}

void Enemy::chase()
{
	/*if(mTarget)
	{
		Vector3 direction = mTarget->getPosition() - mSceneNode->getPosition();

		direction.normalise();

		move(direction);
	}*/
}

void Enemy::setDebugEnabled(bool isDebugEnabled)
{
	if(mIsDebugEnabled != isDebugEnabled)
	{
		mIsDebugEnabled = isDebugEnabled;

		if(mOBBoxRenderable)
			mOBBoxRenderable->setVisible(mIsDebugEnabled);
	}
}
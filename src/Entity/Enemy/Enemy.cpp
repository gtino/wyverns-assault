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
	{EnemyTypes::Woman,	"runWomanLogic"}
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

	return EnemyTypes::Naked;
}

Enemy::Enemy(EnemyTypes type) :
mStateTimeout(0.0f),
mState(EnemyStates::Initial),
mMaxLife(100.0f),
mLife(100.0f),
mDirection(Vector3::ZERO)
{
	mType = type;
	mAnimationState = 0;
}

Enemy::~Enemy()
{
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
	return;
	//autoTrackTarget();

	if(mDirection != Vector3::ZERO)
	{
		// Translate forward (always move forward into facing direction)
		mSceneNode->translate(mDirection * mSpeed * elapsedSeconds, Ogre::Node::TransformSpace::TS_LOCAL);
	}

	if(mAnimationState)
	{
		mAnimationState->addTime(elapsedSeconds);
	}
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
			mDirection = Vector3::ZERO;
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
	if(mTarget)
	{
		Vector3 direction = mTarget->getPosition() - mSceneNode->getPosition();

		direction.normalise();

		move(direction);
	}
}

void Enemy::move(Real x, Real y, Real z)
{
	// Direction vector
	Vector3 direction = Vector3(x, y, -z);

	move(direction);
}

void Enemy::move(Vector3 to)
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
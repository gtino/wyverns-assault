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
mState(EnemyStates::Idle)
{
	mType = type;
}

Enemy::~Enemy()
{
}

void Enemy::setBillboardSet(BillboardSet* balloonSet)
{
	mBalloonSet = balloonSet;
	mBalloonSet->setDefaultDimensions(15.0,15.0);
	mBalloonSet->setMaterialName("Balloons/Idle");

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
	//autoTrackTarget();
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
			break;
		case EnemyStates::Sleeping:
			mBalloonSet->setVisible(true);
			mBalloonSet->setMaterialName("Balloons/Sleeping");
			break;
		case EnemyStates::What:
			mBalloonSet->setVisible(true);
			mBalloonSet->setMaterialName("Balloons/What");
			break;
		case EnemyStates::Alert:
			mBalloonSet->setVisible(true);
			mBalloonSet->setMaterialName("Balloons/Alert");
			break;
		case EnemyStates::Rage:
			mBalloonSet->setVisible(true);
			mBalloonSet->setMaterialName("Balloons/Rage");
			break;
		case EnemyStates::Love:
			mBalloonSet->setVisible(true);
			mBalloonSet->setMaterialName("Balloons/Love");
			break;
		case EnemyStates::Fear:
			mBalloonSet->setVisible(true);
			mBalloonSet->setMaterialName("Balloons/Fear");
			break;
		case EnemyStates::Magic:
			mBalloonSet->setVisible(true);
			mBalloonSet->setMaterialName("Balloons/Magic");
			break;
		case EnemyStates::Patrol:
			mBalloonSet->setVisible(true);
			mBalloonSet->setMaterialName("Balloons/Patrol");
			break;
		default:
			mBalloonSet->setVisible(false);
			mBalloonSet->setMaterialName("Balloons/Idle");
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

void Enemy::autoTrackTarget()
{
	if(mTarget)
	{
		Vector3 direction = mTarget->_getDerivedPosition() - mSceneNode->getPosition();

		direction.normalise();

		mSceneNode->lookAt(mTarget->_getDerivedPosition(),Ogre::Node::TransformSpace::TS_WORLD);
	}
}
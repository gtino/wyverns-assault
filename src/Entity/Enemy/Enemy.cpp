#include "..\..\..\include\Entity\Enemy\Enemy.h"

using namespace WyvernsAssault;

static struct EnemyLogic 
{
	Enemy::EnemyTypes type;
	const char* function;
} EnemyLogicList[] = {
	{Enemy::EnemyTypes::KnightA, "runSoldierLogic"},
	{Enemy::EnemyTypes::KnightA2, "runSoldierLogic"},
	{Enemy::EnemyTypes::KnightA3, "runSoldierLogic"},
	{Enemy::EnemyTypes::KnightA4, "runSoldierLogic"},
	{Enemy::EnemyTypes::KnightA5, "runSoldierLogic"},
	{Enemy::EnemyTypes::KnightB, "runSoldierLogic"},
	{Enemy::EnemyTypes::KnightB2, "runSoldierLogic"},
	{Enemy::EnemyTypes::KnightB3, "runSoldierLogic"},
	{Enemy::EnemyTypes::KnightB4, "runSoldierLogic"},
	{Enemy::EnemyTypes::KnightB5, "runSoldierLogic"},
	{Enemy::EnemyTypes::KnightC, "runSoldierLogic"},
	{Enemy::EnemyTypes::KnightC2, "runSoldierLogic"},
	{Enemy::EnemyTypes::KnightC3, "runSoldierLogic"},
	{Enemy::EnemyTypes::KnightC4, "runSoldierLogic"},
	{Enemy::EnemyTypes::KnightC5, "runSoldierLogic"},

	{Enemy::EnemyTypes::Wizard, "runWizardLogic"},
	{Enemy::EnemyTypes::Wizard2, "runWizardLogic"},

	{Enemy::EnemyTypes::Naked, "runNakedLogic"},
	{Enemy::EnemyTypes::Soldier, "runSoldierLogic"},	
	{Enemy::EnemyTypes::Peasant, "runPeasantLogic"},
	{Enemy::EnemyTypes::Woman,	"runWomanLogic"},
	{Enemy::EnemyTypes::Chicken, "runAnimalLogic"}
};

Enemy::EnemyTypes Enemy::StringToType (Ogre::String string)
{
	const char* str = string.c_str();

	if(strcmp ( "KnightA", str ) == 0) return Enemy::EnemyTypes::KnightA;
	if(strcmp ( "KnightA2", str ) == 0) return Enemy::EnemyTypes::KnightA2;
	if(strcmp ( "KnightA3", str ) == 0) return Enemy::EnemyTypes::KnightA3;
	if(strcmp ( "KnightA4", str ) == 0) return Enemy::EnemyTypes::KnightA4;
	if(strcmp ( "KnightA5", str ) == 0) return Enemy::EnemyTypes::KnightA5;
	if(strcmp ( "KnightB", str ) == 0) return Enemy::EnemyTypes::KnightB;
	if(strcmp ( "KnightB2", str ) == 0) return Enemy::EnemyTypes::KnightB2;
	if(strcmp ( "KnightB3", str ) == 0) return Enemy::EnemyTypes::KnightB3;
	if(strcmp ( "KnightB4", str ) == 0) return Enemy::EnemyTypes::KnightB4;
	if(strcmp ( "KnightB5", str ) == 0) return Enemy::EnemyTypes::KnightB5;
	if(strcmp ( "KnightC", str ) == 0) return Enemy::EnemyTypes::KnightC;
	if(strcmp ( "KnightC2", str ) == 0) return Enemy::EnemyTypes::KnightC2;
	if(strcmp ( "KnightC3", str ) == 0) return Enemy::EnemyTypes::KnightC3;
	if(strcmp ( "KnightC4", str ) == 0) return Enemy::EnemyTypes::KnightC4;
	if(strcmp ( "KnightC5", str ) == 0) return Enemy::EnemyTypes::KnightC5;

	if(strcmp ( "Wizard", str ) == 0) return Enemy::EnemyTypes::Wizard;
	if(strcmp ( "Wizard2", str ) == 0) return Enemy::EnemyTypes::Wizard2;

	if(strcmp ( "Naked", str ) == 0) return Enemy::EnemyTypes::Naked;
	if(strcmp ( "Soldier", str ) == 0) return Enemy::EnemyTypes::Soldier;
	if(strcmp ( "Peasant", str ) == 0) return Enemy::EnemyTypes::Peasant;
	if(strcmp ( "Woman", str ) == 0) return Enemy::EnemyTypes::Woman;
	if(strcmp ( "Chicken", str) == 0) return Enemy::EnemyTypes::Chicken;

	return Enemy::EnemyTypes::Naked;
}

Enemy::Enemy(Ogre::String name, Enemy::EnemyTypes type)
: EntityInterface(name)
, PhysicsInterface()
, LogicInterface()
, mStateTimeout(0.0f)
, mState(Enemy::EnemyStates::Initial)
, mMaxLife(100.0f)
, mLife(100.0f)
, mOBBoxRenderable(0)
, mIsDebugEnabled(false)
, mBalloonSet(0)
, mBalloon(0)
, beginPatrolPoint(Vector3::ZERO)
, endPatrolPoint(Vector3::ZERO)
{
	mType = type;
}

Enemy::~Enemy()
{
	finalizeEntity();
}

void Enemy::initializeEntity(Ogre::Entity* entity, Ogre::SceneNode* sceneNode, Ogre::SceneManager* sceneManager)
{
	// Always call base method before!
	EntityInterface::initializeEntity(entity, sceneNode, sceneManager);

	// Ballon Set
	mBalloonSet = mSceneManager->createBillboardSet(mName + "_BillboardSet");
	mBalloonSet->setDefaultDimensions(15.0,15.0);
	mBalloonSet->setMaterialName("Balloons/Initial");
	mSceneNode->attachObject(mBalloonSet);

	Vector3 balloonPosition(0, 15, 0);
	mBalloon = mBalloonSet->createBillboard(balloonPosition);
	//mBalloon->setColour(ColourValue::White);

	// Bounding Box
	mOBBoxRenderable = new OBBoxRenderable("OBBoxManualMaterial_Enemy");

	mOBBoxRenderable->setupVertices(mEntity->getBoundingBox());
	mOBBoxRenderable->setVisible(mIsDebugEnabled);
	mSceneNode->attachObject(mOBBoxRenderable);

	// Animation system
	mAnimationSystem = new tecnofreak::ogre::AnimationSystem( entity );
	switch( mType )
	{
	case Enemy::EnemyTypes::Chicken:
		mAnimationSystem->loadAnimationTree( "data/animations/chicken.xml" );
		break;
	case Enemy::EnemyTypes::Wizard:
		mAnimationSystem->loadAnimationTree( "data/animations/wizard.xml" );
		break;
	case Enemy::EnemyTypes::Wizard2:
		mAnimationSystem->loadAnimationTree( "data/animations/wizard.xml" );
		break;
	case Enemy::EnemyTypes::Naked:
		mAnimationSystem->loadAnimationTree( "data/animations/naked.xml" );
		break;
	default:
		mAnimationSystem->loadAnimationTree( "data/animations/knight.xml" );		
	}
	mCurrentAnimation = mAnimationSystem->getParameter( "CurrentAnimation" );

	moving = false;
	attacking = false;

	// Random animation start time
	mAnimationSystem->update( rand() );
}

void Enemy::finalizeEntity()
{
	if(mBalloonSet)
	{
		mSceneManager->destroyBillboardSet(mBalloonSet);
		mBalloonSet = NULL;
	}

	// Always call base method before!
	EntityInterface::finalizeEntity();
}

void Enemy::updateEntity(const float elapsedSeconds)
{
	if( attacking )
	{
		mCurrentAnimation->setValue( ENEMY_ATTACK );
	}
	else if( moving )
	{
		mCurrentAnimation->setValue( ENEMY_RUN );
	}
	else
	{
		mCurrentAnimation->setValue( ENEMY_IDDLE );
	}

	mAnimationSystem->update( elapsedSeconds );
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
	Enemy::EnemyStates newState = (Enemy::EnemyStates)luaL_checkint(L, -1);
	lua_pop(L, 1);

	// state is the new state for the player
	// Do something if and only if the state has changed!
	if(newState != mState)
	{
		switch(newState)
		{
		case Enemy::EnemyStates::Idle:
			mBalloonSet->setVisible(false);
			mBalloonSet->setMaterialName("Balloons/Idle");
			mDirection = Vector3::ZERO;
			mSpeed = 0.0f;
			setMoving(false);
			setAttacking(false);
			break;
		case Enemy::EnemyStates::Sleeping:
			mBalloonSet->setVisible(true);
			mBalloonSet->setMaterialName("Balloons/Sleeping");
			mDirection = Vector3::ZERO;
			mSpeed = 0.0f;
			setMoving(false);
			setAttacking(false);
			break;
		case Enemy::EnemyStates::What:
			mBalloonSet->setVisible(true);
			mBalloonSet->setMaterialName("Balloons/What");
			setDirectionToTarget();
			mSpeed = BASIC_ENEMY_SLOW_VELOCITY;
			setMoving(true);
			setAttacking(false);
			break;
		case Enemy::EnemyStates::Alert:
			mBalloonSet->setVisible(true);
			mBalloonSet->setMaterialName("Balloons/Alert");
			setDirectionToTarget();
			mSpeed = BASIC_ENEMY_FAST_VELOCITY;
			setMoving(true);
			setAttacking(false);
			break;
		case Enemy::EnemyStates::Rage:
			mBalloonSet->setVisible(true);
			mBalloonSet->setMaterialName("Balloons/Rage");
			mDirection = Vector3::ZERO;
			mSpeed = 0.0f;
			setMoving(false);
			setAttacking(true);
			break;
		case Enemy::EnemyStates::Love:
			mBalloonSet->setVisible(true);
			mBalloonSet->setMaterialName("Balloons/Love");
			mDirection = Vector3::ZERO;
			mSpeed = 0.0f;
			setMoving(false);
			setAttacking(true);
			break;
		case Enemy::EnemyStates::Fear:
			mBalloonSet->setVisible(true);
			mBalloonSet->setMaterialName("Balloons/Fear");
			setDirectionOutTarget();
			mSpeed = BASIC_ENEMY_FAST_VELOCITY;
			setMoving(true);
			setAttacking(false);
			break;
		case Enemy::EnemyStates::Magic:
			mBalloonSet->setVisible(true);
			mBalloonSet->setMaterialName("Balloons/Magic");
			mSpeed = 0.0f;
			mDirection = Vector3::ZERO;
			setMoving(false);
			setAttacking(true);
			break;
		case Enemy::EnemyStates::Patrol:
			mBalloonSet->setVisible(true);
			mBalloonSet->setMaterialName("Balloons/Patrol");
			mSpeed = BASIC_ENEMY_SLOW_VELOCITY;
			//setAleatoryDirection();
			//setMoving(true);
			setMoving(false);
			setAttacking(false);
			break;
		case Enemy::EnemyStates::Dying:
  			mBalloonSet->setVisible(true);
			mBalloonSet->setMaterialName("Balloons/Dying");
			mDirection = Vector3::ZERO;
			mSpeed = 0.0f;
			setMoving(false);
			setAttacking(false);
			break;
		case Enemy::EnemyStates::Dead:
			mBalloonSet->setVisible(false);
			mBalloonSet->setMaterialName("Balloons/Initial");
			mSpeed = 0.0f;
			mDirection = Vector3::ZERO;
			setMoving(false);
			setAttacking(false);
			setMaterialName("Skin/Blue"); // DEBUG : Make him blue so it is obvious he is dead
			break;
		default:
			mBalloonSet->setVisible(false);
			mBalloonSet->setMaterialName("Balloons/Initial");
			mSpeed = 0.0f;
			mDirection = Vector3::ZERO;
			break;
		}

		mState = newState;
		mStateTimeout = 0.0f;
	}
	else
	{
		mStateTimeout += elapsedSeconds;
	}

	if((mStateTimeout > ENEMY_BILLBOARD_SHOW_TIME) && (mState != Enemy::EnemyStates::Sleeping))
	{
		mBalloonSet->setVisible(false);
	}
}

void Enemy::setTarget(SceneNode* target)
{
	//if(target)
	//	mSceneNode->setAutoTracking(true,target);

	mTarget = target;
}

void Enemy::hit(float damage)
{
	mLife -= damage;
}

bool Enemy::isHurt()
{
	return (mLife / mMaxLife * 100.0f) < 15.0f;
}

bool Enemy::isDying()
{
	return (mLife <= 0.0f);
}

void Enemy::setDirectionToTarget()
{
	if(mTarget)
	{
		Vector3 direction = mTarget->getPosition() - mSceneNode->getPosition();

		direction.normalise();

		mDirection = direction;

		/* TODO
		*/
		//Quaternion q1 = Quaternion(1,direction.x,direction.y,direction.z);
		//setOrientation(q1);

	}
}

void Enemy::setDirectionOutTarget()
{
	if(mTarget)
	{
		Vector3 direction = mSceneNode->getPosition() - mTarget->getPosition();

		direction.normalise();

		mDirection = direction;

		/* TODO
		*/
		//Quaternion q1 = Quaternion(1,direction.x,direction.y,direction.z);
		//setOrientation(q1);

	}
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

// Attack animation
void Enemy::setAttacking(bool attack)
{
	moving = false;
	attacking = attack;
}

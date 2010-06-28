#include "..\..\..\include\Entity\Enemy\Enemy.h"

using namespace WyvernsAssault;

static struct EnemyLogic 
{
	Enemy::EnemyTypes type;
	const char* function;
} EnemyLogicList[] = {
	{Enemy::EnemyTypes::Naked, "runNakedLogic"},
	{Enemy::EnemyTypes::Chicken, "runChickenLogic"},
	{Enemy::EnemyTypes::Knight, "runSoldierLogic"},
	{Enemy::EnemyTypes::Wizard, "runWizardLogic"},
	{Enemy::EnemyTypes::Soldier, "runSoldierLogic"},	
	{Enemy::EnemyTypes::Peasant, "runPeasantLogic"},
	{Enemy::EnemyTypes::Woman, "runWomanLogic"},
	{Enemy::EnemyTypes::Cow, "runAnimalLogic"}
};

Enemy::EnemyTypes Enemy::StringToType (Ogre::String string)
{
	const char* str = string.c_str();

	if(strcmp ( "Naked", str ) == 0) return Enemy::EnemyTypes::Naked;
	if(strcmp ( "Chicken", str) == 0) return Enemy::EnemyTypes::Chicken;
	if(strcmp ( "Knight", str ) == 0) return Enemy::EnemyTypes::Knight;
	if(strcmp ( "Wizard", str ) == 0) return Enemy::EnemyTypes::Wizard;
	// Still not used	
	if(strcmp ( "Soldier", str ) == 0) return Enemy::EnemyTypes::Soldier;
	if(strcmp ( "Peasant", str ) == 0) return Enemy::EnemyTypes::Peasant;
	if(strcmp ( "Woman", str ) == 0) return Enemy::EnemyTypes::Woman;	
	if(strcmp ( "Cow", str ) == 0) return Enemy::EnemyTypes::Cow;

	return Enemy::EnemyTypes::Naked;
}

Enemy::Enemy(Ogre::String name, Enemy::EnemyTypes type, Enemy::EnemyParameters params)
: EntityInterface(name)
, PhysicsInterface()
, LogicInterface()
, mStateTimeout(0.0f)
, mState(Enemy::EnemyStates::Initial)
, mOBBoxRenderable(0)
, mIsDebugEnabled(false)
, mBalloonSet(0)
, mBalloon(0)
, beginPatrolPoint(Vector3::ZERO)
, endPatrolPoint(Vector3::ZERO)
, mDieMesh(NULL)
, mDieAnimation(NULL)
{
	mType = type;
	mParameters = params;
	mMaxLife = params.life;
}

Enemy::~Enemy()
{
	finalizeEntity();
}

void Enemy::initializeEntity(Ogre::Entity* entity, Ogre::SceneNode* sceneNode, Ogre::SceneManager* sceneManager)
{
	// Always call base method before!
	EntityInterface::initializeEntity(entity, sceneNode, sceneManager);

	// Set physic body
	mPhysicEntity = mSceneManager->createEntity(entity->getName() + "_physicBody", mParameters.physicMesh);
	mPhysicEntity->setVisible(mIsDebugEnabled);
	mSceneNode->attachObject(mPhysicEntity);
	initializePhysics(mPhysicEntity);

	// Ballon Set
	mBalloonSet = mSceneManager->createBillboardSet(mName + "_BillboardSet");
	mBalloonSet->setDefaultDimensions(15.0,15.0);
	mBalloonSet->setMaterialName("Balloons/Initial");
	mSceneNode->attachObject(mBalloonSet);

	Vector3 balloonPosition(0, 15, 0);
	mBalloon = mBalloonSet->createBillboard(balloonPosition);

	// Bounding Box
	mOBBoxRenderable = new OBBoxRenderable("OBBoxManualMaterial_Enemy");
	mOBBoxRenderable->setupVertices(mPhysicEntity->getBoundingBox());
	mOBBoxRenderable->setVisible(mIsDebugEnabled);
	mSceneNode->attachObject(mOBBoxRenderable);

	// Animation system
	mAnimationSystem = new tecnofreak::ogre::AnimationSystem( entity );
	mAnimationSystem->loadAnimationTree( mParameters.animationTree );	
	mCurrentAnimation = mAnimationSystem->getParameter( "CurrentAnimation" );

	// States control variables
	moving = false;
	attacking = false;
	newAttack = false;
	attackHited = false;
	burning = false;
	flying = false;

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

	if(mDieAnimation)
	{
		mSceneManager->destroyAnimationState(mDieAnimation->getAnimationName());
		mDieAnimation = NULL;
	}

	// Always call base method before!
	EntityInterface::finalizeEntity();
}

void Enemy::updateEntity(const float elapsedSeconds)
{
	// Update if enemy is not dying
	if ( !isDying() )
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

		if( mCurrentAnimation->getFloatValue() == ENEMY_ATTACK )
		{
			// New Attack
			if( mEntity->getAnimationState("Attack")->getTimePosition() + elapsedSeconds > mEntity->getAnimationState("Attack")->getLength() )
				newAttack = true;
		}

		mAnimationSystem->update( elapsedSeconds );
	}
	// Update in case that has dying animation
	else
	{
		if( hasDieAnimation() )
		{
			mDieAnimation->addTime(elapsedSeconds);
			if( mDieAnimation->hasEnded() && hasDieMesh() )
				mDieMesh->setVisible(false);
		}
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
			setMoving(false);
			setAttacking(false);
			break;
		case Enemy::EnemyStates::Sleeping:
			mBalloonSet->setVisible(true);
			mBalloonSet->setMaterialName("Balloons/Sleeping");
			mDirection = Vector3::ZERO;
			setMoving(false);
			setAttacking(false);
			break;
		case Enemy::EnemyStates::What:
			mBalloonSet->setVisible(true);
			mBalloonSet->setMaterialName("Balloons/What");
			setDirectionToTarget();
			setMoving(true);
			setAttacking(false);
			break;
		case Enemy::EnemyStates::Alert:
			mBalloonSet->setVisible(true);
			mBalloonSet->setMaterialName("Balloons/Alert");
			setDirectionToTarget();
			setMoving(true);
			setAttacking(false);
			break;
		case Enemy::EnemyStates::Rage:
			mBalloonSet->setVisible(true);
			mBalloonSet->setMaterialName("Balloons/Rage");
			mDirection = Vector3::ZERO;
			setMoving(false);
			setAttacking(true);
			break;
		case Enemy::EnemyStates::Love:
			mBalloonSet->setVisible(true);
			mBalloonSet->setMaterialName("Balloons/Love");
			mDirection = Vector3::ZERO;
			setMoving(false);
			setAttacking(false);
			break;
		case Enemy::EnemyStates::Fear:
			mBalloonSet->setVisible(true);
			mBalloonSet->setMaterialName("Balloons/Fear");
			setDirectionOutTarget();
			setMoving(true);
			setAttacking(false);
			break;
		case Enemy::EnemyStates::Magic:
			mBalloonSet->setVisible(true);
			mBalloonSet->setMaterialName("Balloons/Magic");
			mDirection = Vector3::ZERO;
			setMoving(false);
			setAttacking(true);
			break;
		case Enemy::EnemyStates::Patrol:
			mBalloonSet->setVisible(true);
			mBalloonSet->setMaterialName("Balloons/Patrol");
			setMoving(false);
			setAttacking(false);
			break;
		case Enemy::EnemyStates::Dying:
  			mBalloonSet->setVisible(true);
			mBalloonSet->setMaterialName("Balloons/Dying");
			mDirection = Vector3::ZERO;
			setMoving(false);
			setAttacking(false);
			break;
		case Enemy::EnemyStates::Dead:
			mBalloonSet->setVisible(false);
			mBalloonSet->setMaterialName("Balloons/Initial");
			mDirection = Vector3::ZERO;
			setMoving(false);
			setAttacking(false);
			setMaterialName("Skin/Blue"); // DEBUG : Make him blue so it is obvious he is dead
			break;
		default:
			mBalloonSet->setVisible(false);
			mBalloonSet->setMaterialName("Balloons/Initial");
			mDirection = Vector3::ZERO;
			break;
		}

		mState = newState;
		mStateTimeout = 0.0f;
	}
	else
	{
		//
		// HACK! To update enemy direction every frame!
		//
		if(mState == Enemy::EnemyStates::Fear) 
		{
			setDirectionOutTarget();
		}
		else if( mState == Enemy::EnemyStates::Alert )
		{
			setDirectionToTarget();
		}

		mStateTimeout += elapsedSeconds;
	}

	if((mStateTimeout > ENEMY_BILLBOARD_SHOW_TIME) && (mState != Enemy::EnemyStates::Sleeping))
	{
		mBalloonSet->setVisible(false);
	}
}

void Enemy::setTarget(SceneNode* target)
{
	mTarget = target;
}

void Enemy::hit(float damage)
{
	mParameters.life -= damage;
}

bool Enemy::isHurt()
{
	return (mParameters.life / mMaxLife * 100.0f) < 15.0f;
}

bool Enemy::isDying()
{
	return (mParameters.life <= 0.0f);
}

void Enemy::setDirectionToTarget()
{
	if(mTarget)
	{	
		mDirection = mTarget->getPosition() - mSceneNode->getPosition();
		Vector3 src = mSceneNode->getOrientation() * Vector3::UNIT_Z;
		src.y = 0;
		mDirection.y = 0;
		mDirection.normalise();
		src.normalise();

		if ((1.0f + src.dotProduct(mDirection)) <= 0.0001f)            // Work around 180 degree quaternion rotation quirk                         
		{
			mSceneNode->yaw(Degree(180));
		}
		else
		{
			Quaternion rotation = src.getRotationTo(mDirection);
			mSceneNode->rotate(rotation);
		}
	}
}

void Enemy::setDirectionOutTarget()
{
	if( mType == EnemyTypes::Chicken )
	{
		setDirectionRandom();
	}
	else if(mTarget)
	{
		mDirection = mSceneNode->getPosition() - mTarget->getPosition();
		Vector3 src = mSceneNode->getOrientation() * Vector3::UNIT_Z;
		src.y = 0;
		mDirection.y = 0;
		mDirection.normalise();
		src.normalise();

		if ((1.0f + src.dotProduct(mDirection)) <= 0.0001f)            // Work around 180 degree quaternion rotation quirk                         
		{
			mSceneNode->yaw(Degree(180));
		}
		else
		{
			Quaternion rotation = src.getRotationTo(mDirection);
			mSceneNode->rotate(rotation);
		}
	}
}

void Enemy::setDirectionRandom()
{
	mDirection = Vector3((rand() / 100), (rand() / 100), (rand() / 100) );
	Vector3 src = mSceneNode->getOrientation() * Vector3::UNIT_Z;
	src.y = 0;
	mDirection.y = 0;
	mDirection.normalise();
	src.normalise();

	if ((1.0f + src.dotProduct(mDirection)) <= 0.0001f)            // Work around 180 degree quaternion rotation quirk                         
	{
		mSceneNode->yaw(Degree(180));
	}
	else
	{
		Quaternion rotation = src.getRotationTo(mDirection);
		mSceneNode->rotate(rotation);
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
	attacking = attack;
	newAttack = attack;
}

// Set die mesh and set invisible
void Enemy::setDieMesh(Ogre::Entity* entity)
{ 
	mDieMesh = entity;
	mDieMesh->setVisible(false);
}

// Set die animation from skeleton
void Enemy::setDieAnimation(Ogre::AnimationState* dieAnimation)
{
	mDieAnimation = dieAnimation;
	mDieAnimation->setEnabled(true);
	mDieAnimation->setWeight(1);
	mDieAnimation->setLoop(false);
}

// Die function, change visible meshes
void Enemy::dieSwitch()
{
	// Main mesh visibility to false
	setVisible(false);

	// Die mesh visible
	mDieMesh->setVisible(true);

	// Rotate random if has die animation
	if( hasDieAnimation() )
	{
		int angle = rand() % 60;
		mSceneNode->rotate(Quaternion(Degree(angle), Vector3(0,1,1)));
	}
}

// Die to camera animation
void Enemy::dieToCamera()
{
	Vector3 begin = getPosition() - getInitialPosition();
	Vector3 end = mSceneManager->getCamera("GameCamera")->getParentSceneNode()->getPosition() - getInitialPosition();
	Vector3 direction = mSceneManager->getCamera("GameCamera")->getDirection();

	if(mSceneManager->hasAnimation(mName + "_Die"))
		mSceneManager->destroyAnimation(mName + "_Die");
	Animation* anim = mSceneManager->createAnimation(mName + "_Die", 0.75);
    // Spline it for nice curves
    anim->setInterpolationMode(Animation::IM_SPLINE);
    // Create a track to animate the camera's node
	NodeAnimationTrack* track = anim->createNodeTrack(0, mSceneNode);
    // Setup keyframes
	TransformKeyFrame* key = track->createNodeKeyFrame(0);
	key->setTranslate(begin);
	key->setRotation(Quaternion(1, 0, 0, 0));
	key = track->createNodeKeyFrame(0.74);
	key->setTranslate(end + direction);
	key->setRotation(Quaternion((rand()%2), 1, 1, 1));
	key->setScale(Vector3(10, 10, 10));
	key = track->createNodeKeyFrame(0.75);
	key->setTranslate(end + direction);
	key->setScale(Vector3(0.1, 0.1, 0.1));
	// Create a new animation state to track this
	mDieAnimation = mSceneManager->createAnimationState(mName + "_Die");
    mDieAnimation->setEnabled(true);
	mDieAnimation->setWeight(1);
	mDieAnimation->setLoop(false);

	flying = true;
}

// Stop all enemy actions
void Enemy::stop()
{
	mTarget = NULL;
	mState = Enemy::EnemyStates::Dead;
}

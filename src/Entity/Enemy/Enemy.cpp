#include "..\..\..\include\Entity\Enemy\Enemy.h"

using namespace WyvernsAssault;

static struct EnemyLogic 
{
	Enemy::EnemyTypes type;
	const char* function;
} EnemyLogicList[] = {
	{Enemy::EnemyTypes::Naked,			"runNakedLogic"},
	{Enemy::EnemyTypes::Chicken,		"runChickenLogic"},
	{Enemy::EnemyTypes::Cow,			"runCowLogic"},
	{Enemy::EnemyTypes::Woman,			"runWomanLogic"},
	{Enemy::EnemyTypes::Peasant,		"runPeasantLogic"},
	{Enemy::EnemyTypes::Knight,			"runSoldierLogic"},
	{Enemy::EnemyTypes::Wizard,			"runWizardLogic"},	
	{Enemy::EnemyTypes::Archer,			"runArcherLogic"}, 		
	{Enemy::EnemyTypes::BatteringRam,	"runBatteringRamLogic"},
	{Enemy::EnemyTypes::Boss,			"runBossTarget"},
};

Enemy::EnemyTypes Enemy::StringToType (Ogre::String string)
{
	const char* str = string.c_str();

	if(strcmp ( "Naked", str ) == 0)		return Enemy::EnemyTypes::Naked;
	if(strcmp ( "Chicken", str) == 0)		return Enemy::EnemyTypes::Chicken;
	if(strcmp ( "Cow", str ) == 0)			return Enemy::EnemyTypes::Cow;
	if(strcmp ( "Woman", str ) == 0)		return Enemy::EnemyTypes::Woman;	
	if(strcmp ( "Peasant", str ) == 0)		return Enemy::EnemyTypes::Peasant;
	if(strcmp ( "Knight", str ) == 0)		return Enemy::EnemyTypes::Knight;
	if(strcmp ( "Wizard", str ) == 0)		return Enemy::EnemyTypes::Wizard;
	if(strcmp ( "Archer", str ) == 0)		return Enemy::EnemyTypes::Archer;
	if(strcmp ( "BatteringRam", str ) == 0)	return Enemy::EnemyTypes::BatteringRam;
	if(strcmp ( "Boss", str ) == 0)			return Enemy::EnemyTypes::Boss;

	return Enemy::EnemyTypes::Cow;
}

Enemy::Enemy(Ogre::String name, Enemy::EnemyTypes type, Enemy::EnemyParameters params)
: EntityInterface(name)
, PhysicsInterface()
, LogicInterface()
, mStateTimeout(0.0f)
, mState(Enemy::EnemyStates::Initial)
, mLastState(Enemy::EnemyStates::Initial)
, mOBBoxRenderable(0)
, mIsDebugEnabled(false)
, mBalloonSet(0)
, mBalloon(0)
, mDieMesh(NULL)
, mDieAnimation(NULL)
, mAttackTimeout(0)
, mHasItem(false)
, mLastEnemyCollision(name)
, mPhysicsList(0)
, mPhysicsListIndex(0)
, mBossRandomAttack(2)
, mAnimationTime(0)
, mBossControlTimeHit(false)
, mBossControlTimeSpecial(false)
{
	mType = type;
	mParameters = params;
	mMaxLife = params.life;

	// Attack cooldown
	mParameters.attackCooldown = 0.5f;

	// Physisc Size
	if( mType == EnemyTypes::BatteringRam )
		mParameters.physicSize = Vector3(40, 30, 40);
	else if( mType == EnemyTypes::Boss )
		mParameters.physicSize = Vector3(100, 30, 30);
	else
		mParameters.physicSize = Vector3(PHYSIC_SIZE, PHYSIC_SIZE, PHYSIC_SIZE);

	// Item drop designation
	if( mType == EnemyTypes::Woman )
		mHasItem = true;
	else if( (rand() % 10) == 0)
		mHasItem = true;

	mOBBoxRenderable = new OBBoxRenderable("OBBoxManualMaterial_Enemy");
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
	initializePhysics(entity->getName(), mParameters.physicSize, "OBBoxManualMaterial_Enemy");
	
	sceneNode->attachObject(getGeometry(PhysicBoxType::body)->getMovableObject());

	// Ballon Set
	mBalloonSet = mSceneManager->createBillboardSet(mName + "_BillboardSet");
	mBalloonSet->setDefaultDimensions(15.0, 15.0);
	mBalloonSet->setMaterialName("Balloons/Initial");
	mSceneNode->attachObject(mBalloonSet);
	mBalloonSet->setVisible(false);

	Vector3 balloonPosition(0, 15, 0);
	mBalloon = mBalloonSet->createBillboard(balloonPosition);

	// Animation system
	mAnimationSystem = new tecnofreak::ogre::AnimationSystem( entity );
	mAnimationSystem->loadAnimationTree( mParameters.animationTree );	
	mCurrentAnimation = mAnimationSystem->getParameter( "CurrentAnimation" );

	// States control variables
	moving = false;
	attacking = false;
	special = false;
	newAttack = false;
	attackHited = false;
	burning = false;
	flying = false;

	// Random animation start time
	mAnimationSystem->update( rand() );
}

void Enemy::initializeBossEntity(Ogre::Entity* entity, Ogre::SceneNode* sceneNode, Ogre::SceneManager* sceneManager)
{
	// Always call base method before!
	EntityInterface::initializeEntity(entity, sceneNode, sceneManager);

	// Set physics for every leg
	initializeBossPhysics(entity->getName(), mParameters.physicSize, "OBBoxManualMaterial_Enemy");
	
	// Attach every physics box to correct bone, and save position in list with correct order
	TagPoint* tag = entity->attachObjectToBone( "bone4", getBossGeometry(0)->getMovableObject());
	mPhysicsList.push_back( mSceneNode->createChildSceneNode(tag->_getDerivedPosition()));
	tag = entity->attachObjectToBone( "bone29", getBossGeometry(1)->getMovableObject());
	mPhysicsList.push_back( mSceneNode->createChildSceneNode(tag->_getDerivedPosition()));
	tag = entity->attachObjectToBone( "bone34", getBossGeometry(2)->getMovableObject());
	mPhysicsList.push_back( mSceneNode->createChildSceneNode(tag->_getDerivedPosition()));
	tag = entity->attachObjectToBone( "bone39", getBossGeometry(3)->getMovableObject());
	mPhysicsList.push_back( mSceneNode->createChildSceneNode(tag->_getDerivedPosition()));
	tag = entity->attachObjectToBone( "bone24", getBossGeometry(4)->getMovableObject());
	mPhysicsList.push_back( mSceneNode->createChildSceneNode(tag->_getDerivedPosition()));
	tag = entity->attachObjectToBone( "bone19", getBossGeometry(5)->getMovableObject());
	mPhysicsList.push_back( mSceneNode->createChildSceneNode(tag->_getDerivedPosition()));
	tag = entity->attachObjectToBone( "bone14", getBossGeometry(6)->getMovableObject());
	mPhysicsList.push_back( mSceneNode->createChildSceneNode(tag->_getDerivedPosition()));
	tag = entity->attachObjectToBone( "bone9", getBossGeometry(7)->getMovableObject());
	mPhysicsList.push_back( mSceneNode->createChildSceneNode(tag->_getDerivedPosition()));

	// Animation system
	mAnimationSystem = new tecnofreak::ogre::AnimationSystem( entity );
	mAnimationSystem->loadAnimationTree( mParameters.animationTree );
	mCurrentAnimation = mAnimationSystem->getParameter( "CurrentAnimation" );

	// Set loop animation to false
	entity->getAnimationState("Attack_1")->setLoop(false);
	entity->getAnimationState("Attack_2")->setLoop(false);
	entity->getAnimationState("Attack_3")->setLoop(false);
	entity->getAnimationState("Attack_4")->setLoop(false);

	// States control variables
	moving = false;
	attacking = false;
	special = false;
	newAttack = false;

	bossHitAnimation = 0;
	hitControl = false;

	// Random animation start time
	mAnimationSystem->update( rand() );

	//
	// Boss EXTRAS
	//

	// Princess in jail
	Entity* extra = mSceneManager->createEntity("bossPrincess", "bossPrincess.mesh");
	extra->getAnimationState("Iddle")->setEnabled(1);
	extra->getAnimationState("Iddle")->setWeight(1);
	entity->attachObjectToBone("null5", extra, Quaternion::IDENTITY, Vector3(0, -34, -5));
	
	// Ballon Set for princess
	mBalloonSet = mSceneManager->createBillboardSet(mName + "_BillboardSet");
	mBalloonSet->setDefaultDimensions(15.0, 15.0);
	mBalloonSet->setMaterialName("Balloons/Help");
	entity->attachObjectToBone("null5", mBalloonSet);

	Vector3 balloonPosition(10, 20, 15);
	mBalloon = mBalloonSet->createBillboard(balloonPosition);

	// Pilot
	extra = mSceneManager->createEntity("bossPilot", "bossPilot.mesh");
	extra->setMaterialName("Skin/Blue");
	extra->getAnimationState("Iddle")->setEnabled(1);
	extra->getAnimationState("Iddle")->setWeight(1);
	entity->attachObjectToBone("null5", extra, Quaternion::IDENTITY, Vector3(0, -34, -5));
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

	mPhysicsList.clear();

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
		else if( mType == EnemyTypes::Woman && mState == EnemyStates::Love )
		{
			mCurrentAnimation->setValue( WOMAN_GIVE );
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
		}
	}
}

void Enemy::updateBossEntity(const float elapsedSeconds)
{
	// Update boss is not dying
	if ( !isDying() )
	{

		//Hit animation control
		if(((mParameters.life < (mMaxLife*3)/4 && bossHitAnimation == 0) || (mParameters.life < mMaxLife/2 && bossHitAnimation == 1) || (mParameters.life < mMaxLife/4 && bossHitAnimation == 2)) || hitControl){
    		mCurrentAnimation->setValue( BOSS_HIT );
 			hitControl = true;
		}

		if(!hitControl){
			if( mState == EnemyStates::Idle )
			{
				mCurrentAnimation->setValue( BOSS_IDDLE ); 
			}
			else if( mState == EnemyStates::Rage )
			{
				mCurrentAnimation->setValue( BOSS_ATTACK4 );
				mBossRandomAttack = 0;
			}	
			else if( mState == EnemyStates::Special )
			{
				if(mLastState != mState)
				{
					//Random value 1-2-3
					mBossRandomAttack = int(Ogre::Math::RangeRandom(1,4));
				}
				mCurrentAnimation->setValue( mBossRandomAttack );
			}
		}

		mAnimationSystem->update( elapsedSeconds );

		// Princess in jail
		mSceneManager->getEntity("bossPrincess")->getAnimationState("Iddle")->addTime(elapsedSeconds);

		// Pilot
		mSceneManager->getEntity("bossPilot")->getAnimationState("Iddle")->addTime(elapsedSeconds);
	}
	// Update in case that has dying animation
	else
	{
		if( hasDieAnimation() )
		{
			mDieAnimation->addTime(elapsedSeconds);
		}
	}
	
	mLastState = mState;
}

Real Enemy::getAttackTime()
{
	if( mBossRandomAttack == 0 )
		return 1.0;
	else if( mBossRandomAttack == 1 )
		return 1.0;
	else if( mBossRandomAttack == 2 )
		return 2.0;
	else if( mBossRandomAttack == 3 )
		return 2.0;
	else
		return 1.0;
}

void Enemy::setBossTarget(lua_State *L){
	///* the function name */
	lua_getglobal(L,EnemyLogicList[mType].function);
	///* push arguments */
	lua_pushstring(L, getName().c_str());
	///* call the function with 1 argument, return 1 result */
	lua_call(L, 1, 0);
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
		case Enemy::EnemyStates::Fire:
			mBalloonSet->setVisible(true);
			mBalloonSet->setMaterialName("Balloons/Rage");
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
		mStateTimeout += elapsedSeconds;

		if(mState == Enemy::EnemyStates::Fear) 
		{
			setDirectionOutTarget();
		}
		else if( mState == Enemy::EnemyStates::Alert )
		{
			setDirectionToTarget();
		}		
	}

	if((mStateTimeout > ENEMY_BILLBOARD_SHOW_TIME) && (mState != Enemy::EnemyStates::Sleeping))
	{
		mBalloonSet->setVisible(false);
	}
}

void Enemy::updateBossLogic(lua_State *L, const float elapsedSeconds)
{
	if( !isDying() )
	{
		// Animation time control
		mAnimationTime = mAnimationTime + elapsedSeconds;
		if(mAnimationTime > 15)
			mAnimationTime = 0;

		// Set mTarget from lua
		setBossTarget(L);

		// 15 seconds block
		if(mAnimationTime > 4 && mAnimationTime < 5 && !mBossControlTimeHit)
		{
			mState = EnemyStates::Rage;
			mBossControlTimeHit = true;
		}
		else if(mAnimationTime > 10 && mAnimationTime < 11 && mBossControlTimeHit)
		{
			mState = EnemyStates::Special;
			mBossControlTimeHit = false;
		}

		if( mCurrentAnimation->getFloatValue() ==  BOSS_ATTACK1 )
		{		
			if( mEntity->getAnimationState("Attack_1")->getTimePosition() + elapsedSeconds > mEntity->getAnimationState("Attack_1")->getLength() )
				mState = EnemyStates::Idle;
		}
		else if( mCurrentAnimation->getFloatValue() ==  BOSS_ATTACK2 )
		{
			if( mEntity->getAnimationState("Attack_2")->getTimePosition() + elapsedSeconds > mEntity->getAnimationState("Attack_2")->getLength() )
				mState = EnemyStates::Idle;
		}
		else if( mCurrentAnimation->getFloatValue() ==  BOSS_ATTACK3 )
		{
			if( mEntity->getAnimationState("Attack_3")->getTimePosition() + elapsedSeconds > mEntity->getAnimationState("Attack_3")->getLength() )
				mState = EnemyStates::Idle;
		}
		else if( mCurrentAnimation->getFloatValue() ==  BOSS_ATTACK4 )
		{
			if( mEntity->getAnimationState("Attack_4")->getTimePosition() + elapsedSeconds > mEntity->getAnimationState("Attack_4")->getLength() )
				mState = EnemyStates::Idle;
		}else if( mCurrentAnimation->getFloatValue() ==  BOSS_HIT )
		{
			if( mEntity->getAnimationState("Hit")->getTimePosition() + elapsedSeconds > mEntity->getAnimationState("Hit")->getLength() ){
				mState = EnemyStates::Idle;
				hitControl = false;
				bossHitAnimation++;
			}
		}		

		// state is the new state for the player
		// Do something if and only if the state has changed!
		if(mState != mLastState)
		{
			switch(mState)
			{
			case Enemy::EnemyStates::Idle:			
				//setMoving(false);
				setAttacking(false);
				setSpecial(false);
				break;
			case Enemy::EnemyStates::Rage:
				//setMoving(false);
				setSpecial(false);
				attackHited = false;
				newAttack = true;
				break;
			case Enemy::EnemyStates::Special:
				//setMoving(false);
				setSpecial(true);
				attackHited = false;
				newAttack = true;
				break;
			default:			
				mDirection = Vector3::ZERO;
				break;
			}

			mStateTimeout = 0.0;
			mBalloonSet->setVisible(true);
		}
		else
		{
			mStateTimeout = mStateTimeout + elapsedSeconds;

			if( (mState == Enemy::EnemyStates::Rage || mState == Enemy::EnemyStates::Special ) && mStateTimeout > 1.0 && !attackHited )
			{
				attacking = true;			
			}
		}	
		
		//Rotate boss
		if(mTarget && mState == EnemyStates::Idle)
		{	
			Ogre::Vector3 directionPlayerBoss = mTarget->getPosition() - mSceneNode->getPosition();
			Vector3 bossOrientation = mSceneNode->getOrientation() * Vector3(1,0,1);
			bossOrientation.y = 0;
			directionPlayerBoss.y = 0;
			directionPlayerBoss.normalise();
			bossOrientation.normalise();

			//if(angleBetween > Ogre::Radian(180)){
			//	mSceneNode->rotate(Ogre::Vector3::UNIT_Y,Degree(-0.5));
			//}else if(angleBetween < Ogre::Radian(180)){
			//	mSceneNode->rotate(Ogre::Vector3::UNIT_Y,Degree(0.5));
			//}

			//if(angleBetween > Ogre::Radian(0.1) || angleBetween < Ogre::Radian(-0.1)){
			//	mSceneNode->rotate(Ogre::Vector3::UNIT_Y,Degree(0.5));
			//}
		
		}
		
		if( mStateTimeout > 2.0 && mState != Enemy::EnemyStates::Sleeping )
		{
			mBalloonSet->setVisible(false);
		}	
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
	if(mTarget)
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
	mDirection = Vector3((rand() / 100), 0, (rand() / 100) );
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

bool Enemy::isRanged()
{
	if( mType == EnemyTypes::Wizard || mType == EnemyTypes::Archer )
		return true;
	return false;
}

bool Enemy::isMashable()
{
	if( mType == EnemyTypes::Chicken || mType == EnemyTypes::Cow )
		return true;
	return false;
}

void Enemy::setDebugEnabled(bool isDebugEnabled)
{
	if(mIsDebugEnabled != isDebugEnabled)
	{
		mIsDebugEnabled = isDebugEnabled;
		if( mType == EnemyTypes::Boss )
		{
			for( int i = 0; i < 8; i++)
			{
				getBossGeometry(i)->getMovableObject()->setVisible(mIsDebugEnabled);
			}
		}
		else
			getGeometry(PhysicBoxType::body)->getMovableObject()->setVisible(mIsDebugEnabled);
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
	if(mBalloonSet)
		mBalloonSet->setVisible(false);

	// Rotate random if has die animation
	if( hasDieAnimation() )
	{
		if( mType != EnemyTypes::Boss )
		{
			int angle = rand() * 200;
			mSceneNode->rotate(Quaternion(Degree(angle), Vector3(0,1,0)));
			mSceneNode->translate(0, getHeight()/2, 0);
		}
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

	if(mBalloonSet)
		mBalloonSet->setVisible(false);

	flying = true;
}

// Visibility function
void Enemy::setVisible(bool visibility)
{
	mEntity->setVisible(visibility);
	//mBalloonSet->setVisible(visibility);
}

// Stop all enemy actions
void Enemy::stop()
{
	mTarget = NULL;
	mState = Enemy::EnemyStates::Dead;
	mParameters.life = 0.0;
}

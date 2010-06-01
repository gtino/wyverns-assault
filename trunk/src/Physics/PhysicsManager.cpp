#include "..\..\include\Physics\PhysicsManager.h"



using namespace WyvernsAssault;

const Ogre::Real STEP_RATE = 0.01;


// BEGIN SINGLETON
template<> PhysicsManager* Ogre::Singleton<PhysicsManager>::ms_Singleton = 0;
PhysicsManager* PhysicsManager::getSingletonPtr(void)
{
    return ms_Singleton;
}
PhysicsManager& PhysicsManager::getSingleton(void)
{  
    assert( ms_Singleton );  return ( *ms_Singleton );  
}
// END SINGLETON

PhysicsManager::PhysicsManager(SceneManager* sceneManager)
{
	mSceneManager = sceneManager;
}

PhysicsManager::~PhysicsManager()
{
	// Stop listening for collisions
	if (mWorld->getCollisionListener() == this) 
        mWorld->setCollisionListener(0);

	// Delete all the joints
	for (std::vector<OgreOde::Joint*>::iterator i = _joints.begin();i != _joints.end();++i)
	{
		delete (*i);
	}

	// Run through the list of bodies we're monitoring
	for (std::vector<OgreOde::Body*>::iterator i = _bodies.begin();i != _bodies.end();++i)
	{
		// Delete the body
		delete (*i);
	}

	// Delete all the collision geometries
	for (std::vector<OgreOde::Geometry*>::iterator i = _geoms.begin();i != _geoms.end();++i)
	{
		delete (*i);
	}
}

bool PhysicsManager::initialize()
{
	// Create Ode world
	mWorld = new OgreOde::World(mSceneManager);
	mWorld->setGravity(Vector3(0,-9.80665,0));
	mWorld->setCFM(10e-5);
	mWorld->setERP(0.8);
	mWorld->setAutoSleep(false);
    mWorld->setAutoSleepAverageSamplesCount(10);
	mWorld->setContactCorrectionVelocity(1.0);
	mWorld->setCollisionListener(this);

	// Set collision space
	mSpace = mWorld->getDefaultSpace();

	// Create stepper
	const Ogre::Real time_scale = Ogre::Real(1.0); 
    const Ogre::Real max_frame_time = Ogre::Real(1.0 / 4);
    const Ogre::Real frame_rate = Ogre::Real(1.0 / 60);

	OgreOde::StepHandler::StepModeType stepModeType = OgreOde::StepHandler::QuickStep;
	mStepper = new OgreOde::ForwardFixedStepHandler(mWorld, stepModeType, STEP_RATE, max_frame_time, time_scale);

	// State control variables
	mPlayerSpecialState = false;
	mPlayerAttackLast = 0;

	return true;
}

void PhysicsManager::finalize()
{
	mPlayerMap.clear();
	mEnemyMap.clear();
}

void PhysicsManager::synchronizeWorld(Real time)
{
	if(mStepper->step(time))
		mWorld->synchronise();
}

void PhysicsManager::showDebugObjects()
{
	mWorld->setShowDebugGeometries(!mWorld->getShowDebugGeometries());
}

void PhysicsManager::createGround(Ogre::String mesh,Ogre::String name,Ogre::Vector3 position,Ogre::Vector3 scale)
{
	SceneNode* node_ground = mSceneManager->getRootSceneNode()->createChildSceneNode(name,position);
	Entity* ent_ground = mSceneManager->createEntity(name,mesh);
	node_ground->attachObject(ent_ground);
	OgreOde::EntityInformer ei_ground(ent_ground,node_ground->_getFullTransform());
	geom_ground = ei_ground.createStaticTriangleMesh(mWorld,mSpace);
	ent_ground->setUserAny(Ogre::Any(geom_ground));
	node_ground->setVisible(false);
	node_ground->setScale(scale);
	_geoms.push_back(geom_ground);
}

void PhysicsManager::addPlayer(PlayerPtr player)
{
	Ogre::String id = player->getName();

	AxisAlignedBox aab = player->getSceneNode()->getAttachedObject(id)->getBoundingBox(); 
	Ogre::Vector3 min = aab.getMinimum()*player->getScale();
	Ogre::Vector3 max = aab.getMaximum()*player->getScale();
	Ogre::Vector3 size(fabs(max.x-min.x),fabs(max.y-min.y),fabs(max.z-min.z));
	float radius = (size.x>size.z)?size.z/2.0f:size.x/2.0f;

	//Geometry
	OgreOde::Body* body = new OgreOde::Body(mWorld,id+"_Body"); 
	body->setMass(OgreOde::CapsuleMass(70,radius/2,Vector3::UNIT_Y,radius/2)); 
	body->setAffectedByGravity(true);
	OgreOde::TransformGeometry* transform = new OgreOde::TransformGeometry(mWorld,mSpace); 
	//OgreOde::CapsuleGeometry* geometry = new OgreOde::CapsuleGeometry(radius/2,size.y/5,mWorld); 
	OgreOde::CapsuleGeometry* geometry = new OgreOde::CapsuleGeometry(radius/4,size.y/3,mWorld);
	geometry->setOrientation(Quaternion(Degree(90),Vector3::UNIT_X));
	transform->setBody(body); 
	transform->setEncapsulatedGeometry(geometry);
	player->getSceneNode()->attachObject(body);
	player->setBody(body);

	//RayInfo
	PhysicsRayInfo rayInfo;
	rayInfo.geometry = new OgreOde::RayGeometry(Ogre::Real(30), mWorld, mSpace);
	rayInfo.radius = radius;
	rayInfo.updated = false;
	rayInfo.lastContact = Vector3(0,0,0);

	player->setRayInfo(rayInfo);

	//
	// Store the player into an internal map
	//
	mPlayerMap[player->getGeometryId()] = player;
	mPlayerGeomMap[body->getGeometry(0)->getID()] = player;

	_bodies.push_back(body);
	_geoms.push_back(geometry);
	
}

void PhysicsManager::update(const float elapsedSeconds){
	synchronizeWorld(elapsedSeconds);
	
	//update players ray
	for(OdePlayerMapIterator it_player = mPlayerMap.begin(); it_player != mPlayerMap.end(); ++it_player)
	{
		PlayerPtr player = it_player->second;
		updateRay(player);
	}

	//update enemys ray apply force to keep up
	for(OdeEnemyMapIterator it_enemy = mEnemyMap.begin(); it_enemy != mEnemyMap.end(); ++it_enemy)
	{
		EnemyPtr enemy = it_enemy->second;
		updateRay(enemy);

		OgreOde::Body* enemy_body = enemy->getBody();
		Quaternion q = enemy_body->getOrientation();         
		Vector3 x = q.xAxis();
		Vector3 y = q.yAxis();
		Vector3 z = q.zAxis();
		enemy_body->wake();
		enemy_body->setOrientation(Quaternion(x,Vector3::UNIT_Y,z));
		enemy_body->setAngularVelocity(Vector3(0,0,0)); 
		enemy_body->setLinearVelocity(Vector3(0,0,0));

		// update ray position
		if(enemy->getRayInfo()->updated)
		{
			enemy_body->setPosition(Vector3(enemy_body->getPosition().x,enemy->getRayInfo()->lastContact.y+15,enemy->getPosition().z));
			enemy->getRayInfo()->updated = false;
		}
	}

	//Enemy - Player collide
	OgreOde::Body* player_body;
	OgreOde::Geometry* player_geom;
	PlayerPtr player;
	for(OdePlayerMapIterator it_player = mPlayerMap.begin(); it_player != mPlayerMap.end(); ++it_player)
	{
		player = it_player->second;
		player_body = player->getBody();
		player_geom = player_body->getGeometry(0);
	}
	for(OdeEnemyMapIterator it_enemy = mEnemyMap.begin(); it_enemy != mEnemyMap.end(); ++it_enemy)
	{
		EnemyPtr enemy = it_enemy->second;
		OgreOde::Body* enemy_body = enemy->getBody();
		OgreOde::Geometry* enemy_geom = enemy_body->getGeometry(0);
		player_geom->collide(enemy_geom,this);
	}

	// Update animals:  move animals - Animals movement IA
	for(OdeEnemyMapIterator it_animal = mAnimalGeomMap.begin(); it_animal != mAnimalGeomMap.end(); ++it_animal){
		EnemyPtr animal = it_animal->second;
		//Verify type of animal and state
		if(animal->getEnemyType() == EnemyTypes::Chicken){
			if(animal->getEnemyState() == EnemyStates::Fear){
				
				/*
				 * Movement based on player and enemy position
				 
				Vector3 player_position = player->getPosition();
				Vector3 animal_position = animal->getPosition();

				Vector3 direction = animal_position - player_position;
				moveAnimal(animal,direction);
				*/
				
				/*
				 * Movement based on last direction of player
				 */
				Vector3 playerDirection = player->getLastDirection();
				if(playerDirection == Vector3::ZERO){
					moveAnimal(animal,animal->getLastDirection());
				}else{
					moveAnimal(animal,playerDirection);
				}

			}else{
				moveAnimal(animal,Vector3(0,0,0));
			}
		}

	}

	//
	// Non physics collisions
	// 
	checkForCollisions();

	return;
}

// Non physics collisions. Now only player special attack collision
void PhysicsManager::checkForCollisions()
{
	for(OdePlayerMapIterator it_player = mPlayerMap.begin(); it_player != mPlayerMap.end(); ++it_player)
	{
		PlayerPtr player = it_player->second;
		AxisAlignedBox player_firebox = player->getFireBox();

		// Only check if player is using special and last check was not using it
		if ( mPlayerSpecialState != player->isSpecial() && player->isSpecial())
		{
			for(OdeEnemyMapIterator it_enemy = mEnemyMap.begin(); it_enemy != mEnemyMap.end(); ++it_enemy)
			{
				EnemyPtr enemy = it_enemy->second;
				AxisAlignedBox enemy_box = enemy->getSceneNode()->_getWorldAABB();
				
				if(player_firebox.intersects(enemy_box))
				{
					EnemySpecialHitEventPtr enemySpecialHitEventPtr = EnemySpecialHitEventPtr(new EnemySpecialHitEvent(enemy, player));
					raiseEvent(enemySpecialHitEventPtr);
				}
			}
		}
		// Save last state
		mPlayerSpecialState = player->isSpecial();
	}
}

void PhysicsManager::addEnemy(EnemyPtr enemy)
{

	Ogre::String id = enemy->getName();

	AxisAlignedBox aab = enemy->getSceneNode()->getAttachedObject(id)->getBoundingBox(); 
	Ogre::Vector3 min = aab.getMinimum()*enemy->getScale();
	Ogre::Vector3 max = aab.getMaximum()*enemy->getScale();
	Ogre::Vector3 size(fabs(max.x-min.x),fabs(max.y-min.y),fabs(max.z-min.z));
	float radius = (size.x>size.z)?size.z:size.x;

	//Geometry
	OgreOde::Body* body = new OgreOde::Body(mWorld,id+"_Body"); 
	body->setMass(OgreOde::CapsuleMass(70,radius/2,Vector3::UNIT_Y,radius/2)); 
	body->setAffectedByGravity(true);
	OgreOde::TransformGeometry* transform = new OgreOde::TransformGeometry(mWorld,mSpace); 
	OgreOde::CapsuleGeometry* geometry = new OgreOde::CapsuleGeometry(radius,size.y/4,mWorld); 
	geometry->setOrientation(Quaternion(Degree(90),Vector3::UNIT_X));
	transform->setBody(body); 
	transform->setEncapsulatedGeometry(geometry);
	enemy->getSceneNode()->attachObject(body);
	enemy->setBody(body);

	//RayInfo
	PhysicsRayInfo rayInfo;
	rayInfo.geometry = new OgreOde::RayGeometry(Ogre::Real(20), mWorld, mSpace);
	rayInfo.radius = radius;
	rayInfo.updated = false;
	rayInfo.lastContact = Vector3(0,0,0);

	enemy->setRayInfo(rayInfo);

	//
	// Store the enemy into an internal map
	//
	mEnemyMap[enemy->getGeometryId()] = enemy;
	mEnemyGeomMap[body->getGeometry(0)->getID()] = enemy;

	_bodies.push_back(body);
	_geoms.push_back(geometry);
}

void PhysicsManager::addAnimal(EnemyPtr animal)
{

	Ogre::String id = animal->getName();

	AxisAlignedBox aab = animal->getSceneNode()->getAttachedObject(id)->getBoundingBox(); 
	Ogre::Vector3 min = aab.getMinimum()*animal->getScale();
	Ogre::Vector3 max = aab.getMaximum()*animal->getScale();
	Ogre::Vector3 size(fabs(max.x-min.x),fabs(max.y-min.y),fabs(max.z-min.z));
	float radius = (size.x>size.z)?size.z:size.x;

	OgreOde::SimpleSpace* dollSpace = new OgreOde::SimpleSpace(mWorld,mSpace);
	dollSpace->setInternalCollisions(false);

	//Geometry
	OgreOde::Body* dollFeetBody = new OgreOde::Body(mWorld,id+"_Body");
	dollFeetBody->setMass(OgreOde::SphereMass(70,radius)); 
	OgreOde::SphereGeometry* feetGeom = new OgreOde::SphereGeometry(radius/4,mWorld);
	OgreOde::TransformGeometry* feetTrans = new OgreOde::TransformGeometry(mWorld,mSpace); 
	feetTrans->setBody(dollFeetBody); 
	feetTrans->setEncapsulatedGeometry(feetGeom);
	animal->getSceneNode()->attachObject(dollFeetBody);

	OgreOde::Body* dollTorsoBody = new OgreOde::Body(mWorld,id+"_Body_Torso"); 
	dollTorsoBody->setMass(OgreOde::CapsuleMass(70*2.5,radius,Vector3::UNIT_Y,radius)); 
	dollTorsoBody->setAffectedByGravity(true);
	dollTorsoBody->setDamping(0,50000);
	OgreOde::TransformGeometry* torsoTrans = new OgreOde::TransformGeometry(mWorld,mSpace); 
	OgreOde::CapsuleGeometry* torsoGeom = new OgreOde::CapsuleGeometry(radius/4,size.y/10,mWorld); 
	torsoGeom->setPosition(Ogre::Vector3(0,size.y-((size.y-4*radius)/2+2*radius),0));
	torsoGeom->setOrientation(Quaternion(Degree(90),Vector3::UNIT_X));
	torsoTrans->setBody(dollTorsoBody); 
	torsoTrans->setEncapsulatedGeometry(torsoGeom);
	animal->getSceneNode()->attachObject(dollTorsoBody);

	OgreOde::HingeJoint* joint = new OgreOde::HingeJoint(mWorld);
	joint->attach(dollTorsoBody,dollFeetBody);
	joint->setAxis(Ogre::Vector3::UNIT_X);	//set the rotation axis

	//Set Bodys to BodyList
	animal->setBody(dollTorsoBody);
	animal->getBodyList()->push_back(dollFeetBody);
	animal->getBodyList()->push_back(dollTorsoBody);

	//
	// Store the animal into an internal map by torso geom
	//
	mAnimalGeomMap[dollTorsoBody->getGeometry(0)->getID()] = animal;

	_bodies.push_back(dollFeetBody);
	_bodies.push_back(dollTorsoBody);
	_geoms.push_back(feetGeom);
	_geoms.push_back(torsoGeom);
	_joints.push_back(joint);

}

void PhysicsManager::updateRay(PlayerPtr player)
{
	// raise desired ray position a little above player's scenenode
	Vector3 position = player->getPosition();
	// fire ray downward
	player->getRayInfo()->geometry->setDefinition(position,Vector3::NEGATIVE_UNIT_Y);
	// add ray to collisionListener
	player->getRayInfo()->geometry->collide(geom_ground,this);
}

void PhysicsManager::updateRay(EnemyPtr enemy)
{
	// raise desired ray position a little above enemy's scenenode
	Vector3 position = enemy->getPosition();
	// fire ray downward
	enemy->getRayInfo()->geometry->setDefinition(position,Vector3::NEGATIVE_UNIT_Y);
	// add ray to collisionListener
	enemy->getRayInfo()->geometry->collide(geom_ground,this);
}


bool PhysicsManager::collision(OgreOde::Contact* contact)
{

	CollisionEventPtr collisionEventPtr = CollisionEventPtr(new CollisionEvent());
	raiseEvent(collisionEventPtr);

	// search through ode_characters and adjust each charNode's height
	OdePlayerMapIterator it_p;
	
	// Check if the player is the first geometry
	it_p = mPlayerMap.find(contact->getFirstGeometry()->getID());
	
	// ...or the second
	if(it_p == mPlayerMap.end())
		it_p = mPlayerMap.find(contact->getSecondGeometry()->getID());

	if(it_p != mPlayerMap.end())
	{
		it_p->second->getRayInfo()->updated = true;
		it_p->second->getRayInfo()->lastContact = contact->getPosition();
	}

	// search through ode_characters and adjust each charNode's height
	OdeEnemyMapIterator it_e;
	
	// Check if the player is the first geometry
	it_e = mEnemyMap.find(contact->getFirstGeometry()->getID());
	
	// ...or the second
	if(it_e == mEnemyMap.end())
		it_e = mEnemyMap.find(contact->getSecondGeometry()->getID());

	if(it_e != mEnemyMap.end())
	{
		it_e->second->getRayInfo()->updated = true;
		it_e->second->getRayInfo()->lastContact = contact->getPosition();
	}
	
	// if one is an enemy and the other one is a player...
	//if(it_e != mEnemyMap.end() && it_p != mPlayerMap.end())
	//{
	//	EnemyHitEventPtr enemyHitEventPtr = EnemyHitEventPtr(new EnemyHitEvent(it_e->second, it_p->second));
	//	raiseEvent(enemyHitEventPtr);
	//}

	// Player - Enemy Collision
	OdePlayerMapIterator it_pp;
	OdeEnemyMapIterator it_ee;
	it_pp = mPlayerGeomMap.find(contact->getFirstGeometry()->getID());
	if(it_pp == mPlayerGeomMap.end())
	{
		it_pp = mPlayerGeomMap.find(contact->getSecondGeometry()->getID());
	}
	else
	{
		//Player is first - check if enemy is second
		it_ee = mEnemyGeomMap.find(contact->getSecondGeometry()->getID());
		if(it_ee != mEnemyGeomMap.end())
		{
			// Enemy is second

			// Check if player is attacking and has changed state
			if( mPlayerAttackLast != it_pp->second->wichAttack() && it_pp->second->isAttacking() )
			{
				// Erease enemy if player is using third attack
				if(it_pp->second->wichAttack() == 3)
				{
					EnemyKillEventPtr enemyKillEventPtr = EnemyKillEventPtr(new EnemyKillEvent(it_ee->second, it_pp->second));
					raiseEvent(enemyKillEventPtr);
				}
				else
				{
					EnemyHitEventPtr enemyHitEventPtr = EnemyHitEventPtr(new EnemyHitEvent(it_ee->second, it_pp->second));
					raiseEvent(enemyHitEventPtr);
				}
			}
			// Save last attack
			mPlayerAttackLast = it_pp->second->wichAttack();
		}
	}
	if(it_pp != mPlayerGeomMap.end())
	{
		// Player is second - check if enemy is first
		it_ee = mEnemyGeomMap.find(contact->getFirstGeometry()->getID());
		if(it_ee != mEnemyGeomMap.end())
		{
			// Enemy is first

			// Check if player is attacking and has changed state
			if( mPlayerAttackLast != it_pp->second->wichAttack() && it_pp->second->isAttacking() )
			{
				// Erease enemy if player is using third attack
				if(it_pp->second->wichAttack() == 3)
				{
					EnemyKillEventPtr enemyKillEventPtr = EnemyKillEventPtr(new EnemyKillEvent(it_ee->second, it_pp->second));
					raiseEvent(enemyKillEventPtr);
				}
				else
				{
					EnemyHitEventPtr enemyHitEventPtr = EnemyHitEventPtr(new EnemyHitEvent(it_ee->second, it_pp->second));
					raiseEvent(enemyHitEventPtr);
				}				
				if(it_e != mEnemyMap.end())
					mEnemyMap.erase(it_e);
			}
			// Save last attack
			mPlayerAttackLast = it_pp->second->wichAttack();
		}
	}

	contact->setCoulombFriction(OgreOde::Utility::Infinity);

	return true;
}

void PhysicsManager::move(PlayerPtr player, Vector3 direction){

	float const maxVel = 80;
	OgreOde::Body* body = player->getBody();
	float actualVel = body->getLinearVelocity().length();

	if(direction != Vector3::ZERO)
	{
		Quaternion q1 = body->getOrientation();
		// Get current direction where player is facing
		Vector3 currentDirection = q1 * Vector3::UNIT_Z;
		Quaternion q2 = currentDirection.getRotationTo(direction);
		body->setOrientation(q1*q2);
	}

	if(direction == Vector3::ZERO)
	{
		body->setLinearVelocity(Vector3(0,0,0));
	}
	else
	{
		if(player->getLastDirection() != direction)
		{
			//If direction change, set new linear velocity
			Quaternion q4 = body->getLinearVelocity().getRotationTo(direction);
			body->setLinearVelocity(q4 * body->getLinearVelocity());
		}
		if(actualVel > maxVel)
			body->setForce(Vector3(0,0,0));
		else
			body->addForce(direction * Vector3((maxVel * 400),0,(maxVel * 400)));


	}

	player->setLastDirection(direction);

	Quaternion q = body->getOrientation();         
    Vector3 x = q.xAxis();
    Vector3 y = q.yAxis();
    Vector3 z = q.zAxis();
    body->wake();
    body->setOrientation(Quaternion(x,Vector3::UNIT_Y,z));
    body->setAngularVelocity(Vector3(0,0,0)); 
    body->setLinearVelocity(Vector3(body->getLinearVelocity().x,0,body->getLinearVelocity().z));

	// update ray position
	if(player->getRayInfo()->updated){
		body->setPosition(Vector3(body->getPosition().x,player->getRayInfo()->lastContact.y+25,body->getPosition().z));
		player->getRayInfo()->updated = false;
    }
}


void move(EnemyPtr enemy, int rotate, int thrust)
{
	//
	// TODO
	//
}

void PhysicsManager::moveAnimal(EnemyPtr animal, Ogre::Vector3 direction)
{
	
	float const maxVel = 80;
	OgreOde::Body* body_feet = animal->getBodyList()[0][0];
	OgreOde::Body* body_torso = animal->getBodyList()[0][1];

	if(direction == Vector3::ZERO){
		//Stop animal
		body_feet->wake();
		body_feet->setAngularVelocity(Vector3(0,0,0));
	}else{
		//Move animal
		Quaternion q1 = body_torso->getOrientation();
		Vector3 currentDirection = q1 * Vector3::UNIT_Z;
		Quaternion q2 = currentDirection.getRotationTo(direction);
		body_torso->setOrientation(q1*q2);
		
		if(animal->getLastDirection() != direction)
		{
			Quaternion q4 = body_torso->getLinearVelocity().getRotationTo(direction);
			body_feet->wake();
			body_feet->setAngularVelocity(q4*Ogre::Vector3(maxVel*cos(1.0),body_feet->getAngularVelocity().y,0));
		}else{
			body_feet->wake();
			body_feet->setAngularVelocity(direction*Ogre::Vector3(maxVel*cos(1.0),body_feet->getAngularVelocity().y,0));
		}

	}

	Quaternion q = body_torso->getOrientation();         
	Vector3 x = q.xAxis();
	Vector3 y = q.yAxis();
	Vector3 z = q.zAxis();
	body_torso->wake();
	body_torso->setOrientation(Quaternion(x,Vector3::UNIT_Y,z));
	body_torso->setAngularVelocity(Vector3(0,0,0)); 
	body_torso->setLinearVelocity(Vector3(0,0,0));
	animal->setLastDirection(direction);	
}


// --------------------------------
// Lua Physics Lib
// --------------------------------
LUA_BEGIN_BINDING(PhysicsManager::physicslib)
LUA_BIND("getHOT", PhysicsManager::LuaGetHOT)
LUA_BIND("getDistance", PhysicsManager::LuaGetDistance)
LUA_BIND("getNearestPlayer", PhysicsManager::LuaGetNearestPlayer)
LUA_END_BINDING()

int PhysicsManager::LuaGetHOT(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 3

	Ogre::Real x = luaL_checknumber(L, 1);
	Ogre::Real y = luaL_checknumber(L, 2);
	Ogre::Real z = luaL_checknumber(L, 3);

	// TODO :
	//smPhysicsManager->getHOT

	/* push the total seconds */
	lua_pushnumber(L, 0); // 

	/* return the number of results */
	return 1;
}

int PhysicsManager::LuaGetDistance(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 2

	Ogre::String name1 = luaL_checkstring(L, 1);
	Ogre::String name2 = luaL_checkstring(L, 2);

	Ogre::SceneManager* sceneManager = PhysicsManager::getSingleton().getSceneManager();

	SceneNode* node1 = sceneManager->getEntity(name1)->getParentSceneNode();
	SceneNode* node2 = sceneManager->getEntity(name2)->getParentSceneNode();

    Vector3 pos1 = node1->_getDerivedPosition();
    Vector3 pos2 = node2->_getDerivedPosition();
      
    //euclidian distance, using x and z, 
    //as y is vertical in this coordinate system
    double range = sqrt( pow((pos1.x - pos2.x), 2) + pow((pos1.z - pos2.z), 2));

	/* push the distance */
	lua_pushnumber(L, range); // 

	/* return the number of results */
	return 1;
}

int PhysicsManager::LuaGetNearestPlayer(lua_State *L)
{
		/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 2

	Ogre::String enemyId = luaL_checkstring(L, 1);

	 // TODO Retrieve the REAL nearest player!
	//Ogre::SceneManager* sceneManager = PhysicsManager::getSingleton().getSceneManager();

	//SceneNode* node1 = sceneManager->getEntity(name1)->getParentSceneNode();

	//SceneNode* node2 = sceneManager->getEntity("Player1")->getParentSceneNode();

 //   Vector3 pos1 = node1->_getDerivedPosition();
 //   Vector3 pos2 = node2->_getDerivedPosition();
 //     
 //   //euclidian distance, using x and z, 
 //   //as y is vertical in this coordinate system
 //   double range = sqrt( pow((pos1.x - pos2.x), 2) + pow((pos1.z - pos2.z), 2));

	Ogre::String playerId = "Player1"; // TODO Retrieve the REAL nearest player!

	/* push the id of the closest player */
	lua_pushstring(L, playerId.c_str()); // 

	/* return the number of results */
	return 1;
}
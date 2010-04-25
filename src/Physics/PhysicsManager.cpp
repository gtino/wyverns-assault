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
	//
	// TODO Distructor logic HERE
	//
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

	// Set collision space
	mSpace = mWorld->getDefaultSpace();

	// Create stepper
	const Ogre::Real time_scale = Ogre::Real(1.0); 
    const Ogre::Real max_frame_time = Ogre::Real(1.0 / 4);
    const Ogre::Real frame_rate = Ogre::Real(1.0 / 60);

	OgreOde::StepHandler::StepModeType stepModeType = OgreOde::StepHandler::QuickStep;
	mStepper = new OgreOde::ForwardFixedStepHandler(mWorld, stepModeType, STEP_RATE, max_frame_time, time_scale);

	return true;
}

void PhysicsManager::finalize()
{
	//
	// TODO
	//
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

void PhysicsManager::createGround(Ogre::String mesh)
{
	SceneNode* node_ground = mSceneManager->getRootSceneNode()->createChildSceneNode("Physic_ground_node",Vector3(0,0,0));
	Entity* ent_ground = mSceneManager->createEntity("Physic_ground",mesh);
	node_ground->attachObject(ent_ground);
	OgreOde::EntityInformer ei_ground(ent_ground,node_ground->_getFullTransform());
	geom_ground = ei_ground.createStaticTriangleMesh(mWorld,mSpace);
	ent_ground->setUserAny(Ogre::Any(geom_ground));
	node_ground->setVisible(false);

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
	OgreOde::CapsuleGeometry* geometry = new OgreOde::CapsuleGeometry(radius/2,size.y/5,mWorld); 
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
	mPlayerMap[id] = player;
}


void PhysicsManager::addEnemy(EnemyPtr enemy)
{
	Ogre::String id = enemy->getName();

	AxisAlignedBox aab = enemy->getSceneNode()->getAttachedObject(id)->getBoundingBox(); 
	Ogre::Vector3 min = aab.getMinimum()*enemy->getScale();
	Ogre::Vector3 max = aab.getMaximum()*enemy->getScale();
	Ogre::Vector3 size(fabs(max.x-min.x),fabs(max.y-min.y),fabs(max.z-min.z));
	float radius = (size.x>size.z)?size.z/2.0f:size.x/2.0f;

	//Geometry
	OgreOde::Body* body = new OgreOde::Body(mWorld,id+"_Body"); 
	body->setMass(OgreOde::CapsuleMass(70,radius/2,Vector3::UNIT_Y,radius/2)); 
	body->setAffectedByGravity(true);
	OgreOde::TransformGeometry* transform = new OgreOde::TransformGeometry(mWorld,mSpace); 
	OgreOde::CapsuleGeometry* geometry = new OgreOde::CapsuleGeometry(radius/2,size.y/5,mWorld); 
	geometry->setOrientation(Quaternion(Degree(90),Vector3::UNIT_X));
	transform->setBody(body); 
	transform->setEncapsulatedGeometry(geometry);
	enemy->getSceneNode()->attachObject(body);
	enemy->setBody(body);

	//RayInfo
	PhysicsRayInfo rayInfo;
	rayInfo.geometry = new OgreOde::RayGeometry(Ogre::Real(30), mWorld, mSpace);
	rayInfo.radius = radius;
	rayInfo.updated = false;
	rayInfo.lastContact = Vector3(0,0,0);

	enemy->setRayInfo(rayInfo);

	//
	// Store the enemy into an internal map
	//
	mEnemyMap[id] = enemy;
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

	// search through ode_characters and adjust each charNode's height
	for( PlayerMapIterator it = mPlayerMap.begin(); it != mPlayerMap.end(); it++ )
	{
		if( contact->getFirstGeometry()->getID() == it->second->getGeometryId() ||
			contact->getSecondGeometry()->getID() == it->second->getGeometryId() )
		{
			it->second->getRayInfo()->updated = true;
			it->second->getRayInfo()->lastContact = contact->getPosition();
			break;
		}
	}

	contact->setCoulombFriction(OgreOde::Utility::Infinity);
	return true;
}

void PhysicsManager::move(PlayerPtr player, int rotate, int thrust){

	float const maxVel = 80;
	OgreOde::Body* body = player->getBody();
	float actualVel = body->getLinearVelocity().length();

	if (rotate != 0)
	{
		//MOVEMENT UPDATE
		// Change autotrack axis for facing movement direction
		//mSceneNode->setAutoTracking(false, mAutoTrackingNode, mDirection);
		
		Quaternion q1 = body->getOrientation();
		Quaternion q2(Degree(-4*rotate),Ogre::Vector3::UNIT_Y);
		body->setOrientation(q1*q2);
		body->setLinearVelocity(Vector3(0,body->getLinearVelocity().y,0));
	}


	if (thrust == 0)
	{
		body->setLinearVelocity(Vector3(0,0,0));
	}
	else
	{
		if(actualVel > maxVel)
			body->setForce(Vector3(0,0,0));
		else
			body->addForce(body->getOrientation() * Vector3(0,0,thrust*(maxVel * 400)));
	}

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
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

void PhysicsManager::showDebuggObjects()
{
	mWorld->setShowDebugGeometries(!mWorld->getShowDebugGeometries());
}

void PhysicsManager::createPhysicGround(Ogre::String mesh)
{
	SceneNode* node_ground = mSceneManager->getRootSceneNode()->createChildSceneNode("Physic_ground_node",Vector3(0,0,0));
	Entity* ent_ground = mSceneManager->createEntity("Physic_ground",mesh);
	node_ground->attachObject(ent_ground);
	OgreOde::EntityInformer ei_ground(ent_ground,node_ground->_getFullTransform());
	geom_ground = ei_ground.createStaticTriangleMesh(mWorld,mSpace);
	ent_ground->setUserAny(Ogre::Any(geom_ground));
	node_ground->setVisible(false);

}


void PhysicsManager::createPhysicCharacter(Ogre::String name, PlayerPtr mPlayer)
{

	AxisAlignedBox aab = mPlayer->getSceneNodeModel()->getAttachedObject(name)->getBoundingBox(); 
	Ogre::Vector3 min = aab.getMinimum()*mPlayer->getScale();
	Ogre::Vector3 max = aab.getMaximum()*mPlayer->getScale();
	Ogre::Vector3 size(fabs(max.x-min.x),fabs(max.y-min.y),fabs(max.z-min.z));
	float radius = (size.x>size.z)?size.z/2.0f:size.x/2.0f;

	//Geometry
	OgreOde::Body* dollTorsoBody = new OgreOde::Body(mWorld,"torso"); 
	dollTorsoBody->setMass(OgreOde::CapsuleMass(70,radius/2,Vector3::UNIT_Y,radius/2)); 
	dollTorsoBody->setAffectedByGravity(true);
	OgreOde::TransformGeometry* torsoTrans = new OgreOde::TransformGeometry(mWorld,mSpace); 
	OgreOde::CapsuleGeometry* torsoGeom = new OgreOde::CapsuleGeometry(radius/2,size.y/5,mWorld); 
	torsoGeom->setOrientation(Quaternion(Degree(90),Vector3::UNIT_X));
	torsoTrans->setBody(dollTorsoBody); 
	torsoTrans->setEncapsulatedGeometry(torsoGeom);
	mPlayer->getSceneNode()->attachObject(dollTorsoBody);
	mPlayer->setTorso(dollTorsoBody);

	//Ray
	ODE_CHAR_INFO ray;
	ray.charRay = new OgreOde::RayGeometry(Ogre::Real(30),mWorld,mSpace);
	ray.radius = radius;
	ray.updated = false;
	ray.last_contact = Vector3(0,0,0);
	ode_characters.push_back(ray);

}


void PhysicsManager::createPhysicEnemy(Ogre::String name, Ogre::String mesh)
{
	//
	// TODO
	//
}

void PhysicsManager::updateRay(PlayerPtr mPlayer)
{
	Vector3 position;
	for( std::vector< ODE_CHAR_INFO >::iterator it = ode_characters.begin(); it != ode_characters.end(); it++ )
	{
  		// raise desired ray position a little above character's scenenode
		position = mPlayer->getPosition();
		// fire ray downward
		it->charRay->setDefinition(position,Vector3::NEGATIVE_UNIT_Y);
		// add ray to collisionListener
		it->charRay->collide(geom_ground,this);
	}

}

bool PhysicsManager::collision(OgreOde::Contact* contact)
{

	// search through ode_characters and adjust each charNode's height
	for( std::vector< ODE_CHAR_INFO >::iterator it = ode_characters.begin(); it != ode_characters.end(); it++ )
	{
		if( contact->getFirstGeometry()->getID() == it->charRay->getID() ||
			contact->getSecondGeometry()->getID() == it->charRay->getID() )
		{
			it->updated = true;
			it->last_contact = contact->getPosition();
			break;
		}
	}

	contact->setCoulombFriction(OgreOde::Utility::Infinity);
	return true;
}

void PhysicsManager::move(PlayerPtr mPlayer, int rotate, int thrust){

	float const maxVel = 80;
	OgreOde::Body* act_torso = mPlayer->getTorso();
	float actualVel = act_torso->getLinearVelocity().length();

	if (rotate != 0)
	{
		//MOVEMENT UPDATE
		// Change autotrack axis for facing movement direction
		//mSceneNode->setAutoTracking(false, mAutoTrackingNode, mDirection);
		
		Quaternion q1 = act_torso->getOrientation();
		Quaternion q2(Degree(-4*rotate),Ogre::Vector3::UNIT_Y);
		act_torso->setOrientation(q1*q2);
		act_torso->setLinearVelocity(Vector3(0,act_torso->getLinearVelocity().y,0));
	}


	if (thrust == 0)
	{
		act_torso->setLinearVelocity(Vector3(0,0,0));
	}
	else
	{
		if(actualVel > maxVel)
			act_torso->setForce(Vector3(0,0,0));
		else
			act_torso->addForce(act_torso->getOrientation() * Vector3(0,0,thrust*(maxVel * 400)));
	}

	Quaternion q = act_torso->getOrientation();         
    Vector3 x = q.xAxis();
    Vector3 y = q.yAxis();
    Vector3 z = q.zAxis();
    act_torso->wake();
    act_torso->setOrientation(Quaternion(x,Vector3::UNIT_Y,z));
    act_torso->setAngularVelocity(Vector3(0,0,0)); 
    act_torso->setLinearVelocity(Vector3(act_torso->getLinearVelocity().x,0,act_torso->getLinearVelocity().z));

	//Ray position of all characters
	if(ode_characters[0].updated){
		act_torso->setPosition(Vector3(act_torso->getPosition().x,ode_characters[0].last_contact.y+25,act_torso->getPosition().z));
		ode_characters[0].updated = false;
    }
	 
}

// --------------------------------
// Lua Physics Lib
// --------------------------------
LUA_BEGIN_BINDING(PhysicsManager::physicslib)
LUA_BIND("getHOT", PhysicsManager::getHOT)
LUA_END_BINDING()

int PhysicsManager::getHOT(lua_State *L)
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
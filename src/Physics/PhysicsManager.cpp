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
: mPhysicsNode(0)
{
	mSceneManager = sceneManager;
}

PhysicsManager::~PhysicsManager()
{
	finalize();
}

bool PhysicsManager::initialize()
{
	mLastAttackChecked = 0;

	mPhysicsNode = mSceneManager->getRootSceneNode()->createChildSceneNode(PHYSICS_NODE_NAME);

	// Initialize RaySceneQuery
	mRaySceneQuery = mSceneManager->createRayQuery(Ogre::Ray());

    if (NULL == mRaySceneQuery)
    {
		return false;
    }
    mRaySceneQuery->setSortByDistance(true);

	return true;
}

void PhysicsManager::finalize()
{
	mPlayerMap.clear();
	mEnemyMap.clear();

	Utils::Destroy(mSceneManager, PHYSICS_NODE_NAME);
	mPhysicsNode = NULL;
}


void PhysicsManager::showDebugObjects()
{
	/*TODO
	*/
}

void PhysicsManager::update(const float elapsedSeconds)
{
	// Skip update if physics have been disabled (maybe via Lua)
	if(!mEnabled)
		return;

	// Update players
	for(PlayerMapIterator it_player = mPlayerMap.begin(); it_player != mPlayerMap.end(); ++it_player)
	{
		PlayerPtr player = it_player->second;
		Vector3 position = player->getPosition();		

		// Update player ray
		Vector3 hotPosition = calculateY(position);
		hotPosition.y += player->getHeight();
		player->setPosition(hotPosition);

		// Update player move
		move(player,elapsedSeconds);
	}

	// Update enemys
	for(EnemyMapIterator it_enemy = mEnemyMap.begin(); it_enemy != mEnemyMap.end(); ++it_enemy)
	{
		EnemyPtr enemy = it_enemy->second;
		Vector3 position = enemy->getPosition();

		// Update enemy ray
		Vector3 hotPosition = calculateY(position);
		hotPosition.y += enemy->getHeight();
		enemy->setPosition(hotPosition);

		//Update player move
		move(enemy,elapsedSeconds);
	}

	// Call bounding box collision
	checkForCollisions();
}

// Bounding-Box collision
void PhysicsManager::checkForCollisions()
{
	// Player Collisions
	for(PlayerMapIterator it_player = mPlayerMap.begin(); it_player != mPlayerMap.end(); ++it_player)
	{
		PlayerPtr player = it_player->second;
		AxisAlignedBox player_firebox = player->getFireBox();
		AxisAlignedBox player_box = player->getGeometry()->getWorldBoundingBox();

		// Player - Enemy COLLISION
		for(EnemyMapIterator it_enemy = mEnemyMap.begin(); it_enemy != mEnemyMap.end(); ++it_enemy)
		{
			EnemyPtr enemy = it_enemy->second;
			AxisAlignedBox enemy_box = enemy->getGeometry()->getWorldBoundingBox();

			// Check if player is using special (fire) and collisioning with enemy
			if ( player->isSpecial() && player_firebox.intersects(enemy_box))
			{
				EnemyHitEventPtr enemyHitEventPtr = EnemyHitEventPtr(new EnemyHitEvent(enemy, player));
				enemyHitEventPtr->setDamage(player->getSpecialHitDamage());
				EVENTS_FIRE(enemyHitEventPtr);
			}

			// Player and enemy are colliding
			if(player_box.intersects(enemy_box))
			{
				// Check if player is attacking and has changed state
				if( player->isAttacking() && mLastAttackChecked != player->wichAttack() )
				{
					EnemyHitEventPtr enemyHitEventPtr = EnemyHitEventPtr(new EnemyHitEvent(enemy, player));
					// If thrid strike more damage
					if( player->wichAttack() == 3 )
						enemyHitEventPtr->setDamage(player->getComboHitDamage());
					else
 						enemyHitEventPtr->setDamage(player->getHitDamage());

					EVENTS_FIRE(enemyHitEventPtr);
				}
				// Check if enemy is attacking
				if( enemy->isAttacking() && !enemy->hasAttackHited() )
				{
					PlayerHitEventPtr playerHitEventPtr = PlayerHitEventPtr(new PlayerHitEvent(enemy, player));
					EVENTS_FIRE(playerHitEventPtr);

					enemy->setAttackHited(true);
				}
				// Colision with chicken if moving. Chicken mash!
				if ( enemy->getEnemyType() == Enemy::EnemyTypes::Chicken && player->isMoving())
				{
					EnemyHitEventPtr enemyHitEventPtr = EnemyHitEventPtr(new EnemyHitEvent(enemy, player));
					enemyHitEventPtr->setDamage(player->getHitDamage());
					EVENTS_FIRE(enemyHitEventPtr);
				}
				// Collision withou attack
			}			
		}

		// Save last player attack checked
		mLastAttackChecked = player->wichAttack();

		// Player - Item COLLISION
		for(ItemMapIterator it_item = mItemMap.begin(); it_item != mItemMap.end(); ++it_item)
		{
			ItemPtr item = it_item->second;
			AxisAlignedBox item_box = item->getGeometry()->getWorldBoundingBox();

			if(player_box.intersects(item_box))
			{
				//Item chatched
				ItemCatchEventPtr evt = ItemCatchEventPtr(new ItemCatchEvent(player, item));
				EVENTS_FIRE(evt);
			}
		}
	}

	// Enemy Collisions
	for(EnemyMapIterator it_enemy = mEnemyMap.begin(); it_enemy != mEnemyMap.end(); ++it_enemy)
	{
		EnemyPtr enemy = it_enemy->second;
		AxisAlignedBox enemy_box = enemy->getGeometry()->getWorldBoundingBox();

		// Enemy - Enemy COLLISION
		for(EnemyMapIterator it_enemy_second = mEnemyMap.begin(); it_enemy_second != mEnemyMap.end(); ++it_enemy_second)
		{
			EnemyPtr enemy_second = it_enemy_second->second;
			AxisAlignedBox enemy_second_box = enemy_second->getGeometry()->getWorldBoundingBox();

			// Check if player is using special (fire) and collisioning with enemy
			if (enemy_box.intersects(enemy_second_box))
			{
				// TODO: Enemy-enemy Collision

			}
		}
	}
}

void PhysicsManager::move(PlayerPtr player, const float elapsedSeconds, bool fastMode)
{

	Vector3 direction = player->getDirection();

	if(direction != Vector3::ZERO)
	{
		Quaternion q1 = player->getOrientation();
		// Get current direction where player is facing
		Vector3 currentDirection = q1 * Vector3::UNIT_Z;
		Quaternion q2 = currentDirection.getRotationTo(direction);
		player->setOrientation(q1*q2);
	}

	Vector3 old_position = player->getPosition();
	
	player->setPosition((direction * player->getSpeed() * elapsedSeconds) + old_position);

	//Test object collision
	bool objCollision = collidesAllObjects(player, old_position,player->getPosition(),0.5f,0.0);
	//Test border collision
	bool borderCollision = collides(old_position,player->getPosition(),borderGround->getPhysicsMeshInfo(),0.5f,0);
	
	if( borderCollision || objCollision  )
		player->setPosition(old_position);
}


void PhysicsManager::move(EnemyPtr enemy, const float elapsedSeconds)
{
	Vector3 direction = enemy->getDirection();

	Vector3 old_position = enemy->getPosition();

	enemy->setPosition((direction*enemy->getSpeed()*elapsedSeconds) + old_position);

	if(collides(old_position,enemy->getPosition(),borderGround->getPhysicsMeshInfo(),0.5f,0))
		enemy->setPosition(old_position);
}

// Load a mesh as ground type
void PhysicsManager::addPhysicGround(Ogre::String mesh, Ogre::String name, WyvernsAssault::GroundQueryFlags type, Ogre::Vector3 position, Ogre::Vector3 scale)
{	
	SceneNode* nodeGround = mPhysicsNode->createChildSceneNode(name,position);
	Ogre::Entity* entityGround = mSceneManager->createEntity(name,mesh);
	entityGround->setQueryFlags(type);
	nodeGround->attachObject(entityGround);
	nodeGround->setVisible(false);
	nodeGround->setScale(scale);

	// initialize geometry
	if(type == WyvernsAssault::BASIC_GROUND_MASK){
		basicGround = GeometryPtr(new Geometry(entityGround));
		basicGround->initializeMeshInformation(nodeGround->getPosition(),nodeGround->getOrientation(),nodeGround->getScale());
	}else if(type == WyvernsAssault::BORDER_GROUND_MASK){
		borderGround = GeometryPtr(new Geometry(entityGround));
		borderGround->initializeMeshInformation(nodeGround->getPosition(),nodeGround->getOrientation(),nodeGround->getScale());
	}
}

// Load player physics
void PhysicsManager::addPhysicPlayer(PlayerPtr player)
{

	mPlayerMap[player->getName()] = player;

}

// Load enemy physics
void PhysicsManager::addPhysicEnemy(EnemyPtr enemy)
{
	mEnemyMap[enemy->getName()] = enemy;
}

// Load item physics
void PhysicsManager::addPhysicItem(ItemPtr item)
{
	mItemMap[item->getName()] = item;
}

// Load object physics
void PhysicsManager::addPhysicObject(ObjectPtr obj)
{
	mObjectMap[obj->getName()] = obj;
}

// Calculate heigth of terrain and translate node to adjust them
Vector3 PhysicsManager::calculateY(const Vector3 &point)
{
	Vector3 yPosition(0,0,0);
	float distToColl = 0.0f;

	if(raycast(point,Vector3::NEGATIVE_UNIT_Y,yPosition, distToColl ,basicGround->getPhysicsMeshInfo()))
	{
		return Ogre::Vector3(point.x,yPosition.y,point.z);
	}

	return point;
}

// Control player-object collision
bool PhysicsManager::collidesAllObjects(PlayerPtr player, const Vector3& fromPoint, const Vector3& toPoint, const float collisionRadius, const float rayHeightLevel )
{

	AxisAlignedBox player_firebox = player->getFireBox();
	AxisAlignedBox player_box = player->getGeometry()->getWorldBoundingBox();

	//Launch ray
	Vector3 fromPointAdj(fromPoint.x, fromPoint.y + rayHeightLevel, fromPoint.z);
	Vector3 toPointAdj(toPoint.x, toPoint.y + rayHeightLevel, toPoint.z);	
	Vector3 normal = toPointAdj - fromPointAdj;

    static Ogre::Ray ray;
	ray.setOrigin(fromPointAdj);
	ray.setDirection(normal);

	Vector3 translation =  Ogre::Vector3::ZERO;

	for(ObjectMapIterator it_obj = mObjectMap.begin(); it_obj != mObjectMap.end(); ++it_obj)
	{
		ObjectPtr obj = it_obj->second;
		AxisAlignedBox obj_box = obj->getGeometry()->getBoundingBox();
		obj_box.transformAffine(obj->_getSceneNode()->_getFullTransform());

		// Check if player is using special (fire) and collisioning with enemy
		if ( player->isSpecial() && player_firebox.intersects(obj_box))
		{
			ObjectHitEventPtr objectHitEventPtr = ObjectHitEventPtr(new ObjectHitEvent(obj, player));
			objectHitEventPtr->setDamage(player->getSpecialHitDamage());
			EVENTS_FIRE(objectHitEventPtr);
		}else{
			std::pair<bool, Ogre::Real> hit = Ogre::Math::intersects(ray, obj_box);
			
			if(hit.first && hit.second < 2){
				/*
				// Check if player is attacking and has changed state
 				if( player->isAttacking() && mLastAttackChecked != player->wichAttack())
				{
					//EnemyHitEventPtr enemyHitEventPtr = EnemyHitEventPtr(new EnemyHitEvent(enemy, player));
					// If thrid strike more damage
					//if( player->wichAttack() == 3 )
						//enemyHitEventPtr->setDamage(player->getComboHitDamage());
					//else
 					//	enemyHitEventPtr->setDamage(player->getHitDamage());

					//EVENTS_FIRE(enemyHitEventPtr);

					//return true;
					
					// Save last player attack checked
					mLastAttackChecked = player->wichAttack();
				}
				*/

				//Collision wihout attack
				return true;
			}
		}
	}

	return false;

}

bool PhysicsManager::collides(const Vector3& fromPoint, const Vector3& toPoint, PhysicsMeshInfo objInfo, const float collisionRadius, const float rayHeightLevel)
{
	Vector3 fromPointAdj(fromPoint.x, fromPoint.y + rayHeightLevel, fromPoint.z);
	Vector3 toPointAdj(toPoint.x, toPoint.y + rayHeightLevel, toPoint.z);	
	Vector3 normal = toPointAdj - fromPointAdj;
	float distToDest = normal.normalise();

	float distToColl = 0.0f;
	Vector3 myResult(0, 0, 0);

	if (raycast(fromPointAdj, normal, myResult, distToColl ,objInfo))
	{
		distToColl -= collisionRadius; 
		return (distToColl <= distToDest);
	}
	else
	{
		return false;
	}
}

bool PhysicsManager::raycast(const Vector3 &point, const Vector3 &normal,
							 Vector3 &result,float &closest_distance, PhysicsMeshInfo objInfo)
{
    // Create the ray to test
    static Ogre::Ray ray;
	ray.setOrigin(point);
	ray.setDirection(normal);

	closest_distance = -1.0f;
    Ogre::Vector3 closest_result;

	PhysicsMeshInfo meshInfo = objInfo;

	size_t vertex_count = meshInfo.vertex_count;
	size_t index_count = meshInfo.index_count;
	Ogre::Vector3 *vertices = meshInfo.vertices;
	unsigned long *indices = meshInfo.indices;

    // test for hitting individual triangles on the mesh
    bool new_closest_found = false;
    for (int i = 0; i < static_cast<int>(index_count); i += 3)
    {
		// check for a hit against this triangle
        std::pair<bool, Ogre::Real> hit = Ogre::Math::intersects(ray, vertices[indices[i]],
                    vertices[indices[i+1]], vertices[indices[i+2]], true, false);

        // if it was a hit check if its the closest
        if (hit.first)
        {
			if ((closest_distance < 0.0f) || (hit.second < closest_distance))
            {
				// this is the closest so far, save it off
                closest_distance = hit.second;
                new_closest_found = true;
			}
		}
	}

	// if we found a new closest raycast for this object, update the
    // closest_result before moving on to the next object.
    if (new_closest_found)
    {
        closest_result = ray.getPoint(closest_distance);               
	}

    if (closest_distance >= 0.0f)
    {
        // raycast success
		result = closest_result;
        return (true);
    }
    else
    {
        // raycast failed
        return (false);
    } 

}

void PhysicsManager::removeEnemy(EnemyPtr enemy)
{
	mEnemyMap.erase(enemy->getName());
}

void PhysicsManager::removeItem(ItemPtr item)
{
	mItemMap.erase(item->getName());
}

void PhysicsManager::removeObject(ObjectPtr obj)
{
	mObjectMap.erase(obj->getName());
}

// --------------
// Event handlers
// --------------
EVENTS_BEGIN_REGISTER_HANDLERS(PhysicsManager)
	EVENTS_REGISTER_HANDLER(PhysicsManager, EnemyKilled)
	EVENTS_REGISTER_HANDLER(PhysicsManager, ItemCatch);
EVENTS_END_REGISTER_HANDLERS()

EVENTS_BEGIN_UNREGISTER_HANDLERS(PhysicsManager)
	EVENTS_UNREGISTER_HANDLER(PhysicsManager, EnemyKilled)
	EVENTS_UNREGISTER_HANDLER(PhysicsManager, ItemCatch);
EVENTS_END_UNREGISTER_HANDLERS()

EVENTS_DEFINE_HANDLER(PhysicsManager,EnemyKilled)
{
	EnemyPtr enemy = evt->getEnemy();
	PlayerPtr player = evt->getPlayer();

	// The player has just hit the enemy
   	removeEnemy(enemy);
}

EVENTS_DEFINE_HANDLER(PhysicsManager,ItemCatch)
{
	ItemPtr item = evt->getItem();

	// The player has just cacth the item
   	removeItem(item);
}

// --------------------------------
// Lua Physics Lib
// --------------------------------
LUA_BEGIN_BINDING(PhysicsManager, physicslib)
LUA_BIND(PhysicsManager, disable)
LUA_BIND(PhysicsManager, enable)
LUA_BIND(PhysicsManager, isEnabled)
LUA_BIND(PhysicsManager, getHOT)
LUA_BIND(PhysicsManager, getDistance)
LUA_BIND(PhysicsManager, getNearestPlayer)
LUA_END_BINDING()

//
// Load lua scripts that will be used by this manager
//
LUA_BEGIN_LOAD_SCRIPTS(PhysicsManager)
// 
// TODO : Load scripts if needed
//
LUA_END_LOAD_SCRIPTS()

LUA_DEFINE_FUNCTION(PhysicsManager,enable)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 0

	PhysicsManager::getSingleton().enable();

	/* return the number of results */
	return 0;
}

LUA_DEFINE_FUNCTION(PhysicsManager,disable)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 0

	PhysicsManager::getSingleton().disable();

	/* return the number of results */
	return 0;
}

LUA_DEFINE_FUNCTION(PhysicsManager, isEnabled)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 0

	bool isEnabled = PhysicsManager::getSingleton().isEnabled();

	lua_pushboolean(L, isEnabled);

	/* return the number of results */
	return 1;
}

LUA_DEFINE_FUNCTION(PhysicsManager, getHOT)
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

LUA_DEFINE_FUNCTION(PhysicsManager, getDistance)
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

LUA_DEFINE_FUNCTION(PhysicsManager, getNearestPlayer)
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
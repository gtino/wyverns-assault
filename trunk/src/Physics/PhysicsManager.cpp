#include "..\..\include\Physics\PhysicsManager.h"

using namespace WyvernsAssault;

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
, mLastAttackChecked(0)
, mCurrentGameArea(-1)
, mGameAreaCleared(true)
{
	mSceneManager = sceneManager;
}

PhysicsManager::~PhysicsManager()
{
	finalize();
}

void PhysicsManager::initialize()
{
	mPhysicsNode = mSceneManager->getRootSceneNode()->createChildSceneNode(PHYSICS_NODE_NAME);

	// Initialize RaySceneQuery
	mRaySceneQuery = mSceneManager->createRayQuery(Ray());

    mRaySceneQuery->setSortByDistance(true);
}

void PhysicsManager::finalize()
{
	mPlayerMap.clear();
	mEnemyMapList.clear();

	Utils::Destroy(mSceneManager, PHYSICS_NODE_NAME);
	mPhysicsNode = NULL;
}

void PhysicsManager::update(const float elapsedSeconds)
{
	// Skip update if physics have been disabled (maybe via Lua)
	if(!mEnabled)
		return;

	// Update players
	for( PlayerMapIterator it_player = mPlayerMap.begin(); it_player != mPlayerMap.end(); ++it_player )
	{
		PlayerPtr player = it_player->second;
		Vector3 position = player->getPosition();		

		// Update player ray
		Vector3 newPosition = calculateHeight(position);
		newPosition.y += player->getHeight();
		player->setPosition(newPosition);

		// Update player move
		move(player, elapsedSeconds);
	}

	// Update enemies
	for(int i = 0; i < mEnemyMapList[mCurrentGameArea].size(); i++)
	{
		EnemyPtr enemy =  mEnemyMapList[mCurrentGameArea][i];
		Vector3 position = enemy->getPosition();

		// Update enemy ray
		Vector3 newPosition = calculateHeight(position);
		newPosition.y += enemy->getHeight();
		enemy->setPosition(newPosition);

		//Update player move
		move(enemy,elapsedSeconds);
	}

	// Update projectiles
	for(int i = 0; i < mProjectileList.size(); i++)
	{
		ProjectilePtr projectile =  mProjectileList[i];

		//Update projectile move
		move(projectile,elapsedSeconds);

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
		AxisAlignedBox player_box = player->getGeometry()->getWorldBoundingBox(player->getPosition());

		// Player - Enemy COLLISION
		for(int i = 0; i < mEnemyMapList[mCurrentGameArea].size(); i++)
		{
			EnemyPtr enemy = mEnemyMapList[mCurrentGameArea][i];
			AxisAlignedBox enemy_box = enemy->getGeometry()->getWorldBoundingBox(enemy->getPosition());

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
				if( enemy->isAttacking() && !enemy->hasAttackHited() && enemy->getEnemyType() != Enemy::EnemyTypes::Wizard )
				{
					PlayerHitEventPtr playerHitEventPtr = PlayerHitEventPtr(new PlayerHitEvent(enemy, player));
					EVENTS_FIRE_AFTER(playerHitEventPtr, 0.4);

					enemy->setAttackHited(true);
				}
				// Colision with chicken or cow if moving. Animal mash!
				if ( ( enemy->getEnemyType() == Enemy::EnemyTypes::Chicken || enemy->getEnemyType() == Enemy::EnemyTypes::Cow  ) 
					&& player->isMoving() )
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
		for(int i = 0; i < mItemMapList[mCurrentGameArea].size(); i++)
		{
			ItemPtr item = mItemMapList[mCurrentGameArea][i];
			AxisAlignedBox item_box = item->getGeometry()->getWorldBoundingBox(item->getPosition());

			if(player_box.intersects(item_box))
			{
				//Item chatched
				ItemCatchEventPtr evt = ItemCatchEventPtr(new ItemCatchEvent(player, item));
				EVENTS_FIRE(evt);
			}
		}

		//Player - Projectile COLLISION
		for(int i = 0; i < mProjectileList.size(); i++)
		{
			ProjectilePtr projectile = mProjectileList[i];
			AxisAlignedBox projectile_box = projectile->getGeometry()->getWorldBoundingBox(projectile->getPosition());

			if(player_box.intersects(projectile_box))
			{
				//Projectile atacck
				ProjectileHitEventPtr evtHit = ProjectileHitEventPtr(new ProjectileHitEvent(projectile, player));
				EVENTS_FIRE(evtHit);
				//Projectile remove
				ProjectileRemoveEventPtr evtRemove = ProjectileRemoveEventPtr(new ProjectileRemoveEvent(projectile));
				EVENTS_FIRE(evtRemove);
				projectile->death();
			}
		}
	}

	// Enemy Collisions
	for(int i = 0; i < mEnemyMapList[mCurrentGameArea].size(); i++)
	{
		EnemyPtr enemy =  mEnemyMapList[mCurrentGameArea][i];
		AxisAlignedBox enemy_box = enemy->getGeometry()->getWorldBoundingBox(enemy->getPosition());

		// Enemy - Enemy COLLISION
		for(int i = 0; i < mEnemyMapList[mCurrentGameArea].size(); i++)
		{
			EnemyPtr enemy =  mEnemyMapList[mCurrentGameArea][i];
			AxisAlignedBox enemy_second_box = enemy->getGeometry()->getWorldBoundingBox(enemy->getPosition());

			// Check if player is using special (fire) and collisioning with enemy
			if (enemy_box.intersects(enemy_second_box))
			{
				// TODO: Enemy-enemy Collision

			}
		}
	}
}

void PhysicsManager::move(PlayerPtr player, const float elapsedSeconds)
{
	Vector3 direction = player->getDirection();
	Vector3 lastPosition = player->getPosition();
	
	// Translation depends on time since last frame and player's speed
	player->translate( (direction * player->getSpeed() * elapsedSeconds) );

	// Test object collision
	bool objectCollision = collidesAllObjects(player, lastPosition, player->getPosition(), 0.5f, 0.0);

	// Test walls collision
	bool wallCollision = collides(lastPosition, player->getPosition(), mWallGeometry->getPhysicsMeshInfo(),0.5f,0);
	
	// If player is colliding with something, undo movement
	if( objectCollision || wallCollision  )
		player->setPosition(lastPosition);

	// If player leaves game area not cleared, undo movement
	if( GameAreaManager::getSingletonPtr()->positionGameArea(player->getPosition()) != mCurrentGameArea )
	{
		if( !mGameAreaCleared )
			player->setPosition(lastPosition);
	}
}


void PhysicsManager::move(EnemyPtr enemy, const float elapsedSeconds)
{
	Vector3 direction = enemy->getDirection();
	Vector3 lastPosition = enemy->getPosition();

	enemy->translate( (direction  * enemy->getSpeed() * elapsedSeconds) );

	// Enemies only collides with scenario walls
	if( collides(lastPosition, enemy->getPosition(), mWallGeometry->getPhysicsMeshInfo(), 0.5f, 0) )
		enemy->setPosition(lastPosition);
}

void PhysicsManager::move(ProjectilePtr projectile, const float elapsedSeconds)
{
	Vector3 direction = projectile->getDirection();

	projectile->translate( (direction  * elapsedSeconds * 80)  );

}

// Load scenario physics
void PhysicsManager::addPhysicScenario(Ogre::String mesh, Ogre::String name, WyvernsAssault::GroundQueryFlags type, Ogre::Vector3 position, Ogre::Vector3 scale)
{	
	SceneNode* nodeScenario = mPhysicsNode->createChildSceneNode(name, position);
	Entity* entityScenario = mSceneManager->createEntity(name, mesh);

	entityScenario->setQueryFlags(type);
	nodeScenario->attachObject(entityScenario);
	nodeScenario->setVisible(false);
	nodeScenario->setScale(scale);

	// Initialize scenario physics geometry
	if( type == WyvernsAssault::GROUND_MASK)
	{
		mGroundGeometry = GeometryPtr(new Geometry(entityScenario));
		mGroundGeometry->initializeMeshInformation(nodeScenario->getPosition(), nodeScenario->getOrientation(), nodeScenario->getScale());
	}
	else if(type == WyvernsAssault::WALL_MASK)
	{
		mWallGeometry = GeometryPtr(new Geometry(entityScenario));
		mWallGeometry->initializeMeshInformation(nodeScenario->getPosition(), nodeScenario->getOrientation(), nodeScenario->getScale());
	}
}

// Load player physics
void PhysicsManager::addPhysicPlayer(PlayerPtr player)
{
	mPlayerMap[player->getName()] = player;
}

// Load enemy physics
void PhysicsManager::addPhysicEnemy(EnemyPtr enemy, int gameArea)
{
	mEnemyMapList[gameArea].push_back(enemy);
}

// Load item physics
void PhysicsManager::addPhysicItem(ItemPtr item, int gameArea)
{
	mItemMapList[gameArea].push_back(item);
}

// Load projectile physics
void PhysicsManager::addPhysicProjectile(ProjectilePtr projectile)
{
	mProjectileList.push_back(projectile);
}

// Load object physics
void PhysicsManager::addPhysicObject(ObjectPtr obj, int gameArea)
{
	mObjectMapList[gameArea].push_back(obj);
}

// Calculate terrain's height and modify player position
Vector3 PhysicsManager::calculateHeight(const Vector3 &point)
{
	Vector3 newPosition = Vector3(0,0,0);
	float distToColl = 0.0f;

	if( raycast(point, Vector3::NEGATIVE_UNIT_Y, newPosition, distToColl, mGroundGeometry->getPhysicsMeshInfo()) )
	{
		return newPosition;
	}

	return point;
}

// Collisions between player and scenario physic objects
bool PhysicsManager::collidesAllObjects(PlayerPtr player, const Vector3& fromPoint, const Vector3& toPoint, const float collisionRadius, const float rayHeightLevel )
{
	AxisAlignedBox player_box = player->getGeometry()->getWorldBoundingBox(player->getPosition());

	for(int i = 0; i < mObjectMapList[mCurrentGameArea].size(); i++)
	{
		ObjectPtr obj = mObjectMapList[mCurrentGameArea][i];

		AxisAlignedBox obj_box = obj->getGeometry()->getWorldBoundingBox(obj->getPosition());

		if( player_box.intersects(obj_box) )
			return true;
	}

	return false;
}

// Collisions between entity and scenario walls
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
		return false;
}

bool PhysicsManager::raycast(const Vector3 &point, const Vector3 &normal,
							 Vector3 &result, float &closest_distance, PhysicsMeshInfo objInfo)
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


bool PhysicsManager::removeEnemy(EnemyPtr enemy)
{	
	EnemyListIterator it = find(mEnemyMapList[mCurrentGameArea].begin(), mEnemyMapList[mCurrentGameArea].end(), enemy);
	
	if( it != mEnemyMapList[mCurrentGameArea].end() )
	{
		mEnemyMapList[mCurrentGameArea].erase(it);
		return true;
	}

	return false;
}


bool PhysicsManager::removeItem(ItemPtr item)
{
	ItemListIterator it = find(mItemMapList[mCurrentGameArea].begin(), mItemMapList[mCurrentGameArea].end(), item);
	
	if( it != mItemMapList[mCurrentGameArea].end() )
	{
		mItemMapList[mCurrentGameArea].erase(it);
		return true;
	}

	return false;
}

bool PhysicsManager::removeObject(ObjectPtr obj)
{
	ObjectListIterator it = find(mObjectMapList[mCurrentGameArea].begin(), mObjectMapList[mCurrentGameArea].end(), obj);
	
	if( it != mObjectMapList[mCurrentGameArea].end() )
	{
		mObjectMapList[mCurrentGameArea].erase(it);
		return true;
	}

	return false;
}

bool PhysicsManager::removeProjectile(ProjectilePtr projectile)
{
	ProjectileListIterator it = find(mProjectileList.begin(), mProjectileList.end(), projectile);
		if( it != mProjectileList.end() )
	{
		mProjectileList.erase(it);
		return true;
	}

	return false;

}

// --------------
// Event handlers
// --------------
EVENTS_BEGIN_REGISTER_HANDLERS(PhysicsManager)
	EVENTS_REGISTER_HANDLER(PhysicsManager, EnemyKilled)
	EVENTS_REGISTER_HANDLER(PhysicsManager, ItemCatch);
	EVENTS_REGISTER_HANDLER(PhysicsManager, ObjectKilled);
	EVENTS_REGISTER_HANDLER(PhysicsManager, GameAreaChanged);
	EVENTS_REGISTER_HANDLER(PhysicsManager, GameAreaCleared);
	EVENTS_REGISTER_HANDLER(PhysicsManager, ProjectileUpdate);
	EVENTS_REGISTER_HANDLER(PhysicsManager, ProjectileRemove);
EVENTS_END_REGISTER_HANDLERS()

EVENTS_BEGIN_UNREGISTER_HANDLERS(PhysicsManager)
	EVENTS_UNREGISTER_HANDLER(PhysicsManager, EnemyKilled)
	EVENTS_UNREGISTER_HANDLER(PhysicsManager, ItemCatch);
	EVENTS_UNREGISTER_HANDLER(PhysicsManager, ObjectKilled);
	EVENTS_UNREGISTER_HANDLER(PhysicsManager, GameAreaChanged);
	EVENTS_UNREGISTER_HANDLER(PhysicsManager, GameAreaCleared);
	EVENTS_UNREGISTER_HANDLER(PhysicsManager, ProjectileUpdate);
	EVENTS_UNREGISTER_HANDLER(PhysicsManager, ProjectileRemove);
EVENTS_END_UNREGISTER_HANDLERS()

EVENTS_DEFINE_HANDLER(PhysicsManager, EnemyKilled)
{
	Debug::Out("PhysicsManager : handleEnemyKilledEvent");

	EnemyPtr enemy = evt->getEnemy();
	PlayerPtr player = evt->getPlayer();

	// The player has just hit the enemy
   	removeEnemy(enemy);
}

EVENTS_DEFINE_HANDLER(PhysicsManager, ItemCatch)
{
	Debug::Out("PhysicsManager : handleItemCatchEvent");

	ItemPtr item = evt->getItem();

	// The player has just cacth the item
   	removeItem(item);
}

EVENTS_DEFINE_HANDLER(PhysicsManager, ObjectKilled)
{
	Debug::Out("PhysicsManager : handleObjectKilledEvent");

	ObjectPtr object = evt->getObject();

	// The player has just cacth the item
   	removeObject(object);
}

EVENTS_DEFINE_HANDLER(PhysicsManager, GameAreaChanged)
{
	Debug::Out("PhysicsManager : handleGameAreaChangedEvent");

	// New game area
	mCurrentGameArea = evt->getActualArea();
	mGameAreaCleared = false;
}

EVENTS_DEFINE_HANDLER(PhysicsManager, GameAreaCleared)
{
	Debug::Out("PhysicsManager : handleGameAreaClearedEvent");

	// Actual game area is cleared
	if( mCurrentGameArea == evt->getGameArea() && !mGameAreaCleared)
		mGameAreaCleared = true;
}

EVENTS_DEFINE_HANDLER(PhysicsManager, ProjectileUpdate)
{
	Debug::Out("PhysicsManager : handleProjectileUpdateEvent");

	ProjectilePtr projectile = evt->getProjectile();
   	addPhysicProjectile(projectile);
	
}

EVENTS_DEFINE_HANDLER(PhysicsManager, ProjectileRemove)
{
	Debug::Out("PhysicsManager : handleProjectileRemoveEvent");

	ProjectilePtr projectile = evt->getProjectile();
   	removeProjectile(projectile);
	
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
	//mPhysicsManager->getHOT

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

	SceneManager* sceneManager = PhysicsManager::getSingleton().getSceneManager();

	SceneNode* node1 = sceneManager->getEntity(name1)->getParentSceneNode();
	SceneNode* node2 = sceneManager->getEntity(name2)->getParentSceneNode();

	double range = node1->getPosition().distance(node2->getPosition());

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

	//double range = node1->getPosition().distance(node2->getPosition());

	Ogre::String playerId = "Player1"; // TODO Retrieve the REAL nearest player!

	/* push the id of the closest player */
	lua_pushstring(L, playerId.c_str()); // 

	/* return the number of results */
	return 1;
}
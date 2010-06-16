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
	mPhysicsNode = mSceneManager->getRootSceneNode()->createChildSceneNode(PHYSICS_NODE_NAME);

	// State control variables
	mPlayerSpecialState = false;
	mPlayerAttackLast = 0;

	// Initialize RaySceneQuery
	mRaySceneQuery = mSceneManager->createRayQuery(Ogre::Ray());
    if (NULL == mRaySceneQuery)
    {
      return (false);
    }
    mRaySceneQuery->setSortByDistance(true);

	return (true);
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

void PhysicsManager::update(const float elapsedSeconds){
	
	// Update players ray
	for(PlayerMapIterator it_player = mPlayerMap.begin(); it_player != mPlayerMap.end(); ++it_player)
	{
		PlayerPtr player = it_player->second;
		Vector3 position = player->getPosition();

		// Check basic ground collision
		Vector3 hotPosition = calculateY(position,BASIC_GROUND_MASK);
	
		hotPosition.y += REDWYVERN_RAY_HEIGHT; // HACK : this should not be hardcoded!
			
		player->setPosition(hotPosition);
	}

	// Call bounding box collision
	checkForCollisions();

	return;
}

// Bounding-Box collision
void PhysicsManager::checkForCollisions()
{
	for(PlayerMapIterator it_player = mPlayerMap.begin(); it_player != mPlayerMap.end(); ++it_player)
	{
		PlayerPtr player = it_player->second;
		AxisAlignedBox player_firebox = player->getFireBox();

		// Only check if player is using special (fire) and last check was not using it
		if ( player->isSpecial())
		{
			for(EnemyMapIterator it_enemy = mEnemyMap.begin(); it_enemy != mEnemyMap.end(); ++it_enemy)
			{
				EnemyPtr enemy = it_enemy->second;
				AxisAlignedBox enemy_box = enemy->getWorldBoundingBox();
				
				if(player_firebox.intersects(enemy_box))
				{
					EnemyHitEventPtr enemyHitEventPtr = EnemyHitEventPtr(new EnemyHitEvent(enemy, player));
					enemyHitEventPtr->setDamage(player->getSpecialHitDamage());
					raiseEvent(enemyHitEventPtr);
				}
			}
		}

		// Only check if player is attacking and has changed state
		if( mPlayerAttackLast != player->wichAttack() && player->isAttacking() )
		{

			AxisAlignedBox player_box = player->getWorldBoundingBox();

			for(EnemyMapIterator it_enemy = mEnemyMap.begin(); it_enemy != mEnemyMap.end(); ++it_enemy)
			{
				EnemyPtr enemy = it_enemy->second;
				AxisAlignedBox enemy_box = enemy->getWorldBoundingBox();

				if(player_box.intersects(enemy_box))
				{
					// More damage if player is doing combo!
					if(player->wichAttack() == 3)
					{
						EnemyHitEventPtr enemyHitEventPtr = EnemyHitEventPtr(new EnemyHitEvent(enemy, player));
						enemyHitEventPtr->setDamage(player->getHitDamage() * 3.0f);
						raiseEvent(enemyHitEventPtr);
					}
					else
					{
						EnemyHitEventPtr enemyHitEventPtr = EnemyHitEventPtr(new EnemyHitEvent(enemy, player));
						enemyHitEventPtr->setDamage(player->getHitDamage());
						raiseEvent(enemyHitEventPtr);
					}
				}
			}
		}
		// Save last attack
		mPlayerAttackLast = player->wichAttack();
	}

}

void PhysicsManager::move(PlayerPtr player, Vector3 direction, const float elapsedSeconds, bool fastMode)
{
	if(direction != Vector3::ZERO)
	{
		Quaternion q1 = player->getOrientation();
		// Get current direction where player is facing
		Vector3 currentDirection = q1 * Vector3::UNIT_Z;
		Quaternion q2 = currentDirection.getRotationTo(direction);
		player->setOrientation(q1*q2);
	}

	Vector3 old_position = player->getPosition();
	
	if(fastMode)
		player->setPosition((direction*REDWYVERN_FAST_VELOCITY*elapsedSeconds) + old_position);
	else
		player->setPosition((direction*REDWYVERN_SLOW_VELOCITY*elapsedSeconds) + old_position);

	if(collidesWithBorders(old_position,player->getPosition(),0.5f,0,BORDER_GROUND_MASK))
		player->setPosition(old_position);
}


void move(EnemyPtr enemy, int rotate, int thrust)
{
	/* TODO
	*/
}

/* Load a mesh as ground type
*/
void PhysicsManager::addPhysicGround(Ogre::String mesh, Ogre::String name, WyvernsAssault::GroundQueryFlags type, Ogre::Vector3 position, Ogre::Vector3 scale)
{
	
	SceneNode* nodeGround = mPhysicsNode->createChildSceneNode(name,position);
	Entity* entityGround = mSceneManager->createEntity(name,mesh);
	entityGround->setQueryFlags(type);
	nodeGround->attachObject(entityGround);
	nodeGround->setVisible(false);
	nodeGround->setScale(scale);

}

/* Load player physics
*/
void PhysicsManager::addPhysicPlayer(PlayerPtr player)
{

	mPlayerMap[player->getName()] = player;

}

/* Load enemy physics
*/
void PhysicsManager::addPhysicEnemy(EnemyPtr enemy)
{
	mEnemyMap[enemy->getName()] = enemy;
}

/* Calculate heigth of terrain and translate node to adjust them
*/
Vector3 PhysicsManager::calculateY(const Vector3 &point, const Ogre::uint32 queryMask)
{
	Vector3 yPosition(0,0,0);
	float distToColl = 0.0f;

	if(raycast(point,Vector3::NEGATIVE_UNIT_Y,yPosition, distToColl ,queryMask)){
		return Ogre::Vector3(point.x,yPosition.y,point.z);
	}

	return point;
}

/* Control border ground collision
*/
bool PhysicsManager::collidesWithBorders(const Vector3& fromPoint, const Vector3& toPoint, const float collisionRadius, const float rayHeightLevel, const uint32 queryMask)
{
	Vector3 fromPointAdj(fromPoint.x, fromPoint.y + rayHeightLevel, fromPoint.z);
	Vector3 toPointAdj(toPoint.x, toPoint.y + rayHeightLevel, toPoint.z);	
	Vector3 normal = toPointAdj - fromPointAdj;
	float distToDest = normal.normalise();

	float distToColl = 0.0f;
	Vector3 myResult(0, 0, 0);

	if (raycast(fromPointAdj, normal, myResult, distToColl ,queryMask))
	{
		distToColl -= collisionRadius; 
		return (distToColl <= distToDest);
	}
	else
	{
		return false;
	}
}

/* Raycast from a point in to the scene. returns success or failure.
 * on success the point is returned in the result.
 */
bool PhysicsManager::raycast(const Vector3 &point, const Vector3 &normal, 
							 Vector3 &result,float &closest_distance, const Ogre::uint32 queryMask)
{


    // Create the ray to test
    static Ogre::Ray ray;
	ray.setOrigin(point);
	ray.setDirection(normal);

    if (mRaySceneQuery != NULL)
    {
        mRaySceneQuery->setRay(ray);
		mRaySceneQuery->setSortByDistance(true);
		mRaySceneQuery->setQueryMask(queryMask);
        // Execute the query, returns a vector of hits
        if (mRaySceneQuery->execute().size() <= 0)
        {
            // Raycast did not hit an objects bounding box
            return (false);
        }
    }
    else
    {
		//raycast failed
        return (false);
    }   

	closest_distance = -1.0f;
    Ogre::Vector3 closest_result;
	ulong target = NULL;
    Ogre::RaySceneQueryResult &query_result = mRaySceneQuery->getLastResults();
	for (size_t qr_idx = 0; qr_idx < query_result.size(); qr_idx++)
	{
		
		// Only check this result if its a hit against an physic entity
		if (query_result[qr_idx].movable->getQueryFlags() == queryMask) 
		{

			 // Get the entity to check
			Ogre::Entity *pentity = static_cast<Ogre::Entity*>(query_result[qr_idx].movable); 			
						      
            size_t vertex_count;
            size_t index_count;
            Ogre::Vector3 *vertices;
            unsigned long *indices;

            // get the mesh information
			GetMeshInformation(pentity->getMesh(), vertex_count, vertices, index_count, indices,             
                              pentity->getParentNode()->getPosition(),
                              pentity->getParentNode()->getOrientation(),
                              pentity->getParentNode()->getScale());

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
                    if ((closest_distance < 0.0f) ||
                        (hit.second < closest_distance))
                    {
                        // this is the closest so far, save it off
                        closest_distance = hit.second;
                        new_closest_found = true;
                    }
                }
            }

            delete[] vertices;
            delete[] indices;

            // if we found a new closest raycast for this object, update the
            // closest_result before moving on to the next object.
            if (new_closest_found)
            {
				target = (ulong)pentity;
                closest_result = ray.getPoint(closest_distance);               
            }
		}
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

/* Get the mesh information for the given mesh.
*/
void PhysicsManager::GetMeshInformation(const Ogre::MeshPtr mesh,
                                size_t &vertex_count,
                                Ogre::Vector3* &vertices,
                                size_t &index_count,
                                unsigned long* &indices,
                                const Ogre::Vector3 &position,
                                const Ogre::Quaternion &orient,
                                const Ogre::Vector3 &scale)
{
    bool added_shared = false;
    size_t current_offset = 0;
    size_t shared_offset = 0;
    size_t next_offset = 0;
    size_t index_offset = 0;

    vertex_count = index_count = 0;

    // Calculate how many vertices and indices we're going to need
    for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh( i );

        // We only need to add the shared vertices once
        if(submesh->useSharedVertices)
        {
            if( !added_shared )
            {
                vertex_count += mesh->sharedVertexData->vertexCount;
                added_shared = true;
            }
        }
        else
        {
            vertex_count += submesh->vertexData->vertexCount;
        }

        // Add the indices
        index_count += submesh->indexData->indexCount;
    }


    // Allocate space for the vertices and indices
    vertices = new Ogre::Vector3[vertex_count];
    indices = new unsigned long[index_count];

    added_shared = false;

    // Run through the submeshes again, adding the data into the arrays
    for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh(i);

        Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;

        if((!submesh->useSharedVertices)||(submesh->useSharedVertices && !added_shared))
        {
            if(submesh->useSharedVertices)
            {
                added_shared = true;
                shared_offset = current_offset;
            }

            const Ogre::VertexElement* posElem =
                vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

            Ogre::HardwareVertexBufferSharedPtr vbuf =
                vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

            unsigned char* vertex =
                static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

            float* pReal;

            for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
            {
                posElem->baseVertexPointerToElement(vertex, &pReal);

                Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);

                vertices[current_offset + j] = (orient * (pt * scale)) + position;
            }

            vbuf->unlock();
            next_offset += vertex_data->vertexCount;
        }


        Ogre::IndexData* index_data = submesh->indexData;
        size_t numTris = index_data->indexCount / 3;
        Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

        bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

        unsigned long*  pLong = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
        unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);


        size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;

        if ( use32bitindexes )
        {
            for ( size_t k = 0; k < numTris*3; ++k)
            {
                indices[index_offset++] = pLong[k] + static_cast<unsigned long>(offset);
            }
        }
        else
        {
            for ( size_t k = 0; k < numTris*3; ++k)
            {
                indices[index_offset++] = static_cast<unsigned long>(pShort[k]) +
                    static_cast<unsigned long>(offset);
            }
        }

        ibuf->unlock();
        current_offset = next_offset;
    }
}

void PhysicsManager::removeEnemy(EnemyPtr enemy)
{
	//
	// TODO : maybe we don't really need a list, and we can just use a map...
	//
	mEnemyMap.erase(enemy->getName());
}

// --------------
// Event handlers
// --------------
void PhysicsManager::registerHandlers()
{
	boost::shared_ptr<PhysicsManager> this_ = shared_from_this();

	registerHandler(EventHandlerPtr(new EventHandler<PhysicsManager,EnemyKillEvent>(this_,&PhysicsManager::handleEnemyKillEvent)),EventTypes::EnemyKill);
}

void PhysicsManager::unregisterHandlers()
{
	
}

void PhysicsManager::handleEnemyKillEvent(EnemyKillEventPtr evt)
 {
	EnemyPtr enemy = evt->getEnemy();
	PlayerPtr player = evt->getPlayer();

	// The player has just hit the enemy
   	removeEnemy(enemy);
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
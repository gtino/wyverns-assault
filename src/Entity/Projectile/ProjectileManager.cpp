#include "..\..\..\include\Entity\Projectile\ProjectileManager.h"

using namespace WyvernsAssault;

// BEGIN SINGLETON
template<> ProjectileManager* Ogre::Singleton<ProjectileManager>::ms_Singleton = 0;
ProjectileManager* ProjectileManager::getSingletonPtr(void)
{
    return ms_Singleton;
}
ProjectileManager& ProjectileManager::getSingleton(void)
{  
    assert( ms_Singleton );  return ( *ms_Singleton );  
}
// END SINGLETON

ProjectileManager::ProjectileManager(Ogre::SceneManager* sceneManager) 
: mId(0)
, mIsDebugEnabled(false)
{
	mSceneManager = sceneManager;
}

ProjectileManager::~ProjectileManager()
{
	finalize();
}

bool ProjectileManager::initialize()
{
	return true;
}

void ProjectileManager::finalize()
{
	mProjectileList.clear();
}

// Create single projectile
ProjectilePtr ProjectileManager::createProjectile(Ogre::String name, Enemy::EnemyTypes type, Ogre::SceneNode* sceneNode, Ogre::Vector3 init, Ogre::Vector3 finish)
{
	ProjectilePtr projectile = ProjectilePtr(new Projectile(name, init, finish));

	// Define Entity
	if( type == Enemy::EnemyTypes::Archer )
	{
		Entity* entity = mSceneManager->createEntity(name, "arrow.mesh");
		entity->setVisible(true);
		sceneNode->attachObject(entity);	
		projectile->initializeEntity(entity, sceneNode, mSceneManager);
		projectile->setProjectileSpeed(120);
	}
	else
	{
		Entity* entity = mSceneManager->createEntity(name, "cube.mesh");
		entity->setVisible(false);	
		sceneNode->attachObject(entity);
		projectile->initializeEntity(entity, sceneNode, mSceneManager);
	}	

	// Store Projectile
	mProjectileList.push_back(projectile);

	// Launch event for add physics projectile
	ProjectileUpdateEventPtr evtUpdate = ProjectileUpdateEventPtr(new ProjectileUpdateEvent(projectile, type));
	EVENTS_FIRE(evtUpdate);

	return projectile;
}

bool ProjectileManager::removeProjectile(Ogre::String name)
{
	ProjectilePtr ProjectileToErase = getProjectile(name);
	
	ProjectileListIterator it = find(mProjectileList.begin(), mProjectileList.end(), ProjectileToErase);

	if( it != mProjectileList.end() )
	{
		mProjectileList.erase(it);
		return true;
	}

	return false;
}

void ProjectileManager::update(const float elapsedSeconds)
{
	for(int i = 0; i < mProjectileList.size(); i++)
	{
		ProjectilePtr projectile = mProjectileList[i];
		projectile->setProjectileTimer(projectile->getProjectileTimer() + elapsedSeconds);		

		// Physic debugg control
		projectile->setDebugEnabled(mIsDebugEnabled);

		if( (projectile->getProjectileTimer() >= projectile->getAliveTime()) && projectile->isLive() )
		{
			//Projectile remove
			ProjectileRemoveEventPtr evtRemove = ProjectileRemoveEventPtr(new ProjectileRemoveEvent(projectile));
			EVENTS_FIRE(evtRemove);
		}
	}
}

Ogre::String ProjectileManager::createUniqueId()
{
	Ogre::StringStream countStrStr;
	countStrStr << mId;

	Ogre::String uniqueId = "Projectile_" + countStrStr.str();

	mId++;

	return uniqueId;
}

void ProjectileManager::setDebugEnabled(bool isDebugEnabled)
{
	if(mIsDebugEnabled != isDebugEnabled)
	{
		mIsDebugEnabled = isDebugEnabled;

		for(int i = 0; i < mProjectileList.size() ; i++)
		{
			ProjectilePtr Projectile =  mProjectileList[i];
			Projectile->setDebugEnabled(mIsDebugEnabled);
		}
	}
}

ProjectilePtr ProjectileManager::getProjectile(Ogre::String name)
{
	// Search enemy in current game area enemies list
	for( int i = 0; i < mProjectileList.size(); i++ )
	{
		ProjectilePtr projectile = mProjectileList[i];

		if( projectile->getName() == name )
			return mProjectileList[i];
	}
}

void ProjectileManager::setDirectionToTarget(Vector3 begin, Vector3 end, SceneNode* node)
{
	Vector3 direction = end - begin;
	Vector3 src = node->getOrientation() * Vector3::NEGATIVE_UNIT_X;
	src.y = 0;
	direction.y = 0;
	direction.normalise();
	src.normalise();

	if ((1.0f + src.dotProduct(direction)) <= 0.0001f)            // Work around 180 degree quaternion rotation quirk                         
	{
		node->yaw(Degree(180));
	}
	else
	{
		Quaternion rotation = src.getRotationTo(direction);
		node->rotate(rotation);
	}
}

// --------------
// Event handlers
// --------------
EVENTS_BEGIN_REGISTER_HANDLERS(ProjectileManager)
	EVENTS_REGISTER_HANDLER(ProjectileManager, ProjectileFire);
	EVENTS_REGISTER_HANDLER(ProjectileManager, ProjectileRemove);
EVENTS_END_REGISTER_HANDLERS()

EVENTS_BEGIN_UNREGISTER_HANDLERS(ProjectileManager)
	EVENTS_UNREGISTER_HANDLER(ProjectileManager, ProjectileFire);
	EVENTS_UNREGISTER_HANDLER(ProjectileManager, ProjectileRemove);
EVENTS_END_UNREGISTER_HANDLERS()

EVENTS_DEFINE_HANDLER(ProjectileManager, ProjectileFire)
{
	Debug::Out("PhysicsManager : handleProjectileFireEvent");

	//Points
	EnemyPtr enemy = evt->getEnemy();
	Vector3 init = enemy->getPosition();
	Vector3 finish = init;
	try
	{
		finish = enemy->getTarget()->getPosition();
	}
	catch(...)
	{
		finish = init;
	}

	finish.y = init.y;	

	//SceneNode
	SceneNode* projectileNode = enemy->_getSceneManager()->getRootSceneNode()->createChildSceneNode();	
	projectileNode->setPosition(init);
	
	// Rotate node to face direction
	this->setDirectionToTarget(init, finish, projectileNode);

	ProjectilePtr projectile = createProjectile(createUniqueId(), enemy->getEnemyType(), projectileNode, init, finish);

	projectile->setProjectileDamage(enemy->getHitDamage());
}

EVENTS_DEFINE_HANDLER(ProjectileManager, ProjectileRemove)
{
	Debug::Out("EnemyManager : handleProjectileRemoveEvent");

	ProjectilePtr projectile = evt->getProjectile();

	projectile->death();
	removeProjectile(projectile->getName());
}

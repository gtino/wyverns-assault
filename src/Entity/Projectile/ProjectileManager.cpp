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
ProjectilePtr ProjectileManager::createProjectile(Ogre::String name, Ogre::SceneNode* sceneNode, Ogre::Vector3 init, Ogre::Vector3 finish)
{

	ProjectilePtr projectile = ProjectilePtr(new Projectile(name,init,finish));

	//Define Entity
	Entity* entity = mSceneManager->createEntity(name,"cube.mesh");
	entity->setVisible(false);
	sceneNode->scale(Ogre::Vector3(0.1,0.1,0.1));
	sceneNode->attachObject(entity);
	projectile->initializeEntity(entity, sceneNode, mSceneManager);

	ParticleManager::getSingletonPtr()->magicBolt(sceneNode);

	// Store Projectile
	mProjectileList.push_back(projectile);

	// Launch event for add physics projectile
	ProjectileUpdateEventPtr evtUpdate = ProjectileUpdateEventPtr(new ProjectileUpdateEvent(projectile));
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

void ProjectileManager::update(const float elapsedSeconds){

	for(int i = 0; i < mProjectileList.size(); i++)
	{

		ProjectilePtr projectile = mProjectileList[i];
		projectile->setProjectileTimer(projectile->getProjectileTimer() + elapsedSeconds);

		// Physic debugg control
		projectile->setDebugEnabled(mIsDebugEnabled);

		if(projectile->getProjectileTimer() >= PROJECTILE_LIVE_TIME && projectile->isLive())
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
	Vector3 finish = enemy->getTarget()->getPosition();
	Vector3 init = enemy->getPosition();

	//SceneNode
	SceneNode* projectileNode = enemy->_getSceneManager()->getRootSceneNode()->createChildSceneNode();
	projectileNode->setPosition(init);

	ProjectilePtr projectile = createProjectile(createUniqueId(), projectileNode, init, finish);

	projectile->setProjectileDamage(enemy->getHitDamage());

}

EVENTS_DEFINE_HANDLER(ProjectileManager, ProjectileRemove)
{
	Debug::Out("EnemyManager : handleProjectileRemoveEvent");

	ProjectilePtr p = evt->getProjectile();
	removeProjectile(p->getName());

}

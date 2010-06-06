#include "..\include\Scenario\ScenarioManager.h"

using namespace WyvernsAssault;

// BEGIN SINGLETON
template<> ScenarioManager* Ogre::Singleton<ScenarioManager>::ms_Singleton = 0;
ScenarioManager* ScenarioManager::getSingletonPtr(void)
{
    return ms_Singleton;
}
ScenarioManager& ScenarioManager::getSingleton(void)
{  
    assert( ms_Singleton );  return ( *ms_Singleton );  
}
// END SINGLETON

ScenarioManager::ScenarioManager(SceneManager* sceneManager)
: mInitialized(false)
, mSceneManager(0)
, mScenarioNode(0)
, mDotSceneLoader(0)
{
	mSceneManager = sceneManager;
}

ScenarioManager::~ScenarioManager()
{
	if(mInitialized)
	{
		if(mDotSceneLoader)
		{
			delete mDotSceneLoader;
			mDotSceneLoader = NULL;
		}

		if(mScenarioNode)
		{
			// FIXME : The whole scene will be destroyed in the clearScene() call, here I do not know what to do...
			mScenarioNode = NULL;
		}

		if(mSceneManager)
		{
			mSceneManager = NULL;
		}

		mInitialized = false;
	}
}

void ScenarioManager::initialize(CameraManagerPtr cameraManager, 
								 LightsManagerPtr lightsManager, 
								 EnemyManagerPtr enemyManager, 
								 PhysicsManagerPtr physicsManager, 
								 ItemManagerPtr itemManager, 
								 ParticleManagerPtr particleManager)
{
	//
	// Copy to internal smart pointers
	//
	mCameraManager = cameraManager;
	mLightsManager = lightsManager;
	mEnemyManager = enemyManager;
	mPhysicsManager = physicsManager;
	mItemManager = itemManager;
	mParticleManager = particleManager;

	//
	// Create the base scenario node...
	//
	mScenarioNode = mSceneManager->getRootSceneNode()->createChildSceneNode("RootSceneNode", Vector3::ZERO);

	//
	// And instantiate the scene loader class
	//
	mDotSceneLoader = new DotSceneLoader();

	// Now it is initialized!
	mInitialized = true;
}

void ScenarioManager::load(Ogre::String file)
{
	assert(mInitialized);

	//
	// Load the .scene file
	//
	mDotSceneLoader->parseDotScene(file,"General", mSceneManager, mCameraManager, mLightsManager, mEnemyManager, mPhysicsManager, mItemManager, mParticleManager, mScenarioNode);
}
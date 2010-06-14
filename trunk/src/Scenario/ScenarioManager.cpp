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
{
	mSceneManager = sceneManager;
}

ScenarioManager::~ScenarioManager()
{
	if(mInitialized)
	{
		//
		// Destroy all the objects (destructors will take care of deleting them)
		//
		mObjectList.clear();
		mObjectMap.clear();

		Utils::Destroy(mSceneManager, SCENARIO_NODE_NAME);
		mScenarioNode = NULL;

		mInitialized = false;
	}
}

void ScenarioManager::initialize()
{
	//
	// Create the base scenario node...
	//
	mScenarioNode = mSceneManager->getRootSceneNode()->createChildSceneNode(SCENARIO_NODE_NAME, Vector3::ZERO);

	// Now it is initialized!
	mInitialized = true;
}

void ScenarioManager::load(Ogre::String file)
{
	assert(mInitialized);
}

ObjectPtr ScenarioManager::createObject(WyvernsAssault::ObjectTypes type, Ogre::String name, Ogre::String meshFile)
{
	Ogre::Entity* pEntity = mSceneManager->createEntity(name, meshFile);

	Ogre::SceneNode* pSceneNode = mSceneManager->getRootSceneNode()->createChildSceneNode("Object_"+name+"_Node");

	return createObject(type, name, pEntity, pSceneNode);
}

ObjectPtr ScenarioManager::createObject(WyvernsAssault::ObjectTypes type, Ogre::String name, Ogre::Entity* entity, Ogre::SceneNode* sceneNode)
{
	ObjectPtr object = ObjectPtr(new Object(name, type));

	object->initializeEntity(entity, sceneNode, mSceneManager);

	sceneNode->attachObject(entity);

	mObjectList.push_back(object);
	mObjectMap[name] = object;

	return object;
}

int ScenarioManager::getCount()
{
	return mObjectList.size();
}

ObjectPtr ScenarioManager::getObject(int index)
{
	return mObjectList[index];
}


ObjectPtr ScenarioManager::getObject(Ogre::String name)
{
	return mObjectMap[name];
}

bool ScenarioManager::removeObject(Ogre::String name)
{
	//
	// TODO : maybe we don't really need a list, and we can just use a map...
	//
	ObjectPtr objectToErase = mObjectMap[name];

	mObjectMap.erase(name);
	
	ObjectListIterator it = find(mObjectList.begin(), mObjectList.end(), objectToErase);
	
	if( it != mObjectList.end() )
	{		
		mObjectList.erase(it);
	}

	return true;
}
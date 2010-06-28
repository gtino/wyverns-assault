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
,mIsDebugEnabled(false)
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

	// Die mesh and animation - TEST
	if(type == WyvernsAssault::ObjectTypes::DynamicObject && name == "Entity#383")
	{
		Ogre::Entity* objDieMesh = NULL;
		Ogre::AnimationState* objDieAnimation = NULL;
		objDieMesh = mSceneManager->createEntity(name + "Die", "BigRock0Die.mesh");
		objDieMesh->setMaterialName("BigRock");
		sceneNode->attachObject(objDieMesh);			
		object->setDieMesh(objDieMesh);		
		objDieAnimation = objDieMesh->getAnimationState("Die");
		object->setDieAnimation(objDieAnimation);
	}

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

void ScenarioManager::setDebugEnabled(bool isDebugEnabled)
{
	if(mIsDebugEnabled != isDebugEnabled)
	{
		mIsDebugEnabled = isDebugEnabled;

		for(int i = 0; i < mObjectList.size() ; i++)
		{
			ObjectPtr obj =  mObjectList[i];
			obj->setDebugEnabled(mIsDebugEnabled);
		}
	}
}

void ScenarioManager::update(const float elapsedSeconds)
{
	for(int i = 0; i < mObjectList.size() ; i++)
	{
		ObjectPtr obj =  mObjectList[i];
		obj->updateEntity(elapsedSeconds); // this updates animations too!
		
	}
}

// --------------
// Event handlers
// --------------
EVENTS_BEGIN_REGISTER_HANDLERS(ScenarioManager)
	EVENTS_REGISTER_HANDLER(ScenarioManager, Collision)
	EVENTS_REGISTER_HANDLER(ScenarioManager, ObjectHit)
	EVENTS_REGISTER_HANDLER(ScenarioManager, ObjectKilled)
	EVENTS_REGISTER_HANDLER(ScenarioManager, ObjectRemove);
EVENTS_END_REGISTER_HANDLERS()

EVENTS_BEGIN_UNREGISTER_HANDLERS(ScenarioManager)
	EVENTS_UNREGISTER_HANDLER(ScenarioManager, Collision)
	EVENTS_UNREGISTER_HANDLER(ScenarioManager, ObjectHit)
	EVENTS_UNREGISTER_HANDLER(ScenarioManager, ObjectKilled)
	EVENTS_UNREGISTER_HANDLER(ScenarioManager, ObjectRemove);
EVENTS_END_UNREGISTER_HANDLERS()

EVENTS_DEFINE_HANDLER(ScenarioManager, Collision)
{
// TODO
}

EVENTS_DEFINE_HANDLER(ScenarioManager, ObjectHit)
 {
	ObjectPtr obj = evt->getObject();
	PlayerPtr player = evt->getPlayer();

	obj->hit(evt->getDamage());

	if(obj->isDying())
	{
		ObjectKilledEventPtr oKill = ObjectKilledEventPtr(new ObjectKilledEvent(obj, player));
 		EVENTS_FIRE(oKill);
	}
}

EVENTS_DEFINE_HANDLER(ScenarioManager, ObjectKilled)
 {
	ObjectPtr obj = evt->getObject();
	PlayerPtr player = evt->getPlayer();

	if( obj->hasDieAnimation() )
	{
		obj->dieSwitch();
	}

	ObjectRemoveEventPtr oRemove = ObjectRemoveEventPtr(new ObjectRemoveEvent(obj));
 	EVENTS_FIRE_AFTER(oRemove, 4.0f);

}

EVENTS_DEFINE_HANDLER(ScenarioManager, ObjectRemove)
{
	ObjectPtr o = evt->getObject();
	removeObject(o->getName());
}

// --------------------------------
// Lua Scenario Lib
// --------------------------------
LUA_BEGIN_BINDING(ScenarioManager, scenariolib)
LUA_BIND(ScenarioManager, createObject)
LUA_BIND(ScenarioManager, removeObject)
LUA_END_BINDING()

//
// Load lua scripts that will be used by this manager
//
LUA_BEGIN_LOAD_SCRIPTS(ScenarioManager)
// 
// TODO : Load scripts if needed
//
LUA_END_LOAD_SCRIPTS()

LUA_DEFINE_FUNCTION(ScenarioManager, createObject)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 1;
}

LUA_DEFINE_FUNCTION(ScenarioManager, removeObject)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 1;
}
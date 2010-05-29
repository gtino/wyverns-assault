#include "..\..\..\include\Entity\Animal\AnimalManager.h"

using namespace WyvernsAssault;

// BEGIN SINGLETON
template<> AnimalManager* Ogre::Singleton<AnimalManager>::ms_Singleton = 0;
AnimalManager* AnimalManager::getSingletonPtr(void)
{
    return ms_Singleton;
}
AnimalManager& AnimalManager::getSingleton(void)
{  
    assert( ms_Singleton );  return ( *ms_Singleton );  
}
// END SINGLETON

AnimalManager::AnimalManager(Ogre::SceneManager* sceneManager) :
mCount(0),
mId(0)
{
	mSceneManager = sceneManager;
}

AnimalManager::~AnimalManager()
{
	finalize();
}

void AnimalManager::initialize()
{
}

void AnimalManager::finalize()
{
	mAnimalList.clear();
	mAnimalMap.clear();
}

AnimalPtr AnimalManager::createAnimal(AnimalTypes type)
{
	Ogre::String mesh;

	switch(type)
	{
	case Naked:
		mesh = Ogre::String("naked.mesh");
		break;
	case Wizard:
		mesh = Ogre::String("wizard.mesh");
		break;
	default:
		mesh = Ogre::String("naked.mesh");
		break;
	}

	Ogre::String name = createUniqueId();

	return createAnimal(type, name, mesh);
}

AnimalPtr AnimalManager::createAnimal(AnimalTypes type, Ogre::String name, Ogre::String mesh)
{
	// Animal name == Mesh Name!
	Ogre::Entity* animalMesh = mSceneManager->createEntity(name, mesh);
	Ogre::SceneNode* animalSceneNode = mSceneManager->getRootSceneNode()->createChildSceneNode();

	animalSceneNode->attachObject(animalMesh);
	
	// Balloon billboard
	Ogre::BillboardSet* mBalloonSet = mSceneManager->createBillboardSet(name + "_BillboardSet");
	animalSceneNode->attachObject(mBalloonSet);

	AnimalPtr animal = AnimalPtr(new Animal(type));
	animal->initializeEntity(animalMesh, animalSceneNode);
	animal->setBillboardSet(mBalloonSet);

	mAnimalList.push_back(animal);
	mAnimalMap[name] = animal;

	mCount++;

	return animal;
}

Ogre::String AnimalManager::createUniqueId()
{
	Ogre::StringStream countStrStr;
	countStrStr << mId;

	Ogre::String uniqueId = "Animal_" + countStrStr.str();

	mId++;

	return uniqueId;
}

int AnimalManager::getCount()
{
	return mAnimalList.size();
}

AnimalPtr AnimalManager::getAnimal(int index)
{
	return mAnimalList[index];
}


AnimalPtr AnimalManager::getAnimal(Ogre::String name)
{
	return mAnimalMap[name];
}

bool AnimalManager::removeAnimal(Ogre::String name)
{
	//
	// TODO : maybe we don't really need a list, and we can just use a map...
	//
	AnimalPtr animalToErase = mAnimalMap[name];

	mAnimalMap.erase(name);
	
	AnimalListIterator it = find(mAnimalList.begin(), mAnimalList.end(), animalToErase);
	
	if( it != mAnimalList.end() )
	{
		mAnimalList.erase(it);
		mSceneManager->destroyEntity(name);//getRootSceneNode()->removeChild(name);
	}

	return true;
}

/* 
* TODO
*/
void AnimalManager::update(const float elapsedSeconds)
{
	for(int i = 0; i < mAnimalList.size() ; i++)
	{
		AnimalPtr animal =  mAnimalList[i];

		animal->updateLogic(L,elapsedSeconds);
		//Animal->updatePhysics(elapsedSeconds);
		animal->updateEntity(elapsedSeconds);
	}
}


// --------------
// Event handlers
// --------------
void AnimalManager::registerHandlers()
{
	boost::shared_ptr<AnimalManager> this_ = shared_from_this();

	//registerHandler(EventHandlerPtr(new EventHandler<AnimalManager,CollisionEvent>(this_,&AnimalManager::handleCollisionEvent)),EventTypes::Collision);
	//registerHandler(EventHandlerPtr(new EventHandler<AnimalManager,AnimalHitEvent>(this_,&AnimalManager::handleAnimalHitEvent)),EventTypes::AnimalHit);
}

void AnimalManager::unregisterHandlers()
{

}

/*
void AnimalManager::handleCollisionEvent(CollisionEventPtr evt)
{
// TODO
}

void AnimalManager::handleAnimalHitEvent(AnimalHitEventPtr evt)
{
	AnimalPtr animal = evt->getAnimal();
	PlayerPtr player = evt->getPlayer();

	// The player has just hit the Animal
	//removeAnimal(animal->getName());
}
*/

// --------------------------------
// Lua Animal Lib
// --------------------------------
LUA_BEGIN_BINDING(AnimalManager::animallib)
LUA_BIND("create", AnimalManager::createAnimal)
LUA_BIND("getCount", AnimalManager::getAnimalCount)
LUA_BIND("getName",AnimalManager::getAnimalName)
LUA_BIND("getPosition", AnimalManager::getAnimalPosition)
LUA_BIND("setPosition", AnimalManager::setAnimalPosition)
LUA_BIND("setState", AnimalManager::setAnimalState)
LUA_BIND("setTarget", AnimalManager::setAnimalTarget)
LUA_BIND("getStateTimeout", AnimalManager::getAnimalStateTimeout)
LUA_BIND("isHurt", AnimalManager::isAnimalHurt)
LUA_BIND("remove", AnimalManager::removeAnimal)
LUA_END_BINDING()

int AnimalManager::createAnimal(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 1, the Animal type

	int type = luaL_checkint(L, 1);

	AnimalPtr animal = AnimalManager::getSingleton().createAnimal((AnimalTypes)type);

	lua_pushstring(L,animal->getName().c_str());

	/* return the number of results */
	return 1;
}

int AnimalManager::getAnimalCount(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 0

	Ogre::Real count = AnimalManager::getSingleton().getCount();

	lua_pushnumber(L,count);

	/* return the number of results */
	return 1;
}

int AnimalManager::getAnimalName(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 1, the Animal idx
	int index = luaL_checkint(L, 1);

	AnimalPtr animal = AnimalManager::getSingleton().getAnimal(index);

	lua_pushstring(L,animal->getName().c_str());

	/* return the number of results */
	return 1;
}

int AnimalManager::getAnimalPosition(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 1;
}

int AnimalManager::setAnimalPosition(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 1;
}

int AnimalManager::setAnimalState(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 2, the Animal idx
	Ogre::String name = luaL_checkstring(L, 1);
	int state = luaL_checkint(L, 2);

	AnimalPtr animal = AnimalManager::getSingleton().getAnimal(name);

	if(state == 1)
	{
		animal->setMaterialName("Skin/Green");
	}
	else if(state == 2)
	{
		animal->setMaterialName("Skin/Yellow");
	}
	else if(state == 3)
	{
		animal->setMaterialName("Skin/Blue");
	}
	else if(state == 4)
	{
		animal->setMaterialName("Skin/Red");
	}
	else
	{
		animal->setMaterialName("Skin");
	}

	/* return the number of results */
	return 1;
}

int AnimalManager::getAnimalStateTimeout(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 1
	Ogre::String name = luaL_checkstring(L, 1);

	AnimalPtr animal = AnimalManager::getSingleton().getAnimal(name);

	lua_pushnumber(L,animal->getStateTimeout());

	/* return the number of results */
	return 1;
}

int AnimalManager::removeAnimal(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 1;
}

int AnimalManager::setAnimalTarget(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 2
	Ogre::String animalId = luaL_checkstring(L, 1);
	Ogre::String targetId = luaL_checkstring(L, 2);

	SceneManager* sceneManager = AnimalManager::getSingleton().getSceneManager();

	AnimalPtr animal = AnimalManager::getSingleton().getAnimal(animalId);
	SceneNode* target = sceneManager->getEntity(targetId)->getParentSceneNode();

	animal->setTarget(target);

	/* return the number of results */
	return 0;
}

int AnimalManager::isAnimalHurt(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	Ogre::String animalId = luaL_checkstring(L, 1);

	AnimalPtr animal = AnimalManager::getSingleton().getAnimal(animalId);

	lua_pushboolean(L,animal->isHurt());

	/* return the number of results */
	return 1;
}
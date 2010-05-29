#include "..\..\..\include\Entity\Item\ItemManager.h"

using namespace WyvernsAssault;

// BEGIN SINGLETON
template<> ItemManager* Ogre::Singleton<ItemManager>::ms_Singleton = 0;
ItemManager* ItemManager::getSingletonPtr(void)
{
    return ms_Singleton;
}
ItemManager& ItemManager::getSingleton(void)
{  
    assert( ms_Singleton );  return ( *ms_Singleton );  
}
// END SINGLETON

ItemManager::ItemManager(Ogre::SceneManager* sceneManager): mId(0)
{
	mSceneManager = sceneManager;
}

ItemManager::~ItemManager()
{
	finalize();
}

bool ItemManager::initialize()
{
	//
	// TODO
	//
	return true;
}

void ItemManager::finalize()
{
	//
	// TODO
	//
}

ItemPtr ItemManager::createItem(ItemTypes type)
{
	Ogre::String mesh;

	switch(type)
	{
	case LiveSmall:
		mesh = Ogre::String("crown.mesh");
		break;
	case LiveMedium:
		mesh = Ogre::String("beerGlass.mesh");
		break;
	default:
		mesh = Ogre::String("beerTank.mesh");
		break;
	}

	Ogre::String name = createUniqueId();

	return createItem(type, name, mesh);
}

ItemPtr ItemManager::createItem(ItemTypes type, Ogre::String name, Ogre::String mesh)
{
	// Item name == Mesh Name!
	Ogre::Entity* itemMesh = mSceneManager->createEntity(name, mesh);
	Ogre::SceneNode* itemSceneNode = mSceneManager->getRootSceneNode()->createChildSceneNode();

	itemSceneNode->attachObject(itemMesh);

	ItemPtr item = ItemPtr(new Item(type));
	item->initializeEntity(itemMesh, itemSceneNode);

	mItemList.push_back(item);
	mItemMap[name] = item;

	//mCount++;

	return item;
}

Ogre::String ItemManager::createUniqueId()
{
	Ogre::StringStream countStrStr;
	countStrStr << mId;

	Ogre::String uniqueId = "Item_" + countStrStr.str();

	mId++;

	return uniqueId;
}

ItemPtr ItemManager::getItem(int index)
{
	return mItemList[index];
}


ItemPtr ItemManager::getItem(Ogre::String name)
{
	return mItemMap[name];
}

bool ItemManager::removeItem(Ogre::String name)
{
	//
	// TODO : maybe we don't really need a list, and we can just use a map...
	//
	ItemPtr itemToErase = mItemMap[name];

	mItemMap.erase(name);
	
	ItemListIterator it = find(mItemList.begin(), mItemList.end(), itemToErase);
	
	if( it != mItemList.end() )
		mItemList.erase(it);

	mSceneManager->destroyEntity(name);

	return true;
}

void ItemManager::update(const float elapsedSeconds)
{
	for(int i = 0; i < mItemList.size() ; i++)
	{
		ItemPtr item =  mItemList[i];

		if(item->getItemState() == ItemStates::Catch){			
			ParticleManager::getSingletonPtr()->remove(item->getSceneNode(), item->getName());
			removeItem(item->getName());
		}else{
			item->updateLogic(L,elapsedSeconds);
			item->updateEntity(elapsedSeconds);
		}
	}
}

// --------------------------------
// Lua Enemy Lib
// --------------------------------
LUA_BEGIN_BINDING(ItemManager::itemlib)
LUA_BIND("create", ItemManager::createItem)
LUA_BIND("remove", ItemManager::removeItem)
LUA_END_BINDING()

int ItemManager::createItem(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 1, the enemy type

	int type = luaL_checkint(L, 1);

	ItemPtr item = ItemManager::getSingleton().createItem((ItemTypes)type);

	lua_pushstring(L,item->getName().c_str());

	/* return the number of results */
	return 1;
}

int ItemManager::removeItem(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 1;
}

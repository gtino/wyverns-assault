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

ItemManager::ItemManager(Ogre::SceneManager* sceneManager)
: mId(0)
, mItemNode(0)
{
	mSceneManager = sceneManager;
}

ItemManager::~ItemManager()
{
	finalize();
}

bool ItemManager::initialize()
{
	mItemNode = mSceneManager->getRootSceneNode()->createChildSceneNode(ITEM_NODE_NAME);

	return true;
}

void ItemManager::finalize()
{
	mItemList.clear();
	mItemMap.clear();

	Utils::Destroy(mSceneManager,ITEM_NODE_NAME);
	mItemNode = NULL;
}

ItemPtr ItemManager::createItem(Item::ItemTypes type)
{
	Ogre::String mesh;

	switch(type)
	{
	case Item::ItemTypes::LiveSmall:
		mesh = Ogre::String("crown.mesh");
		break;
	case Item::ItemTypes::LiveMedium:
		mesh = Ogre::String("beerGlass.mesh");
		break;
	default:
		mesh = Ogre::String("beerTank.mesh");
		break;
	}

	Ogre::String name = createUniqueId();

	return createItem(type, name, mesh);
}

ItemPtr ItemManager::createItem(Item::ItemTypes type, Ogre::String name, Ogre::String mesh)
{
	// Item name == Mesh Name!
	Ogre::Entity* itemMesh = mSceneManager->createEntity(name, mesh);
	itemMesh->setQueryFlags(SceneManager::ENTITY_TYPE_MASK);
	Ogre::SceneNode* itemSceneNode = mItemNode->createChildSceneNode();

	itemSceneNode->attachObject(itemMesh);

	ItemPtr item = ItemPtr(new Item(name, type));
	item->initializeEntity(itemMesh, itemSceneNode, mSceneManager);

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

	// Fire the event
	ItemRemovedEventPtr e = ItemRemovedEventPtr(new ItemRemovedEvent(itemToErase) );
	raiseEvent(e);

	return true;
}

void ItemManager::update(const float elapsedSeconds)
{
	ItemList itemsToRemove;

	for(int i = 0; i < mItemList.size() ; i++)
	{
		ItemPtr item =  mItemList[i];

		if(item->getItemState() == Item::ItemStates::Removed)
		{
			itemsToRemove.push_back(item);
		}
		else 
		{
			if(item->getItemState() == Item::ItemStates::Catch)
			{			
				ParticleManager::getSingleton().remove(item->_getSceneNode(), item->getName());
				ParticleManager::getSingleton().glow(item->_getSceneNode());

				item->caught();

				ItemCatchEventPtr evt = ItemCatchEventPtr(new ItemCatchEvent(item));
				raiseEvent(evt);
			}

			item->updateLogic(L,elapsedSeconds);
			item->updateEntity(elapsedSeconds);
		}
	}

	// Now we have to remove them and notify other listeners with the 'dead/remove' event!
	for(int i = 0; i < itemsToRemove.size(); i++)
	{
		ItemPtr item = itemsToRemove[i];
		removeItem(item->getName());
	}

	itemsToRemove.clear();	
}


// --------------
// Event handlers
// --------------
EVENTS_BEGIN_REGISTER_HANDLERS(ItemManager)
	EVENTS_REGISTER_HANDLER(ItemManager,ItemCatch)
	EVENTS_REGISTER_HANDLER(ItemManager,ItemRemoved)
EVENTS_END_REGISTER_HANDLERS()

EVENTS_BEGIN_UNREGISTER_HANDLERS(ItemManager)
	EVENTS_UNREGISTER_HANDLER(ItemManager,ItemCatch)
	EVENTS_UNREGISTER_HANDLER(ItemManager,ItemRemoved)
EVENTS_END_UNREGISTER_HANDLERS()

EVENTS_DEFINE_HANDLER(ItemManager,ItemCatch)
{
	//TODO
}

EVENTS_DEFINE_HANDLER(ItemManager,ItemRemoved)
{
	//TODO
}

// --------------------------------
// Lua Item Lib
// --------------------------------
LUA_BEGIN_BINDING(ItemManager::itemlib)
LUA_BIND("create", ItemManager::createItem)
LUA_BIND("remove", ItemManager::removeItem)
LUA_BIND("getStateTimeout", ItemManager::getItemStateTimeout)
LUA_END_BINDING()

int ItemManager::createItem(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 1, the enemy type

	int type = luaL_checkint(L, 1);

	ItemPtr item = ItemManager::getSingleton().createItem((Item::ItemTypes)type);

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

int ItemManager::getItemStateTimeout(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	// n should be 1
	Ogre::String name = luaL_checkstring(L, 1);

	ItemPtr item = ItemManager::getSingleton().getItem(name);

	lua_pushnumber(L,item->getStateTimeout());

	/* return the number of results */
	return 1;
}
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
,mIsDebugEnabled(false)
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

	int subType = rand();

	switch(type)
	{
	case Item::ItemTypes::Life:
		if( subType%2 == 0 )
			mesh = Ogre::String("chickenLegItem.mesh");
		else
			mesh = Ogre::String("chickenItem.mesh");
		break;
	case Item::ItemTypes::Points:
		if( subType%2 == 0 )
			mesh = Ogre::String("ruby.mesh");
		else
			mesh = Ogre::String("crown.mesh");
		break;
	case Item::ItemTypes::Drunk:
		if( subType%3 == 0 )
			mesh = Ogre::String("beerHorn.mesh");
		if( subType%3 == 1 )
			mesh = Ogre::String("beerGlass.mesh");
		else
			mesh = Ogre::String("beerTank.mesh");
		break;
	}

	Ogre::String name = createUniqueId();

	Ogre::Entity* itemMesh = mSceneManager->createEntity(name, mesh);
	itemMesh->setCastShadows(true);
	itemMesh->setQueryFlags(SceneManager::ENTITY_TYPE_MASK);

	Ogre::SceneNode* sceneNode = mItemNode->createChildSceneNode(name + "_Node");
	sceneNode->attachObject(itemMesh);

	Item::ItemParameters params;

	// Get standar parameters for item type

	return createItem(type, name, itemMesh, sceneNode, params);
}

ItemPtr ItemManager::createItem(Item::ItemTypes type, Ogre::String name, Ogre::Entity* mesh, Ogre::SceneNode* sceneNode, Item::ItemParameters params)
{
	// Item name == Mesh Name!
	Ogre::Entity* itemMesh = mesh;

	sceneNode->attachObject(itemMesh);

	ItemPtr item = ItemPtr(new Item(name, type, params));
	item->initializeEntity(itemMesh, sceneNode, mSceneManager);

	mItemList.push_back(item);
	mItemMap[name] = item;

	//mCount++;

	ItemCreationEventPtr evt = ItemCreationEventPtr(new ItemCreationEvent(item));	
	EVENTS_FIRE(evt);

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

	return true;
}

void ItemManager::update(const float elapsedSeconds)
{
	for(int i = 0; i < mItemList.size() ; i++)
	{
		ItemPtr item =  mItemList[i];

		item->updateEntity(elapsedSeconds);
	}
}

void ItemManager::setDebugEnabled(bool isDebugEnabled)
{
	if(mIsDebugEnabled != isDebugEnabled)
	{
		mIsDebugEnabled = isDebugEnabled;

		for(int i = 0; i < mItemList.size() ; i++)
		{
			ItemPtr item =  mItemList[i];
			item->setDebugEnabled(mIsDebugEnabled);
		}
	}
}

// --------------
// Event handlers
// --------------
EVENTS_BEGIN_REGISTER_HANDLERS(ItemManager)
	EVENTS_REGISTER_HANDLER(ItemManager,ItemCatch)
	EVENTS_REGISTER_HANDLER(ItemManager,ItemRemove)
EVENTS_END_REGISTER_HANDLERS()

EVENTS_BEGIN_UNREGISTER_HANDLERS(ItemManager)
	EVENTS_UNREGISTER_HANDLER(ItemManager,ItemCatch)
	EVENTS_UNREGISTER_HANDLER(ItemManager,ItemRemove)
EVENTS_END_UNREGISTER_HANDLERS()

EVENTS_DEFINE_HANDLER(ItemManager,ItemCatch)
{
	Debug::Out("ItemManager : handleItemCatchEvent");

	ItemPtr item = evt->getItem();
	PlayerPtr player = evt->getPlayer();

	item->caught();

	// Item will be removed after a while
	ItemRemoveEventPtr re = ItemRemoveEventPtr(new ItemRemoveEvent(item));
	EVENTS_FIRE_AFTER(re,3.0f);
}

EVENTS_DEFINE_HANDLER(ItemManager,ItemRemove)
{
	Debug::Out("ItemManager : handleItemRemoveEvent");

	ItemPtr item = evt->getItem();	

	removeItem(item->getName());
}

// --------------------------------
// Lua Particle Lib
// --------------------------------
LUA_BEGIN_BINDING(ItemManager, itemlib)
LUA_BIND(ItemManager, create)
LUA_BIND(ItemManager, remove)
LUA_END_BINDING()

//
// Load lua scripts that will be used by this manager
//
LUA_BEGIN_LOAD_SCRIPTS(ItemManager)
// 
// TODO : Load scripts if needed
//
LUA_END_LOAD_SCRIPTS()

LUA_DEFINE_FUNCTION(ItemManager, create)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 1;
}

LUA_DEFINE_FUNCTION(ItemManager, remove)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 1;
}

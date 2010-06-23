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


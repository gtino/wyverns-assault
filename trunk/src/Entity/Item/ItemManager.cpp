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
, mIsDebugEnabled(false)
, mCurrentGameArea(-1)
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
	mItemMapList.clear();

	Utils::Destroy(mSceneManager,ITEM_NODE_NAME);
	mItemNode = NULL;
}
Item::ItemParameters ItemManager::createParameters(Item::ItemTypes type, int subType)
{
	Item::ItemParameters params;
	
	if( type == Item::ItemTypes::Life )
	{
		if( subType == 0 )
		{
			params.life = 50;
			params.points = 100;
			params.drunkTime = 0;
		}
		else
		{
			params.life = 100;
			params.points = 200;
			params.drunkTime = 0;
		}

	}
	else if( type == Item::ItemTypes::Points )
	{
		if( subType == 0 )
		{
			params.life = 0;
			params.points = 250;
			params.drunkTime = 0;
		}
		else
		{
			params.life = 0;
			params.points = 500;
			params.drunkTime = 0;
		}

	}
	else
	{
		if( subType == 0 )
		{
			params.life = 0;
			params.points = 100;
			params.drunkTime = 3;
		}
		else if( subType == 1 )
		{
			params.life = 0;
			params.points = 200;
			params.drunkTime = 5;
		}
		else
		{
			params.life = 0;
			params.points = 300;
			params.drunkTime = 7;
		}
	}

	return params;
}

ItemPtr ItemManager::createItem(Item::ItemTypes type, int subType, Vector3 position)
{
	Ogre::String mesh;

	switch(type)
	{
	case Item::ItemTypes::Life:
		if( subType == 0 )
			mesh = Ogre::String("chickenLegItem.mesh");
		else
			mesh = Ogre::String("chickenItem.mesh");
		break;
	case Item::ItemTypes::Points:
		if( subType == 0 )
			mesh = Ogre::String("ruby.mesh");
		else
			mesh = Ogre::String("crown.mesh");
		break;
	case Item::ItemTypes::Drunk:
		if( subType == 0 )
			mesh = Ogre::String("beerHorn.mesh");
		if( subType == 1 )
			mesh = Ogre::String("beerGlass.mesh");
		else
			mesh = Ogre::String("beerTank.mesh");
		break;
	}

	Ogre::String name = createUniqueId();

	Ogre::Entity* itemMesh = mSceneManager->createEntity(name, mesh);
	itemMesh->setCastShadows(true);
	itemMesh->setQueryFlags(SceneManager::ENTITY_TYPE_MASK);

	Ogre::SceneNode* sceneNode = mItemNode->createChildSceneNode(name + "_Node", position);

	// Get standar parameters for item type
	Item::ItemParameters params = createParameters(type, subType);

	return createItem(type, name, itemMesh, sceneNode, params, mCurrentGameArea);
}

ItemPtr ItemManager::createItem(Item::ItemTypes type, Ogre::String name, Ogre::Entity* mesh, Ogre::SceneNode* sceneNode, Item::ItemParameters params, int gameArea)
{
	// Item name == Mesh Name!
	Ogre::Entity* itemMesh = mesh;

	sceneNode->attachObject(itemMesh);

	ItemPtr item = ItemPtr(new Item(name, type, params));
	item->initializeEntity(itemMesh, sceneNode, mSceneManager);

	mItemMapList[gameArea].push_back(item);

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

int ItemManager::getCount()
{
	return mItemMapList[mCurrentGameArea].size();
}

int ItemManager::getCount(int gameArea)
{
	return mItemMapList[gameArea].size();
}

ItemPtr ItemManager::getItem(int index)
{
	return mItemMapList[mCurrentGameArea][index];
}

ItemPtr ItemManager::getItem(int index, int gameArea)
{
	return mItemMapList[gameArea][index];
}

ItemPtr ItemManager::getItem(Ogre::String name)
{
	// Search item in current game area enemies list
	for( int i = 0; i < mItemMapList[mCurrentGameArea].size(); i++ )
	{
		ItemPtr item = mItemMapList[mCurrentGameArea][i];

		if( item->getName() == name )
			return mItemMapList[mCurrentGameArea][i];
	}

	// Search in others game areas lists
	for( ItemMapListIterator it = mItemMapList.begin(); it != mItemMapList.end(); ++it )
	{
		ItemList list = it->second;

		for( int i = 0; i < list.size(); i++ )
		{
			ItemPtr item = list[i];

			if( item->getName() == name )
				return list[i];
		}
	}
}

ItemPtr ItemManager::getItem(Ogre::String name, int gameArea)
{
	for( int i = 0; i < mItemMapList[gameArea].size(); i++ )
	{
		ItemPtr item = mItemMapList[gameArea][i];

		if( item->getName() == name )
			return mItemMapList[gameArea][i];
	}
}

bool ItemManager::removeItem(Ogre::String name)
{
	ItemPtr itemToErase = getItem(name);
	
	ItemListIterator it = find(mItemMapList[mCurrentGameArea].begin(), mItemMapList[mCurrentGameArea].end(), itemToErase);
	
	if( it != mItemMapList[mCurrentGameArea].end() )
	{
		mItemMapList[mCurrentGameArea].erase(it);
		return true;
	}

	return false;
}

void ItemManager::update(const float elapsedSeconds)
{
	for(int i = 0; i < mItemMapList[mCurrentGameArea].size() ; i++)
	{
		ItemPtr item =  mItemMapList[mCurrentGameArea][i];

		item->updateEntity(elapsedSeconds);

		//Physic debugg control
		item->setDebugEnabled(mIsDebugEnabled);
	}
}

void ItemManager::setDebugEnabled(bool isDebugEnabled)
{
	if(mIsDebugEnabled != isDebugEnabled)
	{
		mIsDebugEnabled = isDebugEnabled;
	}
}

// --------------
// Event handlers
// --------------
EVENTS_BEGIN_REGISTER_HANDLERS(ItemManager)
	EVENTS_REGISTER_HANDLER(ItemManager,ItemCatch)
	EVENTS_REGISTER_HANDLER(ItemManager,ItemRemove)
	EVENTS_REGISTER_HANDLER(ItemManager,GameAreaChanged)
	EVENTS_REGISTER_HANDLER(ItemManager,EnemyCreateItem)
EVENTS_END_REGISTER_HANDLERS()

EVENTS_BEGIN_UNREGISTER_HANDLERS(ItemManager)
	EVENTS_UNREGISTER_HANDLER(ItemManager,ItemCatch)
	EVENTS_UNREGISTER_HANDLER(ItemManager,ItemRemove)
	EVENTS_UNREGISTER_HANDLER(ItemManager,GameAreaChanged)
	EVENTS_UNREGISTER_HANDLER(ItemManager,EnemyCreateItem)
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

EVENTS_DEFINE_HANDLER(ItemManager,GameAreaChanged)
{
	Debug::Out("ItemManager : handleGameAreaChangedEvent");

	mCurrentGameArea = evt->getActualArea();
}

EVENTS_DEFINE_HANDLER(ItemManager,EnemyCreateItem)
{
	Debug::Out("ItemManager : handleEnemyCreateItemEvent");

	EnemyPtr enemy = evt->getEnemy();
	ItemPtr item;

	if( enemy->getEnemyType() == Enemy::EnemyTypes::Chicken || enemy->getEnemyType() == Enemy::EnemyTypes::Cow )
		item = this->createItem(Item::ItemTypes::Life, 1, enemy->getPosition());
	else if( enemy->getEnemyType() == Enemy::EnemyTypes::Woman )
	{
		if( rand()%2 == 0) 
			item = this->createItem(Item::ItemTypes::Life, (rand()%2), enemy->getPosition());
		else
			item = this->createItem(Item::ItemTypes::Points, (rand()%2), enemy->getPosition());
	}
	else
	{
		int type = rand()%3;

		if( type == 0) 
			item = this->createItem(Item::ItemTypes::Life, (rand()%2), enemy->getPosition());
		else if( type == 0) 
			item = this->createItem(Item::ItemTypes::Points, (rand()%2), enemy->getPosition());
		else
			item = this->createItem(Item::ItemTypes::Drunk, (rand()%3), enemy->getPosition());
	}

	ItemPhysicsEventPtr ePhysic = ItemPhysicsEventPtr(new ItemPhysicsEvent(item, evt->getGameArea()));
	EVENTS_FIRE(ePhysic);
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

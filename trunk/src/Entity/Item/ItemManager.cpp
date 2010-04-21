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

ItemManager::ItemManager()
{
	//
	// TODO Constructor logic HERE
	//
}

ItemManager::~ItemManager()
{
	//
	// TODO Distructor logic HERE
	//
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

// --------------------------------
// Lua Item Lib
// --------------------------------
LUA_BEGIN_BINDING(ItemManager::itemlib)
LUA_BIND("create", ItemManager::createItem)
LUA_BIND("getPosition", ItemManager::getItemPosition)
LUA_BIND("setPosition", ItemManager::setItemPosition)
LUA_BIND("remove", ItemManager::removeItem)
LUA_END_BINDING()

int ItemManager::createItem(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 1;
}

int ItemManager::getItemPosition(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 1;
}

int ItemManager::setItemPosition(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

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
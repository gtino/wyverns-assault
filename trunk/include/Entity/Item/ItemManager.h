#ifndef __ITEM_MANAGER_H__
#define __ITEM_MANAGER_H__

#define ITEM_NODE_NAME "ItemNode"

#include <Ogre.h>
#include <OgreSingleton.h>

#include <boost/enable_shared_from_this.hpp>

#include "..\..\Utils\Utils.h"
#include "..\include\Lua\LuaInterface.h"
#include "..\include\Entity\EntityManager.h"
#include "..\include\Particle\ParticleManager.h"
#include "..\..\Events\EventsInterface.h"

#include "Item.h"

namespace WyvernsAssault
{

	typedef std::map<Ogre::String,ItemPtr> ItemMap;
	typedef std::map<Ogre::String,ItemPtr>::iterator ItemMapIterator;

	typedef std::vector<ItemPtr> ItemList;
	typedef std::vector<ItemPtr>::iterator ItemListIterator;

	/**
	Class used to manage all the items
	*/
	class ItemManager : public Ogre::Singleton<ItemManager>, public boost::enable_shared_from_this<ItemManager>, public EntityManager, public LuaInterface, public EventsInterface
	{
	public:
		ItemManager(Ogre::SceneManager* sceneManager);
		~ItemManager();

		bool initialize();
		void finalize();

		static ItemManager& getSingleton(void);
		static ItemManager* getSingletonPtr(void);

		ItemPtr createItem(Item::ItemTypes type);
		ItemPtr createItem(Item::ItemTypes type, Ogre::String name, Ogre::String mesh);

		ItemPtr getItem(int index);
		ItemPtr getItem(Ogre::String name);
		
		void update(const float elpasedSeconds);

		bool removeItem(Ogre::String name);

		Ogre::SceneNode* _getSceneNode() const { return mItemNode; }

		// ----------------
		// Events interface
		// ----------------
		// Register event handlers
		void registerHandlers();
		// Unregister handlers
		void unregisterHandlers();

		void handleItemCatchEvent(ItemCatchEventPtr evt);

	private:
		Ogre::String createUniqueId();

	private:
		Ogre::SceneManager* mSceneManager;
		Ogre::SceneNode* mItemNode;

		ItemList mItemList;
		ItemMap mItemMap;
		int mId;

	// --------------------------------
	// BEGIN Lua Interface Declarations
	// -------------------------------- 
	public:
		// Item Lib (exported to Lua)
		LUA_LIBRARY("Item",itemlib);

		LUA_FUNCTION(createItem)
		LUA_FUNCTION(removeItem)
		LUA_FUNCTION(getItemStateTimeout)

	public:
		void luaLoadScripts(){luaLoadScript(".\\data\\scripts\\ItemLogic.lua");};
	// ------------------------------
	// END Lua Interface Declarations
	// ------------------------------
	};

	typedef boost::shared_ptr<ItemManager> ItemManagerPtr;
}

#endif // __ITEM_MANAGER_H__
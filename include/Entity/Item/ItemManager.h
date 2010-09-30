/*
-----------------------------------------------------------------------------
Wyverns' Assault 3D Videgame.
Copyright (C) 2010  Giorgio Tino, Javier Soto Huesa, Jordi Carreras Ribot, 
					Marc Serena, Elm Oliver Torres

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

-----------------------------------------------------------------------------
*/
#ifndef __ITEM_MANAGER_H__
#define __ITEM_MANAGER_H__

#define ITEM_NODE_NAME "ItemNode"

#include <Ogre.h>
#include <OgreSingleton.h>

#include <boost/enable_shared_from_this.hpp>

#include "..\..\Debug\Debug.h"

#include "..\..\Utils\Utils.h"
#include "..\..\Entity\EntityManager.h"
#include "..\..\Particle\ParticleManager.h"
#include "..\..\Events\EventsInterface.h"

#include "..\..\Lua\LuaInterface.h"

#include "Item.h"

namespace WyvernsAssault
{
	typedef std::vector<ItemPtr> ItemList;
	typedef std::vector<ItemPtr>::iterator ItemListIterator;

	typedef std::map<int, ItemList> ItemMapList;
	typedef std::map<int, ItemList>::iterator ItemMapListIterator;

	/**
	Class used to manage all the items
	*/
	class ItemManager : public Ogre::Singleton<ItemManager>, 
						public boost::enable_shared_from_this<ItemManager>, 
						public EntityManager, 
						public EventsInterface,
						public LuaInterface
	{
	public:
		ItemManager(Ogre::SceneManager* sceneManager);
		~ItemManager();

		bool initialize();
		void finalize();

		static ItemManager& getSingleton(void);
		static ItemManager* getSingletonPtr(void);

		Item::ItemParameters createParameters(Item::ItemTypes type, int subType);

		ItemPtr createItem(Item::ItemTypes type, int subType, Vector3 position);
		ItemPtr createItem(Item::ItemTypes type, Ogre::String name, Ogre::Entity* mesh, Ogre::SceneNode* sceneNode, Item::ItemParameters params, int gameArea);

		int getCount();
		int getCount(int gameArea);

		ItemPtr getItem(int index);
		ItemPtr getItem(int index, int gameArea);
		ItemPtr getItem(Ogre::String name);
		ItemPtr getItem(Ogre::String name, int gameArea);
		
		bool removeItem(Ogre::String name);

		void update(const float elpasedSeconds);

		// Enable Debug Stuff
		void setDebugEnabled(bool isDebugEnabled);
		bool getDebugEnabled(){return mIsDebugEnabled;};		

		Ogre::SceneNode* _getSceneNode() const { return mItemNode; }

		//
		// Enable/disable the manager
		//
		void disable(){mEnabled = false;}
		void enable(){mEnabled = true;}
		bool isEnabled(){return mEnabled;}

	private:
		Ogre::String createUniqueId();

	private:
		Ogre::SceneManager*		mSceneManager;
		Ogre::SceneNode*		mItemNode;

		// Where items are stored and sorted by game area
		ItemMapList				mItemMapList;
		int						mCurrentGameArea;

		int						mId;
		bool					mIsDebugEnabled;
		bool					mEnabled;

	public:
		// ----------------
		// Events interface
		// ----------------
		EVENTS_INTERFACE()

		EVENTS_HANDLER(ItemCatch)
		EVENTS_HANDLER(ItemRemove)		
		EVENTS_HANDLER(GameAreaChanged)
		EVENTS_HANDLER(EnemyCreateItem)

		// --------------------------------
		// BEGIN Lua Interface Declarations
		// --------------------------------
	public:
		LUA_INTERFACE();

		//Particle Lib (exported to Lua)
		LUA_LIBRARY("Item",itemlib);

		LUA_FUNCTION(create);
		LUA_FUNCTION(remove);
		LUA_FUNCTION(enable);
		LUA_FUNCTION(disable);
		LUA_FUNCTION(isEnabled);
		// ------------------------------
		// END Lua Interface Declarations
		// ------------------------------
	};

	typedef boost::shared_ptr<ItemManager> ItemManagerPtr;
}

#endif // __ITEM_MANAGER_H__
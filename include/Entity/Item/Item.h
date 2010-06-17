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
#ifndef __ITEM_H__
#define __ITEM_H__

#include <Ogre.h>

#include "..\EntityInterface.h"
#include "..\..\Physics\PhysicsInterface.h"
#include "..\..\Logic\LogicInterface.h"

namespace WyvernsAssault
{
	/**
	Class used to manage single item
	*/
	class Item : public EntityInterface, public LogicInterface, public PhysicsInterface
	{
	public:
		/** List of item types */
		enum ItemTypes
		{
			LiveSmall	= 0,
			LiveMedium	= 1,
			LiveBig	    = 2,
			PowerSmall	= 3,
			PowerMedium	= 4,
			PowerBig	= 5,
			ScoreSmall  = 6,
			ScoreBig    = 7
		};

		enum ItemStates
		{
			Initial = 0,
			Catch = 1,
			Caught = 2,
			Removed = 3
		};

	public:
		Item(Ogre::String name, Item::ItemTypes type);
		~Item();
		
		//
		// Entity Interface
		//
		void initializeEntity(Ogre::Entity* mesh, Ogre::SceneNode* sceneNode, Ogre::SceneManager* sceneManager);
		void finalizeEntity();
		void updateEntity(const float elapsedSeconds);

		void caught();
		
		//
		// Logic Interface
		//
		void updateLogic(lua_State *L, const float elapsedSeconds);

		Item::ItemStates getItemState(){return mState;}
		float getStateTimeout(){return mStateTimeout;}

		float getLife(){ return mLife; }
		float getSpecial(){ return mSpecial; }
		float getScore(){ return mScore; }

	private:
		Item::ItemTypes mType;
		Item::ItemStates mState;
		float mStateTimeout;

		float	mLife;
		float	mSpecial;
		float	mScore;

	public:
		static Item::ItemTypes StringToType(Ogre::String typeStr);
	};

	typedef boost::shared_ptr<Item> ItemPtr;
}

#endif // __ITEM_H__
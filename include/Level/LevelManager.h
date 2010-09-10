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
#ifndef __LEVEL_MANAGER_H_
#define __LEVEL_MANAGER_H_

#include <Ogre.h>
#include <OgreRenderWindow.h>

#include <boost/enable_shared_from_this.hpp>

#include "..\Debug\Debug.h"
#include "..\Utils\Utils.h"
#include "..\Lua\LuaInterface.h"
#include "..\Events\EventsInterface.h"

#include "Level.h"

namespace WyvernsAssault
{
	typedef std::map<int, LevelPtr> LevelMap;
	typedef std::map<int, LevelPtr>::iterator LevelMapIterator;

	/**
		Class used to manager user input, from keyboard, mouse, joystick and so on...
	*/
	class LevelManager	: public Ogre::Singleton<LevelManager>
						, public boost::enable_shared_from_this<LevelManager>
						, public LuaInterface
						, public EventsInterface
	{
	public:
		LevelManager();
		~LevelManager();
		static LevelManager& getSingleton(void);
		static LevelManager* getSingletonPtr(void);

	public:
		/** Initialize the level manager */
		bool initialize(int initialLevel);
		/** Finalize the level manager */
		void finalize();

		void update(const float elapsedSeconds);

		void change(int newLevel);
		bool next();

		//
		// Enable/disable manager
		//
		void disable(){mEnabled = false;}
		void enable(){mEnabled = true;}
		bool isEnabled(){return mEnabled;}
		bool isLast(){return mIsLast;}

		/** Get current level index */
		LevelPtr getCurrentLevel();
		int getCurrentLevelIndex();

	protected:
		bool mInitialized;
		bool mEnabled;

		int mCurrentLevelIndex;
		LevelMap mLevels;
		bool mIsLast;

	public:
		// ----------------
		// Events interface
		// ----------------
		EVENTS_INTERFACE()

		EVENTS_HANDLER(GameAreaChanged)
		EVENTS_HANDLER(GameAreaCleared)

	public:
		// --------------------------------
		// BEGIN Lua Interface Declarations
		// --------------------------------
		public:
			LUA_INTERFACE();

			// Physics Lib (exported to Lua)
			LUA_LIBRARY("Level",levellib);

			LUA_FUNCTION(change);
			LUA_FUNCTION(enable);
			LUA_FUNCTION(disable);
			LUA_FUNCTION(isEnabled);
		// ------------------------------
		// END Lua Interface Declarations
		// ------------------------------
	};

	typedef boost::shared_ptr<LevelManager> LevelManagerPtr;
}

#endif // __LEVEL_MANAGER_H_
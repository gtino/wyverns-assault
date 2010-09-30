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
#ifndef __GAMEAREA_MANAGER_H_
#define __GAMEAREA_MANAGER_H_

#include <Ogre.h>
#include <OgreSingleton.h>

#include <boost/enable_shared_from_this.hpp>

#include "..\Debug\Debug.h"

#include "..\Utils\Utils.h"
#include "..\Events\EventsInterface.h"

#include "..\Lua\LuaInterface.h"

using namespace Ogre;

#define CURRENT_LEVEL 0 // HACK, to be fixed! (0 : Level1_1, 1 : Boss)

namespace WyvernsAssault 
{
	/**
	Class used to manage all the game areas
	*/
	class GameAreaManager	: public Ogre::Singleton<GameAreaManager>
							, public boost::enable_shared_from_this<GameAreaManager>
							, public EventsInterface
							, public LuaInterface
	{
	public:
		
		struct GameArea
		{
			int mLevel;

			Ogre::Vector3	mBeginNear;
			Ogre::Vector3	mEndNear;
			Ogre::Vector3	mBeginFar;
			Ogre::Vector3	mEndFar;

			Ogre::Real		mFinishTime;
			int				mEnemies;
			int				mDifficult;
			int				mType;
			//
			// Game Area Types
			//
			// 0: Cleared by kkiling enemies
			// 1: Cleared by time
			// 2: No need to be cleared
			//
			// 10: Boos game area, no need to be cleared

			std::vector<Vector3> mSpawnPoints;

			bool			mCleared;
		};

		GameAreaManager();
		~GameAreaManager();
		static GameAreaManager& getSingleton(void);
		static GameAreaManager* getSingletonPtr(void);

		void initialize();
		void load(Ogre::String file);
		void update(Vector3 playerPosition, const float elapsedSeconds);
		
		int getGameArea() { return mCurrentGameArea; }
		void setGameArea(int gameArea) { mCurrentGameArea = gameArea; }

		// Manage game area enemy creation events
		void manageGameArea();

		// Get parameters
		int getEnemies(){ return mGameAreas[mCurrentGameArea].mEnemies; } 
		int getDifficult(){ return mGameAreas[mCurrentGameArea].mDifficult; }
		int getType(){ return mGameAreas[mCurrentGameArea].mType; }
		Real getFinishTime(){ return mGameAreas[mCurrentGameArea].mFinishTime; }
		Real getTime(){ return mTime; }

		// Get game area from a position
		int positionGameArea(Vector3 position);

		// Get a point from game area boundaries
		Vector3 getSpawnPoint(int gameArea);

		// Enable Debug Stuff
		void setDebugEnabled(bool isDebugEnabled);
		bool getDebugEnabled(){return mIsDebugEnabled;};

		// Add game area to game area list
		void addGameArea(int level, GameArea area);
		int getLastGameArea();

		//
		// Enable/disable the manager
		//
		void disable(){mEnabled = false;}
		void enable(){mEnabled = true;}
		bool isEnabled(){return mEnabled;}

	private:

		std::vector<GameArea> mGameAreas;				
		
		int		mCurrentLevel;
		int		mCurrentGameArea;
		bool	mGameAreaCleared;

		bool	mEnemiesAlive;
		Real	mTime;
		bool	mAreaManaged;

		bool	mIsDebugEnabled;
		bool	mEnabled;

	private:

		bool mInitialized;

	public:
		// ----------------
		// Events interface
		// ----------------
		EVENTS_INTERFACE()

		EVENTS_HANDLER(GameAreaEnemiesDeath)
		EVENTS_HANDLER(EnemyCreation)

		// --------------------------------
		// BEGIN Lua Interface Declarations
		// --------------------------------
	public:
		LUA_INTERFACE();

		//Particle Lib (exported to Lua)
		LUA_LIBRARY("GameArea",gameArealib);
		
		LUA_FUNCTION(enable);
		LUA_FUNCTION(disable);
		LUA_FUNCTION(isEnabled);
		// ------------------------------
		// END Lua Interface Declarations
		// ------------------------------

	};

	typedef boost::shared_ptr<GameAreaManager> GameAreaManagerPtr;
}

#endif //__GAMEAREA_MANAGER_H_
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
#ifndef __CUTSCENE_MANAGER_H__
#define __CUTSCENE_MANAGER_H__

#include <Ogre.h>
#include <OgreSingleton.h>
#include <boost/enable_shared_from_this.hpp>

#include "..\Utils\Utils.h"
#include "..\Lua\LuaInterface.h"
#include "..\Events\EventsInterface.h"

using namespace Ogre;

namespace WyvernsAssault 
{
	/**
	Class used to manage all the lights
	*/
	class CutSceneManager	: public Ogre::Singleton<CutSceneManager>
							, public boost::enable_shared_from_this<CutSceneManager>
							, public LuaInterface
							, public EventsInterface
	{

	public:
		typedef enum CutSceneId
		{
			Nothing = 0,
			Intro = 1,
			FirstKills = 2,
			Beer = 3,
			Bridge = 4,
			Forest = 5,
			WoodenWall = 6,
			Village = 7,
			Boss = 8
		};

	public:
		CutSceneManager(SceneManager* sceneManager);
		~CutSceneManager();
		static CutSceneManager& getSingleton(void);
		static CutSceneManager* getSingletonPtr(void);

	public:
		/** Initialize the lights manager */
		void initialize();
		/** Finalize the lights manager */
		void finalize();
		/** Update and run lua scripts */
		void update(const float elapsedSeconds);
		void play(CutSceneId id);

		Ogre::SceneNode* _getSceneNode() const { return mCutSceneNode; }

	private:
		const int getCurrentStep(){return mCurrentStep;}
		const float getElapsedSceneTime(){return mElapsedSceneTime;}
		const bool wait(const float timeout);
		const int nextStep(){return ++mCurrentStep;}
		void reset();

	private:
		SceneManager* mSceneManager;
		SceneNode* mCutSceneNode;

		CutSceneId mCutSceneId;
		int mCurrentStep;
		float mElapsedSceneTime;
		float mWaitTimer;

	public:
		// ----------------
		// Events interface
		// ----------------
		EVENTS_INTERFACE();

		EVENTS_HANDLER(GameAreaChanged);

	// --------------------------------
	// BEGIN Lua Interface Declarations
	// --------------------------------
	public:
		LUA_INTERFACE();

		// Export library lib as "CutScene" to be called from Lua scripts
		LUA_LIBRARY("CutScene",cutscenelib)

		// From Lua to C++
		LUA_FUNCTION(getCurrentStep)
		LUA_FUNCTION(getElapsedSceneTime)
		LUA_FUNCTION(wait)
		LUA_FUNCTION(nextStep)
		LUA_FUNCTION(reset)

	private:
		// ----------------------------
		// Lua Routines called from C++
		// ----------------------------
		bool playCutScene(const float totalSeconds, Ogre::String cutSceneName);
	// ------------------------------
	// END Lua Interface Declarations
	// ------------------------------
	};

	typedef boost::shared_ptr<CutSceneManager> CutSceneManagerPtr;
}

#endif // __CUTSCENE_MANAGER_H__
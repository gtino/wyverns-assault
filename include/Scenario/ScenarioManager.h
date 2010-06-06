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
#ifndef __SCENARIO_MANAGER_H_
#define __SCENARIO_MANAGER_H_

#include <Ogre.h>
#include <OgreSingleton.h>

#include <boost/enable_shared_from_this.hpp>

#include "..\Events\EventsInterface.h"
#include "DotSceneLoader.h"

using namespace Ogre;

namespace WyvernsAssault 
{
	/**
	Class used to manage all the scenarios
	*/
	class ScenarioManager	: public Ogre::Singleton<ScenarioManager>
							, public boost::enable_shared_from_this<ScenarioManager>
							, public EventsInterface
	{
	public:
		ScenarioManager(SceneManager* sceneManager);
		~ScenarioManager();
		static ScenarioManager& getSingleton(void);
		static ScenarioManager* getSingletonPtr(void);

		void initialize(CameraManagerPtr cameraManager, LightsManagerPtr lightsManager, EnemyManagerPtr enemyManager, PhysicsManagerPtr physicsManager, ItemManagerPtr itemManager, ParticleManagerPtr particleManager);
		void load(Ogre::String file);

	public:
		// ----------------
		// Events interface
		// ----------------
		// Register event handlers
		void registerHandlers(){};
		// Unregister handlers
		void unregisterHandlers(){};

	private:
		SceneManager*			mSceneManager;
		SceneNode*				mScenarioNode; // Base node for the scenario
		DotSceneLoader*			mDotSceneLoader;

	private: // Managers shamt pointers
		CameraManagerPtr mCameraManager;
		LightsManagerPtr mLightsManager;
		EnemyManagerPtr mEnemyManager;
		PhysicsManagerPtr mPhysicsManager;
		ItemManagerPtr mItemManager;
		ParticleManagerPtr mParticleManager;

	private:
		bool mInitialized;
	};

	typedef boost::shared_ptr<ScenarioManager> ScenarioManagerPtr;
}

#endif //__SCENARIO_MANAGER_H_
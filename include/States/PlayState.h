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
#ifndef __PLAY_STATE_H_
#define __PLAY_STATE_H_

#include "BaseState.h"
#include "..\Camera\CameraManager.h"
#include "..\Graphics\LightsManager.h"
#include "..\Physics\PhysicsManager.h"
#include "..\Logic\LogicManager.h"
#include "..\Lua\LuaManager.h"
#include "..\Entity\Player\PlayerManager.h"
#include "..\Entity\Enemy\EnemyManager.h"
#include "..\Entity\Item\ItemManager.h"
#include "..\Particle\ParticleManager.h"
#include "..\Events\EventsManager.h"
#include "..\Scenario\ScenarioManager.h"
#include "..\include\Graphics\PostProcessManager.h"
#include "..\include\Scenario\DotSceneLoader.h"

#include <boost/scoped_ptr.hpp>

#include <SdkTrays.h>
#include <ParticleUniversePlugin.h>

namespace WyvernsAssault
{
	/**
		Play game state class
	*/
	class PlayState : public BaseState
	{
	public:
		PlayState(GraphicsManagerPtr graphicsManager, InputManagerPtr inputManager, AudioManagerPtr audioManager, CameraManagerPtr cameraManager, GuiManagerPtr guiManager);
		~PlayState();

	public:
		/** Initialize current state */
		void initialize();
		/** Load resources */
		void load();
		/** Manage input - INPUT */
		void input();
		/**  Renderin queue */
		bool frameRenderingQueued(const Ogre::FrameEvent& evt);
		/** Update internal stuff - PROCESS */
		void update(const float elapsedSeconds);
		/** Render - OUTPUT */
		void render(const float elapsedSeconds);
		/** Unload resources */
		void unload();
		/** Destroy the state */
		void finalize();
		/** Called when the state has to be paused */
		void pause();
		/** Called when the state has to be resumed (from pause) */
		void resume();

		/** Get state Id */
		GameStateId getStateId();

		/** Buffered input - keyboard key clicked */
		bool keyPressed(const OIS::KeyEvent& e);
		bool keyReleased(const OIS::KeyEvent& e);

		/** Mouse input */
		bool mouseMoved(const OIS::MouseEvent& e);

	private:
		LightsManagerPtr mLightsManager;
		PlayerManagerPtr mPlayerManager;
		EnemyManagerPtr mEnemyManager;
		LogicManagerPtr mLogicManager;
		LuaManagerPtr	mLuaManager;	
		PhysicsManagerPtr mPhysicsManager;
		ItemManagerPtr mItemManager;
		ParticleManagerPtr mParticleManager;
		EventsManagerPtr mEventsManager;
		ScenarioManagerPtr mScenarioManager;
		PostProcessManagerPtr mPostProcessManager;

		SceneNode* mRootSceneNode;

		//
		// SdkTrays manager
		//
		OgreBites::SdkTrayManager* mTrayMgr;
		OgreBites::ParamsPanel* mDetailsPanel;

		//
		// User interface
		//
		GuiUserInterface* mPlayerUI;
		
	};
}
#endif // __PLAY_STATE_H_
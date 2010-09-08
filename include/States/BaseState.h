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
#ifndef __BASE_STATE_H_
#define __BASE_STATE_H_

//
// Graphics Manager
//
#include "..\Graphics\GraphicsManager.h"

//
// User input
//
#include "..\Input\InputManager.h"
#include "..\Input\InputListener.h"

//
// Audio
//
#include "..\Audio\AudioManager.h"

//
// Cameras
//
#include "..\Camera\CameraManager.h"

//
// Gui
//
#include "..\GUI\GuiManager.h"

namespace WyvernsAssault
{
	/** List of game states of the game */
	enum GameStateId
	{
		SplashScreen	= 0, // Splash screen
		Intro			= 1,
		MainMenu		= 2,
		Play			= 3,
		LevelLoading	= 4,
		GameOver		= 5,
		Ending			= 6,
		Outro			= 7,
		Credits			= 8,
		Options			= 9,
		Pause			= 10,
		Previous		= 11,// Special state, it is just a flag to return back to whatever the previous state was.
		Exit			= 12 // Special state, it is just a flag to exit the application
	};

	/**
		Base game state class
	*/
	class BaseState : public InputListener
					, public FrameListener
	{
	public:
		BaseState();
		BaseState(	GraphicsManagerPtr graphicsManager, 
					InputManagerPtr inputManager, 
					AudioManagerPtr audioManager, 
					CameraManagerPtr cameraManager, 
					GuiManagerPtr guiManager);
		virtual ~BaseState() = 0;

	public:
		/** Initialize current state */
		virtual void initialize() = 0;
		/** Load resources */
		virtual void load() = 0;
		/** Manage input - INPUT */
		virtual void input() = 0;
		/** Update internal stuff - PROCESS */
		virtual void update(const float elapsedSeconds) = 0;
		/** Render - OUTPUT */
		virtual void render(const float elapsedSeconds) = 0;
		/** Unload resources */
		virtual void unload() = 0;
		/** Destroy the state */
		virtual void finalize() = 0;

		/** Called the first time we enter the state */
		void enter();
		/** Called when we exit from the state */
		void exit();
		/** Called when the state has to be paused */
		virtual void pause() = 0;
		/** Called when the state has to be resumed (from pause) */
		virtual void resume() = 0;

		/** Get state Id */
		virtual GameStateId getStateId() = 0;
		/** Get NEXT state Id */
		GameStateId getNextStateId() { return mNextGameStateId; };

		bool mIsPaused;

	protected:
		/** State is paused */
		//bool mIsPaused;
		/** State is active */
		bool mIsActive;
		/** State is dead */
		bool mIsDead;

	protected:
		/** Graphics manager */
		GraphicsManagerPtr mGraphicsManager;
		/** Input manager */
		InputManagerPtr mInputManager;
		/** Audio manager */
		AudioManagerPtr mAudioManager;
		/** Gui manager */
		GuiManagerPtr mGuiManager;
		/** Camera manager*/
		CameraManagerPtr mCameraManager;

		/** Next Game State Id */
		GameStateId mNextGameStateId;

		/** The GUI manager */
		GuiScreenPtr mGuiScreen;

		/** GUI options menu */
		GuiMenu*	mMenu;

	protected:
		Ogre::Root* mRoot;
		Ogre::RenderWindow* mWindow;
		Ogre::Camera* mCamera;
		Ogre::Viewport* mViewport;
		Ogre::SceneManager* mSceneManager;
	};
}
#endif // __BASE_STATE_H_
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
// Gui
//
#include "..\GUI\GuiScreen.h"
#include "..\GUI\GuiBackground.h"
#include "..\GUI\GuiButton.h"

namespace WyvernsAssault
{
	/** List of game states of the game */
	enum GameStateId
	{
		SplashScreen	= 0, // Splash screen
		Intro			= 1,
		MainMenu		= 2,
		Play			= 3,
		GameOver		= 4,
		Ending			= 5,
		Outro			= 6,
		Credits			= 7,
		Options			= 8,
		Exit			= 9 // Special state, it is just a flag to exit the application
	};

	/**
		Base game state class
	*/
	class BaseState : public InputListener, public FrameListener
	{
	public:
		BaseState();
		BaseState(GraphicsManager& graphicsManager, InputManager& inputManager, AudioManager& audioManager);
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

	protected:
		/** State is paused */
		bool mIsPaused;
		/** State is active */
		bool mIsActive;
		/** State is dead */
		bool mIsDead;

	protected:
		/** Graphics manager */
		GraphicsManager* mGraphicsManager;
		/** Input manager */
		InputManager* mInputManager;
		/** Audio manager */
		AudioManager* mAudioManager;

		/** Next Game State Id */
		GameStateId mNextGameStateId;

		/** The GUI manager */
		GuiScreen* mGuiScreen;

	protected:
		Ogre::Root* mRoot;
		Ogre::RenderWindow* mWindow;
		Ogre::Camera* mCamera;
		Ogre::Viewport* mViewport;
		Ogre::SceneManager* mSceneManager;
	};
}
#endif // __BASE_STATE_H_
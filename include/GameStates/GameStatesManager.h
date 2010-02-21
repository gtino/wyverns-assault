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
#ifndef __GAME_STATES_MANAGER_H_
#define __GAME_STATES_MANAGER_H_

#include <Ogre.h>

#include <map>

//
// Game States
//
#include "BaseState.h"
#include "SplashScreenState.h"
#include "IntroState.h"
#include "EndingState.h"
#include "GameoverState.h"
#include "MainMenuState.h"
#include "OutroState.h"
#include "PlayState.h"
#include "SplashScreenState.h"
#include "CreditsState.h"

//
// Audio
//
#include "..\Graphics\GraphicsManager.h"

//
// User input
//
#include "..\Input\InputManager.h"

//
// Audio
//
#include "..\Audio\AudioManager.h"

using namespace std;

namespace WyvernsAssault
{
	/** The games states map type */
	typedef std::map<GameStateId, BaseState*> GameStatesMap;
	typedef GameStatesMap::iterator GameStatesMapIterator;

	/** Game options */

	/** Game levels */

	/** Game score */

	/**
		Class used to manage the game states
	*/
	class GameStatesManager
	{
	public:
		GameStatesManager();
		~GameStatesManager();

	public:
		/** Initialize the manager */
		void initialize(GraphicsManager& graphicsManager, InputManager& gameInputManager, AudioManager& audioManager);
		/** Destroy and release all resources used by the manager */
		void finalize();
		/** Run the current state (main loop)*/
		void loop();

	private:
		/** Change state */
		void changeState(BaseState* newState);
		/** Retrive a game state reference by its ID */
		BaseState* getGameStateById(const GameStateId id);

	private:
		/** Map of game states */ 
		GameStatesMap mGameStates;

		/** Current (active) game state */
		BaseState* mCurrentState;
		/** Id of the current (active) game state */
		GameStateId mCurrentStateId;
	};
}

#endif // __GAME_STATES_MANAGER_H_
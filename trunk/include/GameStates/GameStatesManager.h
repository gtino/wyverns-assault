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

#include <map>

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

using namespace std;

namespace WyvernsAssault
{
	/** List of game states of the game */
	enum GameState
	{
		SplashScreen	= 0, // Splash screen
		Intro			= 1,
		MainMenu		= 2,
		Play			= 3,
		GameOver		= 4,
		Ending			= 5,
		Outro			= 6,
		Credits			= 7
	};

	/** The games states map type */
	typedef std::map<GameState, BaseState*> GameStatesMap;
	typedef GameStatesMap::iterator GameStatesMapIterator;

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
		void initialise();
		/** Destroy and release all resources used by the manager */
		void destroy();
		/** Run the current state */
		void run();

	private:
		/** Map of game states */ 
		GameStatesMap mGameStates;
		/** Current (active) game state */
		BaseState* mCurrentState;
	};
}

#endif // __GAME_STATES_MANAGER_H_
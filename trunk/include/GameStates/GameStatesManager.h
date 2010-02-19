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

#include <vector>

#include "BaseState.h"

using namespace std;

namespace WyvernsAssault
{
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

	private:
		std::vector<BaseState*> mGameStates;
	};
}

#endif // __GAME_STATES_MANAGER_H_
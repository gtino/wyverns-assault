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
#ifndef __GAME_H__
#define __GAME_H__

#include ".\Graphics\GraphicsManager.h"
#include ".\Input\GameInputManager.h"
#include ".\Audio\AudioManager.h"
#include ".\GameStates\GameStatesManager.h"

namespace WyvernsAssault
{
	class Game
	{
	public:
		//Constructor
		Game();
		//Destructor
		virtual ~Game();

		/** Initialize */ 
		void initialize();
		/** Main loop */
		void go(); 
		/** Finalize the game */
		void finalize();

	private:

		/** Graphics manager */
		GraphicsManager mGraphicsManager;
		/** Audio manager */
		AudioManager mAudioManager;
		/** Input manager */
		GameInputManager mInputManager;

		/** Game states (FSM) manager */
		GameStatesManager mGameStatesManager;
	};
}

#endif // __GAME_H__
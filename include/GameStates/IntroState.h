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
#ifndef __INTRO_STATE_H_
#define __INTRO_STATE_H_

#include "BaseState.h"

namespace WyvernsAssault
{
	/**
		Intro game state class
	*/
	class IntroState : public BaseState
	{
	public:
		IntroState(GraphicsManager& graphicsManager, InputManager& inputManager, AudioManager& audioManager);
		~IntroState();

	public:
		/** Initialize current state */
		void initialise();
		/** Load resources */
		void load();
		/** Manage input - INPUT */
		virtual void input();
		/** Update internal stuff - PROCESS */
		virtual void update();
		/** Render - OUTPUT */
		virtual void render();
		/** Unload resources */
		void unload();
		/** Destroy the state */
		void destroy();

		/** Get state Id */
		virtual GameStateId getStateId();
	};
}
#endif // __INTRO_STATE_H_
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
#ifndef __INPUT_LISTENER_H__
#define __INPUT_LISTENER_H__

#include <ois/OIS.h>

namespace WyvernsAssault
{
	/**
		Class used to listen to user input, from keyboard, mouse, joystick and so on...
	*/
	class InputListener
	{
	public:
		InputListener();
		~InputListener();

	public:
		//
		// Keyboard listeners
		//
		/** Buffered input - keyboard key clicked */
		virtual bool keyClicked(const OIS::KeyEvent& e){return true;}
		/** Buffered input - keyboard key clicked */
		virtual bool keyPressed(const OIS::KeyEvent& e){return true;}
		/** Buffered input - keyboard key clicked */
		virtual bool keyReleased(const OIS::KeyEvent& e){return true;}

		//
		// MouseListener
		//
		/** Buffered input - mouse moved */
		virtual bool mouseMoved(const OIS::MouseEvent &evt){return true;}
		/** Buffered input - mouse button pressed */
		virtual bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID){return true;}
		/** Buffered input - mouse button released */
		virtual bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID){return true;}
	
		//
		// JoyStickListener
		//
		/** Buffered input - joystick button pressed */
		virtual bool buttonPressed(const OIS::JoyStickEvent &evt, int index){return true;}
		/** Buffered input - joystick button released */
		virtual bool buttonReleased(const OIS::JoyStickEvent &evt, int index){return true;}
		/** Buffered input - axis pad moved */
		virtual bool axisMoved(const OIS::JoyStickEvent &evt, int index){return true;}
		/** Buffered input - pov moved */
		virtual bool povMoved(const OIS::JoyStickEvent &evt, int index){return true;}
	};
}

#endif //__INPUT_LISTENER_H__
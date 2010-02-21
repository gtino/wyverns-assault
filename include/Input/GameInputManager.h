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
#ifndef __INPUT_MANAGER_H_
#define __INPUT_MANAGER_H_

#include <ois/OIS.h>

#include <Ogre.h>
#include <OgreRenderWindow.h>

namespace Ogre
{
	class RenderWindow;
}

namespace WyvernsAssault
{
	/**
		Class used to manager user input, from keyboard, mouse, joystick and so on...
	*/
	class GameInputManager	: public OIS::MouseListener
							, public OIS::KeyListener
	{
	public:
		GameInputManager();
		~GameInputManager();

	public:
		/** Initialize the input manager */
		void initialize( Ogre::RenderWindow* window, bool showDefaultMousePointer );
		/** Finalize the input manager */
		void finalize();

		/** Acquire all inputs */
		void acquireAll();
		/** Relese all inputs */
		void unacquireAll();
		/** Read all user inputs */
		void capture();

		virtual void switchMouseMode();
		virtual void switchKeyMode();

		virtual bool mouseMoved( const OIS::MouseEvent& e );
		virtual bool mousePressed( const OIS::MouseEvent& e, OIS::MouseButtonID button );
		virtual bool mouseReleased( const OIS::MouseEvent& e, OIS::MouseButtonID button );

		virtual bool keyPressed( const OIS::KeyEvent& e );
		virtual bool keyReleased( const OIS::KeyEvent& e );

	protected:
		OIS::InputManager* mInputManager;
		OIS::Keyboard* mKeyboard;
		OIS::Mouse* mMouse;
		OIS::JoyStick* mJoyStick;
		
		bool mUseBufferedInputMouse;
		bool mUseBufferedInputKeys;
		bool mInputTypeSwitchingOn;
	};
}

#endif // __INPUT_MANAGER_H_
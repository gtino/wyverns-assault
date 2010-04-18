/*
-----------------------------------------------------------------------------
Wyverns' Assault 3D Videogame.
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
#ifndef __GUI_MANAGER_H_
#define __GUI_MANAGER_H_

#include <Ogre.h>
#include <OgreRenderWindow.h>

#include "..\Input\InputListener.h"

#include "GuiScreen.h"

using namespace Ogre;

namespace WyvernsAssault
{
	/** The widget map type */
	typedef std::map<GuiScreenId, GuiScreen*> GuiScreenMap;
	typedef GuiScreenMap::iterator GuiScreenMapIterator;

	/**
		Class used to deal with the graphics layer
	*/
	class GuiManager :  public InputListener
	{
	public:
		GuiManager(Ogre::Root* root, Ogre::SceneManager* sceneManager, Ogre::RenderWindow* window);
		~GuiManager();

	public:		
		bool initialize();		
		void finalize();

	private: // InputListener
		bool mouseMoved( const OIS::MouseEvent &arg );
		bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
		bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
		bool keyPressed( const OIS::KeyEvent &arg );
		bool keyReleased( const OIS::KeyEvent &arg );

	private:
		Root*			mRoot;
		RenderWindow*   mWindow;
		SceneManager*	mSceneManager;
	};
}

#endif // __GUI_MANAGER_H_
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

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "../../resources/resource.h"
#endif

#include <Ogre.h>
#include <OgreRenderWindow.h>
#include <CEGUI/CEGUI.h>
#include <OgreCEGUIRenderer.h>

//
// Defines
//
#define WYVERNS_ASSAULT_RESOURCES_FILE	".\\config\\resources.cfg"
#define WYVERNS_ASSAULT_PLUGINS_FILE	".\\config\\plugins.cfg"
#define WYVERNS_ASSAULT_CONFIG_FILE		".\\config\\config.cfg"
#define WYVERNS_ASSAULT_LOG_FILE		".\\log.log"

using namespace Ogre;

namespace WyvernsAssault
{
	/**
		Class used to deal with the graphics layer
	*/
	class GuiManager :  public FrameListener, public WindowEventListener
	{
	public:
		GuiManager();
		~GuiManager();

	public:
		/** Initialize CEGUI */
		bool initialize();
		/** Finalize CEGUI */
		void finalize();
		
		/** Load CEGUI resources */
		void loadResources();
		/** Unload CEGUI resources */
		void unloadResources();

		/** Render one frame */
		//bool renderOneFrame();

		/** Retrieve the render window */
		//Root*			getRoot(void);
		//RenderWindow*	getRenderWindow(void);
		//CEGUIManager*	getSceneManager(void);

	private:
		void chooseMenu(void);
		//bool configure(void);
		//void chooseSceneManager(void);
		//void createResourceListener(void);
		/** Setup resources (i.e. read and parse resources .cfg file) */
		//void setupResources();

	private:
		CEGUI::System*  mSystem;
		Root*			mRoot;
		RenderWindow*   mWindow;
		//RenderWindow*	mRenderWindow;
		//SceneManager*	mSceneManager;
	};
}

#endif // __GUI_MANAGER_H_
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
#ifndef __GRAPHICS_MANAGER_H_
#define __GRAPHICS_MANAGER_H_

#include <Ogre.h>
#include <OgreRenderWindow.h>

using namespace Ogre;

namespace WyvernsAssault
{
	/**
		Class used to deal with the graphics layer
	*/
	class GraphicsManager
	{
	public:
		GraphicsManager();
		~GraphicsManager();

	public:
		/** Initialize graphics */
		void initialize();
		/** Finalize graphics */
		void finalize();
		
		/** Load graphics resources */
		void loadResources();
		/** Unload graphics resources */
		void unloadResources();

		/** Retrieve the render window */
		RenderWindow* getRenderWindow();

	private:
		RenderWindow* mRenderWindow;
	};
}

#endif // __GRAPHICS_MANAGER_H_
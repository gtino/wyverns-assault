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
#ifndef __GUI_FLASHMOVIE_H_
#define __GUI_FLASHMOVIE_H_

#include <Ogre.h>
#include <OgreRenderWindow.h>

#include "OgrePanelOverlayElement.h"
#include "OgreTextAreaOverlayElement.h"

#include "GuiWidget.h"

#include "Hikari.h"

using namespace Ogre;

namespace WyvernsAssault
{
	class GuiFlashMovie : public GuiWidget
	{

	public:

	public:

		GuiFlashMovie(Ogre::Viewport* viewport, int screenId, Hikari::HikariManager* mHikari, Ogre::String resource);
		~GuiFlashMovie();

		void reset();

		void hide(){ fm->hide();}
		void show(){ fm->show();}

	protected:

		Hikari::HikariManager* mHikariManager;
		Hikari::FlashControl *fm;


	};

	typedef boost::shared_ptr<GuiFlashMovie> GuiFlashMoviePtr;
}

#endif // __GUI_FLASHCOUNTER_H_
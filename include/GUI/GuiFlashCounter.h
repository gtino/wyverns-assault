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
#ifndef __GUI_FLASHCOUNTER_H_
#define __GUI_FLASHCOUNTER_H_

#include <Ogre.h>
#include <OgreRenderWindow.h>

#include "OgrePanelOverlayElement.h"
#include "OgreTextAreaOverlayElement.h"

#include "GuiWidget.h"

#include "Hikari.h"

using namespace Ogre;

namespace WyvernsAssault
{
	class GuiFlashCounter : public GuiWidget
	{

	public:

		enum CounterTypes
		{
			Timer	= 0,
			Manual	= 1
		};

	public:

		GuiFlashCounter(Ogre::Viewport* viewport, int screenId, int seconds);
		~GuiFlashCounter();


		void finalize();
		void reset();

		bool update(const float elapsedSeconds);
		void setSeconds(Real seconds);
		Real getSeconds(){return mSeconds; }

	protected:

		Hikari::HikariManager* mHikariManager;
		Hikari::FlashControl *fc;

	protected:
		Real	mSeconds;


	};

	typedef boost::shared_ptr<GuiFlashCounter> GuiFlashCounterPtr;
}

#endif // __GUI_FLASHCOUNTER_H_
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
#ifndef __GUI_USERINTERFACE_H_
#define __GUI_USERINTERFACE_H_

#include <Ogre.h>
#include <OgreRenderWindow.h>

#include "OgreFontManager.h"
#include "OgreBorderPanelOverlayElement.h"
#include "OgreTextAreaOverlayElement.h"

#include "GuiWidget.h"

using namespace Ogre;

namespace WyvernsAssault
{
	class GuiUserInterface : public GuiWidget
	{
	public:
		GuiUserInterface(Real aspectRatio, GuiWidgetId widgetId);
		~GuiUserInterface();

	public:
		GuiWidgetId getPlayer(){ return mWidgetId; }
		/** Hide and show interface */
		void show();
		void hide();

		/** Text area values function */
		Real getTextKills(){return mKills; }
		Real getTextPoints(){return mPoints; }
		void setTextKills(int value);		
		void setTextPoints(int value);

	protected:
		GuiWidgetId					mWidgetId;
		Overlay*					mOverlay;
		TextAreaOverlayElement*		mTextKills;
		TextAreaOverlayElement*		mTextPoints;

	protected:
		Real	mKills;
		Real	mPoints;
	};
}

#endif // __GUI_USERINTERFACE_H_
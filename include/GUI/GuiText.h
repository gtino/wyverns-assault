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
#ifndef __GUI_TEXT_H_
#define __GUI_TEXT_H_

#include <Ogre.h>
#include <OgreRenderWindow.h>

#include "GuiWidget.h"

using namespace Ogre;

namespace WyvernsAssault
{
	class GuiText : public GuiWidget
	{
	public:
		GuiText();
		~GuiText();

	public:
		void setText(Ogre::String& text);
		Ogre::String& getText(void);
		void setColor(Ogre::ColourValue color);
		Ogre::ColourValue getColor(void);
		void setFont(Ogre::String& fontName);
		Ogre::String& getFont(void);

	protected:
		Ogre::String mText;
		Ogre::ColourValue mColor;
		Ogre::String mFont;
	};
}

#endif // __GUI_TEXT_H_
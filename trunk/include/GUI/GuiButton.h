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
#ifndef __GUI_BUTTON_H_
#define __GUI_BUTTON_H_

#include <Ogre.h>
#include <OgreRenderWindow.h>

#include "GuiWidget.h"
#include "GuiImage.h"

using namespace Ogre;

namespace WyvernsAssault
{
	class GuiButton : public GuiWidget
	{
	public:
		GuiButton();
		~GuiButton();

	public:
		void setImageNormal(Ogre::String image);
		void setImageDown(Ogre::String image);
		void setImageSelected(Ogre::String image);
		void setImageDisabled(Ogre::String image);

	protected:
		Ogre::SceneNode* mButtonNode;
		Ogre::Rectangle2D* mButtonRectangle;

		GuiImage* mNormalImage;
		GuiImage* mDownImage;
		GuiImage* mSelectedImage;
		GuiImage* mDisabledImage;
	};
}

#endif // __GUI_BUTTON_H_
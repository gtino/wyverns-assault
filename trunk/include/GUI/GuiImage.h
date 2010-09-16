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
#ifndef __GUI_IMAGE_H_
#define __GUI_IMAGE_H_

#include <Ogre.h>
#include <OgreRenderWindow.h>

#include "GuiWidget.h"

using namespace Ogre;

namespace WyvernsAssault
{
	class GuiImage : public GuiWidget
	{
	public:
		GuiImage();
		~GuiImage();

	public:
		void setImage(const Ogre::String& filename, const Ogre::String& name, const Ogre::String& group);
		void setImage(const Ogre::String& material);

		int getFrames(){ mRectangle2D->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(0)->getNumFrames(); }
		void setFrame(int frame){ mRectangle2D->getMaterial()->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setCurrentFrame(frame); }

	protected:
		Ogre::SceneNode* mImageNode;
		Ogre::MaterialPtr mImageMaterial;
	};
}

#endif // __GUI_IMAGE_H_
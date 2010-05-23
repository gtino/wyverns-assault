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
#ifndef __GUI_OBJECT_TEXT_DISPLAY_H_
#define __GUI_OBJECT_TEXT_DISPLAY_H_

#include <Ogre.h>

using namespace Ogre;

namespace WyvernsAssault
{
	class GuiObjectTextDisplay {

	public:
		GuiObjectTextDisplay(const Ogre::MovableObject* p, const Ogre::Camera* c);
		virtual ~GuiObjectTextDisplay();

		void enable(bool enable);
		void setText(const Ogre::String& text);

		void update();

	protected:
		const Ogre::MovableObject* m_p;
		const Ogre::Camera* m_c;
		bool m_enabled;
		Ogre::Overlay* m_pOverlay;
		Ogre::OverlayElement* m_pText;
		Ogre::OverlayContainer* m_pContainer;
		Ogre::String m_text;
	};
}
#endif // __GUI_OBJECT_TEXT_DISPLAY_H_
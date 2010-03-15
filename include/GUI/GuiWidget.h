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
#ifndef __GUI_WIDGET_H_
#define __GUI_WIDGET_H_

#include <Ogre.h>
#include <OgreRenderWindow.h>

using namespace Ogre;

namespace WyvernsAssault
{
	typedef int GuiWidgetId;
	
	/** List of Credits states */
	enum GuiWidgetCreditsId
	{
		ReturnMenu = 0
	};

	/** List of Ending states */
	enum GuiWidgetEndingId
	{
	};

	/** List of GameOver states */
	enum GuiWidgetGameOverId
	{
	};

	/** List of Intro states */
	enum GuiWidgetIntroId
	{
		SkipIntro = 0
	};
	
	/** List of menu states */
	enum GuiWidgetMenuId
	{	
		PlayMenu	= 0,
		OptionsMenu = 1,
		CreditsMenu = 2,
		QuitMenu	= 3	
	};
	
	enum GuiWidgetPlayId
	{
	};

	enum GuiWidgetPlayScreenId
	{
	};

	struct GuiSize
	{
		float width;
		float heigth;
 	};

	struct GuiPosition
	{
		float x;
		float y;
	};

	class GuiWidget
	{
	public:
		GuiWidget();
		virtual ~GuiWidget() = 0;

		virtual GuiPosition getPosition(){return mPosition;};
		virtual GuiSize getSize(){return mSize;};
		virtual void setPosition(float position){mPosition.x = position; mPosition.y = position;};
		virtual void setSize(float size){mSize.width = size; mSize.heigth = size * 3;};

	protected:
		GuiPosition mPosition;
		GuiSize mSize;
	};
}

#endif // __GUI_WIDGET_H_
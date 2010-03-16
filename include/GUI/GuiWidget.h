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
		CreditsToMenu = 0
	};

	/** List of Ending states */
	enum GuiWidgetEndingId
	{
		EndingToMenu	= 0,
		EndingToCredits = 1
	};

	/** List of GameOver states */
	enum GuiWidgetGameoverId
	{
		ReturnMenu = 0,
		QuitGame   = 1
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
		UserInterface1 = 0,
		UserInterface2 = 1,
		UserInterface3 = 2,
		UserInterface4 = 3
	};

	enum GuiWidgetPlayScreenId
	{
		GoIntro = 0
	};

	enum GuiWidgetPauseId
	{
		PlayGame    = 0,
		OptionsGame = 1,
		QuitToMenu  = 2
	};

	struct GuiSize
	{
		Real width;
		Real heigth;
 	};

	struct GuiPosition
	{
		Real x;
		Real y;
	};

	class GuiWidget
	{
	public:
		GuiWidget();
		virtual ~GuiWidget() = 0;

		virtual GuiPosition getPosition(){return mPosition;};
		virtual GuiSize getSize(){return mSize;};
		virtual void setPosition(float positionX, float positionY);
		virtual void setSize(float sizeX, float sizeY);
		virtual Ogre::Rectangle2D* getRectangle2D();

	protected:
		virtual void initialize();
		virtual void finalize();
		/** Called to recalc rectangle corners */
		virtual void update();

	protected:
		GuiPosition mPosition;
		GuiSize mSize;

		Ogre::Rectangle2D* mRectangle2D;
	};
}

#endif // __GUI_WIDGET_H_
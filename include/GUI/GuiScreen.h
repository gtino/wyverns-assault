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
#ifndef __GUI_SCREEN_H_
#define __GUI_SCREEN_H_

#include <Ogre.h>
#include <OgreRenderWindow.h>

#include "GuiWidget.h"
#include "GuiBackground.h"
#include "GuiButton.h"
#include "GuiImage.h"
#include "GuiUserInterface.h"

#include "..\Input\InputListener.h"

using namespace Ogre;

namespace WyvernsAssault
{
	/** List of game states of the game */
	enum GuiScreenId
	{
		SplashScreenGui	= 0, // Splash screen
		IntroGui		= 1,
		MainMenuGui		= 2,
		PlayGui			= 3,
		GameOverGui		= 4,
		EndingGui		= 5,
		OutroGui		= 6,
		CreditsGui		= 7,
		PauseGui		= 8,
	};

	/** The widget map type */
	typedef std::map<GuiWidgetId, GuiWidget*> WidgetMap;
	typedef WidgetMap::iterator WidgetMapIterator;

	/** The widget list type */
	typedef std::vector<GuiWidget*> WidgetList;
	typedef WidgetList::iterator WidgetListIterator;

	class GuiScreen : public InputListener
	{
	public:
		GuiScreen(Ogre::SceneManager* sceneManager, GuiScreenId id, const Ogre::String& name);
		~GuiScreen();

		void setBackground(GuiBackground* background);

		void addWidget(GuiWidget* widget, GuiWidgetId widgetId);
		GuiWidget* getWidget(GuiWidgetId widgetId);
		void removeWidget(GuiWidgetId widgetId);
		void removeAllWidgets();
		// This will destroy whole Gui
		void removeGui();
		GuiWidget* nextWidget(GuiWidgetId widgetId);
		GuiWidget* previousWidget(GuiWidgetId widgetId);
		// Hide and show gui widgets
		void show();
		void hide();		

	public:
		//
		// Keyboard listeners
		//
		/** Buffered input - keyboard key clicked */
		bool keyClicked(const OIS::KeyEvent& e);
		/** Buffered input - keyboard key clicked */
		bool keyPressed(const OIS::KeyEvent& e);
		/** Buffered input - keyboard key clicked */
		bool keyReleased(const OIS::KeyEvent& e);

		//
		// MouseListener
		//
		/** Buffered input - mouse moved */
		bool mouseMoved(const OIS::MouseEvent &evt);
		/** Buffered input - mouse button pressed */
		bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID);
		/** Buffered input - mouse button released */
		bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID);
	
		//
		// JoyStickListener
		//
		/** Buffered input - joystick button pressed */
		bool buttonPressed(const OIS::JoyStickEvent &evt, int index);
		/** Buffered input - joystick button released */
		bool buttonReleased(const OIS::JoyStickEvent &evt, int index);
		/** Buffered input - axis pad moved */
		bool axisMoved(const OIS::JoyStickEvent &evt, int index);
		/** Buffered input - pov moved */
		bool povMoved(const OIS::JoyStickEvent &evt, int index);

	protected:
		Ogre::SceneManager* mSceneManager;

		WidgetMap mWidgetMap;

		GuiScreenId mGuiScreenId;
		Ogre::String mGuiScreenName;

		GuiBackground* mBackground;
		SceneNode* mBackgroundNode;

		GuiButton* mButton;
		GuiImage* mImage;
	};
}

#endif // __GUI_SCREEN_H_
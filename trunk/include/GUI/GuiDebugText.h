#ifndef __GUI_DEBUG_TEXT_H_
#define __GUI_DEBUG_TEXT_H_

#include <tchar.h>
#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#define WIN32_LEAN_AND_MEAN   
#define _WIN32_WINNT 0x0501
#include <windows.h> 

#include <Ogre.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgreFontManager.h> 

static float DEFAULT_SIZE = 0.03f;

using namespace Ogre;

namespace Ogre
{
	class TextAreaOverlayElement;
}

namespace WyvernsAssault
{
	class GuiDebugText
	{
	public:
		GuiDebugText();
		~GuiDebugText();

		void init();
		void finalize();
		// Print at <x/y>
		void print(float x, float y, const char*, ...);
		void print(float x, float y, float size, const char*, ...);
		// Begin printing at a certain coordinate
		void beginPrint(float x, float y, float size);
		// Continue printing (started with beginPrint). Each print will start in a new line.
		void print(const char*, ...);
		// Call this at the start of each frame
		void frameStarted();
		// Call this at the end of each frame before it is rendered!
		void update();
		// Change the color
		void setColor(ColourValue& color);
		// Show/hide toogle
		void toogle();

	private:
		struct Entry
		{
			Entry()
			{
				mX = 0;
				mY = 0;
				mSize = 1.0f;
			}
		float mX;
		float mY;
		float mSize;
		std::string mString;
		};

		typedef std::vector<Entry> Strings;
		ColourValue mColour;
		ResourcePtr mFont;
		Strings mStrings;
		Overlay* mOverlay;
		OverlayContainer* mPanel;
		enum { MAX_TEXT_AREAS = 50 };
		TextAreaOverlayElement* mTextAreas[MAX_TEXT_AREAS];

		struct Context
		{
			Context();
			float mX;
			float mY;
			float mSize;
		} mContext;
	};
}
#endif // !__GUI_DEBUG_TEXT_H_
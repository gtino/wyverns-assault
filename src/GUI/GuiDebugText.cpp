
#include "..\..\include\GUI\GuiDebugText.h"

using namespace Ogre;
using namespace WyvernsAssault;

//------------------------------------------------------------------------------
GuiDebugText::GuiDebugText() 
: mPanel(0)
//------------------------------------------------------------------------------
{
	mPanel = 0;
	mOverlay = 0;
	memset(mTextAreas, 0, MAX_TEXT_AREAS*sizeof(TextAreaOverlayElement*));
}

GuiDebugText::~GuiDebugText()
{
	finalize();
}

void GuiDebugText::finalize()
{
	if(mPanel)
	{
		OverlayManager::getSingleton().destroyOverlayElement(mPanel);
		mPanel = 0;
	}

	if(mOverlay)
	{
		OverlayManager::getSingleton().destroy(mOverlay);
		mOverlay = 0;
	}

	for (int i = 0; i<MAX_TEXT_AREAS; ++i)
	{
		if(mTextAreas[i] != 0)
		{
			OverlayManager::getSingleton().destroyOverlayElement(mTextAreas[i]);
			mTextAreas[i] = 0;
		}
	}
}

//------------------------------------------------------------------------------
void GuiDebugText::init()
//------------------------------------------------------------------------------
{
	// Create a panel
	mPanel = static_cast<OverlayContainer*>(
	OverlayManager::getSingleton().createOverlayElement("Panel", "PanelName"));
	mPanel->setMetricsMode(Ogre::GMM_RELATIVE);
	mPanel->setPosition(0, 0);
	mPanel->setDimensions(1.0f, 1.0f);
	//panel->setMaterialName("MaterialName"); // Optional background material

	for (int i = 0; i<MAX_TEXT_AREAS; ++i)
	{
		mTextAreas[i] = static_cast<TextAreaOverlayElement*>(
		OverlayManager::getSingleton().createOverlayElement("TextArea",           StringConverter::toString(i) + "TextAreaName"));
		mTextAreas[i]->setMetricsMode(Ogre::GMM_RELATIVE);
		mTextAreas[i]->setFontName("Fonts\GuiDebugText");
		mTextAreas[i]->setColourBottom(ColourValue(1,1,1));
		mTextAreas[i]->setColourTop(ColourValue(1,1,1));
		mTextAreas[i]->setDimensions(1.0f, 1.0f);
		mPanel->addChild(mTextAreas[i]);
	}
	mOverlay = OverlayManager::getSingleton().create("GuiDebugText");
	mOverlay->add2D(mPanel);
	// Show the overlay
	mOverlay->hide();
}

//------------------------------------------------------------------------------
void GuiDebugText::setColor(ColourValue& color)
//------------------------------------------------------------------------------
{
	mColour = color;
}

//------------------------------------------------------------------------------
void GuiDebugText::frameStarted()
//------------------------------------------------------------------------------
{
	mStrings.clear();
	for (int i = 0; i<MAX_TEXT_AREAS; ++i)
	{
		mTextAreas[i]->hide();
	}
}
//------------------------------------------------------------------------------
void GuiDebugText::update()
//------------------------------------------------------------------------------
{
	int count = 0;

	for (Strings::iterator i = mStrings.begin(); i!=mStrings.end() && count < MAX_TEXT_AREAS; ++i)
	{
		Entry& entry = *i;
		mTextAreas[count]->setPosition(entry.mX, entry.mY);
		mTextAreas[count]->setCaption(entry.mString.c_str());
		mTextAreas[count]->setColour(mColour);
		mTextAreas[count]->setCharHeight(DEFAULT_SIZE*entry.mSize);
		mTextAreas[count]->show();
		++count;
		assert(count < MAX_TEXT_AREAS);
	}   
}

namespace
{
	//------------------------------------------------------------------------------
	const char* vprint(const char* format, va_list args)
	//------------------------------------------------------------------------------
	{
	static const int BUFFER_SIZE = 200;
	static char buffer[BUFFER_SIZE];
	int size = _vscprintf(format, args);
	assert(size < BUFFER_SIZE);
	vsnprintf(buffer, BUFFER_SIZE, format, args);
	return buffer;
	}
}

//------------------------------------------------------------------------------
void GuiDebugText::print(float x, float y, const char* format, ...)
//------------------------------------------------------------------------------
{
	Entry entry;
	entry.mX = x;
	entry.mY = y;
	va_list args;
	va_start( args, format );   
	entry.mString = vprint(format, args);
	va_end( args );   
	mStrings.push_back(entry);
}
//------------------------------------------------------------------------------
void GuiDebugText::print(float x, float y, float size, const char* format, ...)
//------------------------------------------------------------------------------
{
	Entry entry;
	entry.mX = x;
	entry.mY = y;
	entry.mSize = size;
	va_list args;
	va_start( args, format );   
	entry.mString = vprint(format, args);
	va_end( args );   
	mStrings.push_back(entry);
}
//------------------------------------------------------------------------------
GuiDebugText::Context::Context()
//------------------------------------------------------------------------------
{
	mX = 0.0f;
	mY = 0.0f;
	mSize = 1.0f;
}
//------------------------------------------------------------------------------
void GuiDebugText::beginPrint(float x, float y, float size)
//------------------------------------------------------------------------------
{
	mContext.mX = x;
	mContext.mY = y;
	mContext.mSize = size;
}
//------------------------------------------------------------------------------
void GuiDebugText::print(const char* format, ...)
//------------------------------------------------------------------------------
{
	Entry entry;
	entry.mX = mContext.mX;
	entry.mY = mContext.mY;
	entry.mSize = mContext.mSize;
	mContext.mY += mContext.mSize*DEFAULT_SIZE;
	va_list args;
	va_start( args, format );   
	entry.mString = vprint(format, args);
	va_end( args );   
	mStrings.push_back(entry);
}
//------------------------------------------------------------------------------
void GuiDebugText::toogle()
{ 
	if(mOverlay->isVisible()) mOverlay->hide();
	else mOverlay->show();
}

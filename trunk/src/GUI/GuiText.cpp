#include "..\..\include\GUI\GuiText.h"

using namespace WyvernsAssault;

GuiText::GuiText()
{
	//
	// TODO Constructor
	//
}

GuiText::~GuiText()
{
	//
	// TODO Distructor logic HERE
	//
}

void GuiText::setText(Ogre::String& text)
{
	mText = text;
}

Ogre::String& GuiText::getText(void)
{
	return mText;
}

void GuiText::setColor(Ogre::ColourValue color)
{
	mColor = color;
}

Ogre::ColourValue GuiText::getColor(void)
{
	return mColor;
}

void GuiText::setFont(Ogre::String& fontName)
{
	mFont = fontName;
}

Ogre::String& GuiText::getFont(void)
{
	return mFont;
}
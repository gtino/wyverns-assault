#include "..\..\include\GUI\GuiButton.h"

using namespace WyvernsAssault;

GuiButton::GuiButton()
{
	//
	// TODO Constructor
	//
}

GuiButton::~GuiButton()
{
	//
	// TODO Distructor logic HERE
	//
}

void GuiButton::setImageNormal(Ogre::String image)
{
	mNormalImage->setImage(image, NULL, NULL);
}

void GuiButton::setImageDown(Ogre::String image)
{
	mDownImage->setImage(image, NULL, NULL);
}
void GuiButton::setImageSelected(Ogre::String image)
{
	mSelectedImage->setImage(image, NULL, NULL);
}

void GuiButton::setImageDisabled(Ogre::String image)
{
	mDisabledImage->setImage(image, NULL, NULL);
}
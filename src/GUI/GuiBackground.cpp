#include "..\..\include\GUI\GuiBackground.h"

using namespace WyvernsAssault;

GuiBackground::GuiBackground() : 
GuiImage()
{
	//
	// TODO Constructor
	//
}

GuiBackground::~GuiBackground()
{
	//
	// TODO Distructor logic HERE
	//
}

void GuiBackground::setImage(const Ogre::String& filename, const Ogre::String& name, const Ogre::String& group)
{
	GuiImage::setImage(filename,name,group);

	// Render the background before everything else
	mRectangle2D->setRenderQueueGroup(RENDER_QUEUE_SKIES_LATE);

	// Create background rectangle covering the whole screen
	mRectangle2D->setCorners(-1.0, 1.0, 1.0, -1.0);

	// Use infinite AAB to always stay visible
	AxisAlignedBox aabInf;
	aabInf.setInfinite();
	mRectangle2D->setBoundingBox(aabInf);
}

void GuiBackground::setImage(const Ogre::String& material)
{
	GuiImage::setImage(material);
	
	// Render the background before everything else
	mRectangle2D->setRenderQueueGroup(RENDER_QUEUE_OVERLAY);

	// Create background rectangle covering the whole screen
	mRectangle2D->setCorners(-1.0, 1.0, 1.0, -1.0);

	// Use infinite AAB to always stay visible
	AxisAlignedBox aabInf;
	aabInf.setInfinite();
	mRectangle2D->setBoundingBox(aabInf);
}
#include "..\..\include\GUI\GuiImage.h"

using namespace WyvernsAssault;

GuiImage::GuiImage() :
GuiWidget()
{
	//
	// TODO Constructor
	//
}

GuiImage::~GuiImage()
{
	//
	// TODO Distructor logic HERE
	//
}

void GuiImage::setImage(const Ogre::String& filename, const Ogre::String& name, const Ogre::String& group)
{
	// Create background material
	mImageMaterial = MaterialManager::getSingleton().create(name, group);
	mImageMaterial->getTechnique(0)->getPass(0)->createTextureUnitState(filename);
	mImageMaterial->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
	mImageMaterial->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
	mImageMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);

	// Create background rectangle covering the whole screen
	mImageRectangle = new Rectangle2D(true);
	mImageRectangle->setCorners(-1.0, 1.0, 1.0, -1.0);
	mImageRectangle->setMaterial(name);

	// Render the background before everything else
	mImageRectangle->setRenderQueueGroup(RENDER_QUEUE_BACKGROUND);

	// Use infinite AAB to always stay visible
	AxisAlignedBox aabInf;
	aabInf.setInfinite();
	mImageRectangle->setBoundingBox(aabInf);

	// Attach background to the scene
	//mImageNode = node;
	//mImageNode->attachObject(getImageRectangle());
}

Ogre::Rectangle2D* GuiImage::getImageRectangle()
{
	return mImageRectangle;
}

void GuiImage::setPosition(float x, float y)
{
	//mImageNode->setPosition(Vector3(x, y, 0));
}

void GuiImage::setSize(GuiSize size)
{

}

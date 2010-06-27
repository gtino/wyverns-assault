#include "..\..\include\GUI\GuiImage.h"

using namespace WyvernsAssault;

GuiImage::GuiImage() :
GuiWidget()
{
	// Render the background after everything else
	mRectangle2D->setRenderQueueGroup(RENDER_QUEUE_OVERLAY);

	// Use infinite AAB to always stay visible
	AxisAlignedBox aabInf;
	aabInf.setInfinite();
	mRectangle2D->setBoundingBox(aabInf);
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
	mImageMaterial->getTechnique(0)->getPass(0)->setDepthCheckEnabled(true);
	mImageMaterial->getTechnique(0)->getPass(0)->setDepthWriteEnabled(true);
	mImageMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);
	mImageMaterial->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);

	mRectangle2D->setMaterial(name);
}

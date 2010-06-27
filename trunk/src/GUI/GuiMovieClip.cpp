#include "..\..\include\GUI\GuiMovieClip.h"

using namespace WyvernsAssault;

GuiMovieClip::GuiMovieClip() :
GuiBackground()
{
	//
	// TODO Constructor
	//
}

GuiMovieClip::~GuiMovieClip()
{
	//
	// TODO Distructor logic HERE
	//
}

void GuiMovieClip::setMovieClip(const Ogre::String& name)
{
	// Create background material
	//mMovieClipMaterial = MaterialManager::getSingleton().getByName(name);
	//mMovieClipMaterial->getTechnique(0)->getPass(0)->createTextureUnitState(filename);
	//mMovieClipMaterial->getTechnique(0)->getPass(0)->setDepthCheckEnabled(true);
	//mMovieClipMaterial->getTechnique(0)->getPass(0)->setDepthWriteEnabled(true);
	//mMovieClipMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);
	//mMovieClipMaterial->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);

	mRectangle2D->setMaterial(name);

	// Render the background after everything else
	mRectangle2D->setRenderQueueGroup(RENDER_QUEUE_BACKGROUND);

	// Create background rectangle covering the whole screen
	mRectangle2D->setCorners(-1.0, 1.0, 1.0, -1.0);

	// Use infinite AAB to always stay visible
	AxisAlignedBox aabInf;
	aabInf.setInfinite();
	mRectangle2D->setBoundingBox(aabInf);
}

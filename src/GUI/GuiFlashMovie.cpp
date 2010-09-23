#include "..\..\include\GUI\GuiFlashMovie.h"

using namespace WyvernsAssault;
using namespace Hikari;

GuiFlashMovie::GuiFlashMovie(Ogre::Viewport* viewport, int screenId, Hikari::HikariManager* mHikari, Ogre::String resource) :
GuiWidget()
{
	
	mHikariManager = mHikari; 

	mFlashControl = mHikariManager->createFlashOverlay("FM"+resource, viewport, viewport->getActualWidth()/2, viewport->getActualHeight()/2, Position(Right));
	mFlashControl->load(resource);
	mFlashControl->setTransparent(true); 
	mFlashControl->setDraggable(false);

}

GuiFlashMovie::~GuiFlashMovie()
{
	mFlashControl = NULL;
}

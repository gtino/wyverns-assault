#include "..\..\include\GUI\GuiFlashMovie.h"

using namespace WyvernsAssault;
using namespace Hikari;

GuiFlashMovie::GuiFlashMovie(Ogre::Viewport* viewport, int screenId, Hikari::HikariManager* mHikari, Ogre::String resource) :
GuiWidget()
{
	
	mHikariManager = mHikari; 

	fm = mHikariManager->createFlashOverlay("FM"+resource, viewport, viewport->getActualWidth()/2, viewport->getActualHeight()/2, Position(Right));
	fm->load(resource);
	fm->setTransparent(true); 
	fm->setDraggable(false);

}

GuiFlashMovie::~GuiFlashMovie()
{
	//TODO
}

void GuiFlashMovie::reset(){
	fm->hide();
}


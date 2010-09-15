#include "..\..\include\GUI\GuiFlashCounter.h"

using namespace WyvernsAssault;
using namespace Hikari;

GuiFlashCounter::GuiFlashCounter(Ogre::Viewport* viewport, int screenId, int seconds) :
GuiWidget()
{

	mSeconds = seconds;
	
	mHikariManager = new HikariManager("\\data\\flash");

	fc = mHikariManager->createFlashOverlay("FPS", viewport, 130, 91, Position(TopRight));
	fc->load("GameTimer.swf");
	fc->setTransparent(true);
	fc->setDraggable(false);

	fc->callFunction("setTime", Args((int)mSeconds));

}

GuiFlashCounter::~GuiFlashCounter()
{
	//finalize();
}

void GuiFlashCounter::finalize(){
	if(mHikariManager)
		delete mHikariManager;

}

void GuiFlashCounter::reset(){
	mSeconds = 0;
	fc->hide();
}

void GuiFlashCounter::setSeconds(Real seconds){
	mSeconds = seconds;
	fc->show();
}

bool GuiFlashCounter::update(const float elapsedSeconds)
{
	mHikariManager->update();
	mSeconds = mSeconds - elapsedSeconds;
	fc->callFunction("setTime", Args((int)mSeconds));
	if(mSeconds <= 0){
		fc->hide();
		return false;
	}

	return true;
}



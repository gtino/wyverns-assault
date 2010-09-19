#include "..\..\include\GUI\GuiFlashCounter.h"

using namespace WyvernsAssault;
using namespace Hikari;

GuiFlashCounter::GuiFlashCounter(CounterTypes type, Ogre::Viewport* viewport, int screenId, int seconds, Hikari::HikariManager* mHikari) :
GuiWidget()
{

	mSeconds = seconds;
	mType = type;
	
	mHikariManager = mHikari; 

	if(type == CounterTypes::Manual){
		fc = mHikariManager->createFlashOverlay("FPS"+type, viewport, 150, 130, Position(TopRight)); 
		fc->load("GameEnemyCounter.swf");
	}
	else{
		fc = mHikariManager->createFlashOverlay("FPS"+type, viewport, 123, 90, Position(TopRight));
		fc->load("GameTimer.swf"); 
	}

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

bool GuiFlashCounter::update(const float elapsedSeconds, int enemyCount)
{
	mHikariManager->update();
	if(mType == CounterTypes::Timer){
		mSeconds = mSeconds - elapsedSeconds;
		fc->callFunction("setTime", Args((int)mSeconds));
		if(mSeconds <= 0){
			fc->hide();
			return false;
		}
	}else{
		fc->callFunction("setTime", Args((int)enemyCount));
		fc->show();
	}

	return true;

}



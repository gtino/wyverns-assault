#include "..\..\include\GUI\GuiFlashCounter.h"

using namespace WyvernsAssault;
using namespace Hikari;

GuiFlashCounter::GuiFlashCounter(CounterTypes type, Ogre::Viewport* viewport, int screenId, int seconds, Hikari::HikariManager* mHikari) :
GuiWidget()
{

	mSeconds = seconds;
	mType = type;
	
	mHikariManager = mHikari; 

	if(type == CounterTypes::Manual)
	{
		mFlashControl = mHikariManager->createFlashOverlay("FPS"+type, viewport, 220, 160, Position(TopRight)); 
		mFlashControl->load("GameEnemyCounter.swf");
	}
	else
	{
		mFlashControl = mHikariManager->createFlashOverlay("FPS"+type, viewport, 225, 200, Position(TopRight));
		mFlashControl->load("GameTimer.swf"); 
	}

	mFlashControl->setTransparent(true); 
	mFlashControl->setDraggable(false);

	mFlashControl->callFunction("setTime", Args((int)mSeconds)); 
}

GuiFlashCounter::~GuiFlashCounter()
{
	mSeconds = 0.0;
	mFlashControl = NULL;
}

void GuiFlashCounter::setSeconds(Real seconds)
{
	mSeconds = seconds;
	mFlashControl->show();
}

bool GuiFlashCounter::update(const float elapsedSeconds, int enemyCount)
{

	if(mType == CounterTypes::Timer)
	{
		mSeconds = mSeconds - elapsedSeconds;
		mFlashControl->callFunction("setTime", Args((int)mSeconds));

		if(mSeconds <= 0)
		{
			mFlashControl->hide();
			return false;
		}
	}
	else
	{
		mFlashControl->callFunction("setTime", Args((int)enemyCount));
	}

	return true;
}

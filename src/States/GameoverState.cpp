#include "..\include\States\GameoverState.h"

using namespace WyvernsAssault;

GameoverState::GameoverState(GraphicsManagerPtr graphicsManager, 
							 InputManagerPtr inputManager, 
							 AudioManagerPtr audioManager, 
							 CameraManagerPtr cameraManager, 
							 GuiManagerPtr guiManager, 
							 LevelManagerPtr levelManager)
: BaseState(graphicsManager,inputManager,audioManager, cameraManager, guiManager, levelManager)
{
	//
	// TODO Constructor logic HERE
	//
}

GameoverState::~GameoverState()
{
	//
	// TODO Distructor logic HERE
	//
	exit();
}

/** Initialize current state */
void GameoverState::initialize()
{
	BaseState::initialize();
}

/** Manage input */
void GameoverState::input()
{
	//
	// TODO Read input
	//
	this->mInputManager->capture();
}

/** Load resources */
void GameoverState::load()
{
	//
	// Gui Screen for this state
	//
	mGuiScreen = mGuiManager->createScreen(GuiScreenId::GameOverGui, "GameOverScreen");
	
	GuiBackgroundPtr guiBackground = GuiBackgroundPtr(new GuiBackground());

	// Common aspect ratio is 4/3
	if( mWindow->getViewport(0)->getCamera()->getAspectRatio() > 1.34 )
		guiBackground->setImage("wide/GameOver.png","GameOverBackground","General");
	else
		guiBackground->setImage("normal/GameOver.png","GameOverBackground","General");

	mGuiScreen->setBackground(guiBackground);

	// Gui Widgets for this state
	mMenu = new GuiMenu(mWindow->getViewport(0)->getCamera()->getAspectRatio(), GuiScreenId::GameOverGui);
	
	// Add menu to screen
	mGuiScreen->addMenu(mMenu);

	//
	// Audio manager
	//
	mAudioManager->playSoundTrack("GameOver.mp3");
}

/** Update internal stuff */
void GameoverState::update(const float elapsedSeconds)
{
	//
	// TODO Update
	//
}

/** Render */
void GameoverState::render(const float elapsedSeconds)
{
	//
	// TODO Render
	//
}

/** Unload resources */
void GameoverState::unload() 
{
	if(mGuiScreen)
	{
		//
		// Remove gui
		//
		mGuiManager->removeScreen(GuiScreenId::GameOverGui);
		mGuiScreen.reset();
	}
}

/** Destroy the state */
void GameoverState::finalize()
{
	BaseState::finalize();
}

/** Get state Id */
GameStateId GameoverState::getStateId()
{
	//
	// BEWARE! Make sure this is the correspondent state or we fuck up everything!
	//
	return GameStateId::GameOver;
}

/** Called when the state has to be paused */
void GameoverState::pause()
{
	//
	// TODO : Pause state
	//
}

/** Called when the state has to be resumed (from pause) */
void GameoverState::resume()
{
	//
	// TODO : Resume state
	//
}

/** Buffered input - keyboard key clicked */
bool GameoverState::keyReleased(const OIS::KeyEvent& e)
{
	switch(e.key)
	{
	case OIS::KC_LEFT:
		mMenu->previousOption();
		mAudioManager->playSound("ChangeOption.wav", -2);
		break;
	case OIS::KC_RIGHT:
		mMenu->nextOption();
		mAudioManager->playSound("ChangeOption.wav", -2);
		break;
	case OIS::KC_RETURN:
		switch(mMenu->getCurrentOption())
		{
		case GuiWidgetGameoverId::ReturnMenu:
			this->mNextGameStateId = GameStateId::SplashScreen;
			mAudioManager->playSound("Select.wav", -1);
			break;
		case GuiWidgetGameoverId::QuitGame:
			this->mNextGameStateId = GameStateId::Exit;
			mAudioManager->playSound("Select.wav", -1);
			// Unload gui NOW!
			unload();
			break;
		}		
		break;
	}

	return true;
}
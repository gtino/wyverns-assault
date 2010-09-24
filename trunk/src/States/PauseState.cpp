#include "..\include\States\PauseState.h"

using namespace WyvernsAssault;

PauseState::PauseState(GraphicsManagerPtr graphicsManager, 
					   InputManagerPtr inputManager, 
					   AudioManagerPtr audioManager, 
					   CameraManagerPtr cameraManager, 
					   GuiManagerPtr guiManager, 
					   LevelManagerPtr levelManager)
: BaseState(graphicsManager,inputManager,audioManager, cameraManager, guiManager, levelManager)
{
	guiBackground = GuiBackgroundPtr(new GuiBackground());
}

PauseState::~PauseState()
{
	//
	// TODO Destructor
	//
	finalize();
}

/** Initialize current state */
void PauseState::initialize()
{
	BaseState::initialize();
}

/** Load resources */
void PauseState::load()
{
	//
	// Gui Screen for this state
	//
	mGuiScreen = mGuiManager->createScreen(GuiScreenId::PauseGui, "PauseScreen");
	
	// Common aspect ratio is 4/3
	if( mWindow->getViewport(0)->getCamera()->getAspectRatio() > 1.34 )
	{
		if( mLevelManager->getCurrentLevelIndex() == 1 )
			guiBackground->setImage("wide/PauseBoss.png","PauseBossBackground","General");
		else
			guiBackground->setImage("wide/Pause.png","PauseBackground","General");
	}
	else
	{
		if( mLevelManager->getCurrentLevelIndex() == 1 )
			guiBackground->setImage("normal/PauseBoss.png","PauseBossBackground","General");
		else
			guiBackground->setImage("normal/Pause.png","PauseBackground","General");
	}

	mGuiScreen->setBackground(guiBackground);

	// Gui Widgets for this state
	mMenu = new GuiMenu(mWindow->getViewport(0)->getCamera()->getAspectRatio(), GuiScreenId::PauseGui);
	
	// Add menu to screen
	mGuiScreen->addMenu(mMenu);
}

/** Manage input */
void PauseState::input()
{
	//
	// TODO Read input
	//
	this->mInputManager->capture();
}

/** Update internal stuff */
void PauseState::update(const float elapsedSeconds)
{
	//
	// TODO update
	//
}

/** Render */
void PauseState::render(const float elapsedSeconds)
{
	//
	// TODO Render
	//
}

/** Unload resources */
void PauseState::unload() 
{
	if(mGuiScreen)
	{
		//
		// Remove gui
		//
		mGuiScreen->removeGui();
	}
}

/** Destroy the state */
void PauseState::finalize()
{
	// Destroy gui
	if(mGuiScreen)
	{
		mGuiManager->removeScreen(GuiScreenId::PauseGui);
		mGuiScreen.reset();
	}

	BaseState::finalize();
}

/** Get state Id */
GameStateId PauseState::getStateId()
{
	//
	// BEWARE! Make sure this is the correspondent state or we fuck up everything!
	//
	return GameStateId::Pause;
}

/** Called when the state has to be paused */
void PauseState::pause()
{
	//
	// Hide gui without removing it
	//
	mGuiScreen->hide();
}

/** Called when the state has to be resumed (from pause) */
void PauseState::resume()
{
	//
	// Set next state to this state (Play)	
	//
	this->mNextGameStateId = this->getStateId();

	mGuiScreen->show();
}

/** Buffered input - keyboard key clicked */
bool PauseState::keyReleased(const OIS::KeyEvent& e)
{
	switch(e.key)
	{
	case OIS::KC_UP:
		mMenu->previousOption();
		mAudioManager->playSound("ChangeOption.wav", -2);
		break;
	case OIS::KC_DOWN:
		mMenu->nextOption();
		mAudioManager->playSound("ChangeOption.wav", -2);
		break;
	case OIS::KC_RETURN:
		switch(mMenu->getCurrentOption())
		{
		case GuiWidgetPauseId::PlayGame:
			this->mNextGameStateId = GameStateId::Play;
			mAudioManager->playSound("Select.wav", -1);
			break;
		case GuiWidgetPauseId::OptionsGame:
			this->mNextGameStateId = GameStateId::Options;
			mAudioManager->playSound("Select.wav", -1);
			break;
		case GuiWidgetPauseId::QuitToMenu:
			this->mNextGameStateId = GameStateId::MainMenu;
			mAudioManager->playSound("Select.wav", -1);			
			break;
		}		
		break;
	}

	return true;
}
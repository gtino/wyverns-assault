#include "..\include\States\MainMenuState.h"
using namespace WyvernsAssault;

MainMenuState::MainMenuState(GraphicsManagerPtr graphicsManager, InputManagerPtr inputManager, AudioManagerPtr audioManager, CameraManagerPtr cameraManager, GuiManagerPtr guiManager)
: BaseState(graphicsManager,inputManager,audioManager, cameraManager, guiManager)
{
	//
	// TODO Constructor logic HERE
	//
	this->mNextGameStateId = this->getStateId();
}

MainMenuState::~MainMenuState()
{
	//
	// TODO Distructor logic HERE
	//
	finalize();
}

/** Initialize current state */
void MainMenuState::initialize()
{
	BaseState::initialize();
}

/** Manage input */
void MainMenuState::input()
{
	//
	// TODO Read input
	//
	this->mInputManager->capture();
}

/** Load resources */
void MainMenuState::load()
{
	//
	// Gui Screen for this state
	//
	mGuiScreen = mGuiManager->createScreen(GuiScreenId::MainMenuGui, "MainMenuScreen");
	
	GuiBackgroundPtr guiBackground = GuiBackgroundPtr(new GuiBackground());
	guiBackground->setImage("MainMenu.png","MainMenuBackground","General");

	mGuiScreen->setBackground(guiBackground);

	// Gui Widgets for this state
	mMenu = new GuiMenu(mWindow->getViewport(0)->getCamera()->getAspectRatio(), GuiScreenId::MainMenuGui);
	
	// Add menu to screen
	mGuiScreen->addMenu(mMenu);

	//
	// Play soft soundtrack
	//
	mAudioManager->playSoundTrack("hard_track.mp3");
}

/** Update internal stuff */
void MainMenuState::update(const float elapsedSeconds)
{
	//
	// TODO Update
	//
}

/** Render */
void MainMenuState::render(const float elapsedSeconds)
{
	//
	// TODO Render
	//
}

/** Unload resources */
void MainMenuState::unload() 
{
	if(mGuiScreen)
	{
		//
		// Remove gui
		//
		mGuiManager->removeScreen(GuiScreenId::MainMenuGui);
		mGuiScreen.reset();
	}
}

/** Destroy the state */
void MainMenuState::finalize()
{
	BaseState::finalize();
}

/** Get state Id */
GameStateId MainMenuState::getStateId()
{
	//
	// BEWARE! Make sure this is the correspondent state or we fuck up everything!
	//
	return GameStateId::MainMenu;
}

/** Called when the state has to be paused */
void MainMenuState::pause()
{
	mGuiScreen->hide();
}

/** Called when the state has to be resumed (from pause) */
void MainMenuState::resume()
{
	//
	// Set next state to this state	
	//
	this->mNextGameStateId = this->getStateId();

	mGuiScreen->show();
}

/** Buffered input - keyboard key clicked */
bool MainMenuState::keyReleased(const OIS::KeyEvent& e)
{
	switch(e.key)
	{
	case OIS::KC_UP:
		mMenu->previousOption();
		break;
	case OIS::KC_DOWN:
		mMenu->nextOption();
		break;
	case OIS::KC_RETURN:
		switch(mMenu->getCurrentOption())
		{
		case GuiWidgetMenuId::PlayMenu:
			this->mNextGameStateId = GameStateId::LevelLoading;
			break;
		case GuiWidgetMenuId::OptionsMenu:
			this->mNextGameStateId = GameStateId::Options;
			break;
		case GuiWidgetMenuId::CreditsMenu:
			this->mNextGameStateId = GameStateId::Credits;
			break;
		case GuiWidgetMenuId::QuitMenu:
			this->mNextGameStateId = GameStateId::Exit;
			break;
		}		
		break;
	case OIS::KC_ESCAPE:
		this->mNextGameStateId = GameStateId::Exit;
		break;
	}

	return true;
}

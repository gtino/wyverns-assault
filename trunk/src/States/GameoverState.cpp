#include "..\include\States\GameoverState.h"

using namespace WyvernsAssault;

GameoverState::GameoverState(GraphicsManager& graphicsManager, InputManager& inputManager, AudioManager& audioManager)
: BaseState(graphicsManager,inputManager,audioManager)
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
	mGuiScreen = new GuiScreen(mSceneManager, GuiScreenId::GameOverGui, "GameOverScreen");
	
	GuiBackground* guiBackground = new GuiBackground();
	guiBackground->setImage("GameOver.png","GameOverBackground","General");

	mGuiScreen->setBackground(guiBackground);

	// Gui Widgets for this state
	mMenu = new GuiMenu(mWindow->getViewport(0)->getCamera()->getAspectRatio(), GuiScreenId::GameOverGui);
	
	// Add menu to screen
	mGuiScreen->addMenu(mMenu);

	//
	// Register the screen as input event listener, so it can receive events
	//
	mInputManager->addListener(mGuiScreen);
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
		// Remove gui listener
		//
		mInputManager->removeListener(mGuiScreen);
		//
		// Remove gui
		//
		mGuiScreen->removeGui();
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
		break;
	case OIS::KC_RIGHT:
		mMenu->nextOption();
		break;
	case OIS::KC_RETURN:
		switch(mMenu->getCurrentOption())
		{
		case GuiWidgetGameoverId::ReturnMenu:
			this->mNextGameStateId = GameStateId::MainMenu;
			break;
		case GuiWidgetGameoverId::QuitGame:
			this->mNextGameStateId = GameStateId::Exit;
			// Unload gui NOW!
			unload();
			break;
		}		
		break;
	}

	return true;
}
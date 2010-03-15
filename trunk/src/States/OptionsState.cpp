#include "..\include\States\OptionsState.h"

using namespace WyvernsAssault;

OptionsState::OptionsState(GraphicsManager& graphicsManager, InputManager& inputManager, AudioManager& audioManager)
: BaseState(graphicsManager,inputManager,audioManager)
{
	//
	// TODO Constructor logic HERE
	//
}

OptionsState::~OptionsState()
{
	//
	// TODO Distructor logic HERE
	//
	exit();
}

/** Initialize current state */
void OptionsState::initialize()
{
	BaseState::initialize();
}

/** Manage input */
void OptionsState::input()
{
	//
	// TODO Read input
	//
	this->mInputManager->capture();
}

/** Load resources */
void OptionsState::load()
{
	//
	// Gui Screen for this state
	//
	mGuiScreen = new GuiScreen(mSceneManager, GuiScreenId::IntroGui, "OptionsScreen");
	
	GuiBackground* guiBackground = new GuiBackground();
	guiBackground->setImage("Options.png","OptionsBackground","General");

	mGuiScreen->setBackground(guiBackground);

	//
	// Register the screen as input event listener, so it can receive events
	//
	mInputManager->addListener(mGuiScreen);
}

/** Update internal stuff */
void OptionsState::update(const float elapsedSeconds)
{
	//
	// TODO Update
	//
}

/** Render */
void OptionsState::render(const float elapsedSeconds)
{
	//
	// TODO Render
	//
}

/** Unload resources */
void OptionsState::unload() 
{
	if(mGuiScreen)
	{
		//
		// Register the screen as input event listener, so it can receive events
		//
		mInputManager->removeListener(mGuiScreen);

		delete mGuiScreen;
		mGuiScreen = 0;
	}
	//
	// TODO Unload
	//
}

/** Destroy the state */
void OptionsState::finalize()
{
	BaseState::finalize();
}

/** Get state Id */
GameStateId OptionsState::getStateId()
{
	//
	// BEWARE! Make sure this is the correspondent state or we fuck up everything!
	//
	return GameStateId::Options;
}

/** Called when the state has to be paused */
void OptionsState::pause()
{
	//
	// TODO : Pause state
	//
}

/** Called when the state has to be resumed (from pause) */
void OptionsState::resume()
{
	//
	// TODO : Resume state
	//
}

/** Buffered input - keyboard key clicked */
bool OptionsState::keyReleased(const OIS::KeyEvent& e)
{
	switch(e.key)
	{
	case OIS::KC_B:
		this->mNextGameStateId = GameStateId::MainMenu;
		break;
	}

	return true;
}
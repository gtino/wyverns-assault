#include "..\include\States\CreditsState.h"

using namespace WyvernsAssault;

CreditsState::CreditsState(GraphicsManagerPtr graphicsManager, 
						   InputManagerPtr inputManager, 
						   AudioManagerPtr audioManager, 
						   CameraManagerPtr cameraManager, 
						   GuiManagerPtr guiManager,
						   LevelManagerPtr levelManager)
: BaseState(graphicsManager,inputManager,audioManager, cameraManager, guiManager, levelManager)
{
	//
	// TODO Constructor
	//
}

CreditsState::~CreditsState()
{
	//
	// TODO Destructor
	//
	exit();
}

/** Initialize current state */
void CreditsState::initialize()
{
	BaseState::initialize();
}

/** Load resources */
void CreditsState::load()
{
	//
	// Gui Screen for this state
	//
	mGuiScreen = mGuiManager->createScreen(GuiScreenId::CreditsGui, "CreditsScreen");
	
	GuiBackgroundPtr guiBackground = GuiBackgroundPtr(new GuiBackground());
	guiBackground->setImage("Credits.png","CreditsBackground","General");

	mGuiScreen->setBackground(guiBackground);

	// Gui Widgets for this state
	mMenu = new GuiMenu(mWindow->getViewport(0)->getCamera()->getAspectRatio(), GuiScreenId::CreditsGui);
	
	// Add menu to screen
	mGuiScreen->addMenu(mMenu);
}

/** Manage input */
void CreditsState::input()
{
	//
	// TODO Read input
	//
	this->mInputManager->capture();
}

/** Update internal stuff */
void CreditsState::update(const float elapsedSeconds)
{
	//
	// TODO Update
	//
}

/** Render */
void CreditsState::render(const float elapsedSeconds)
{
	//
	// TODO Render
	//
}

/** Unload resources */
void CreditsState::unload() 
{
	if(mGuiScreen)
	{
		//
		// Remove gui
		//
		mGuiManager->removeScreen(GuiScreenId::CreditsGui);
		mGuiScreen.reset();
	}
}

/** Destroy the state */
void CreditsState::finalize()
{
	BaseState::finalize();
}

/** Get state Id */
GameStateId CreditsState::getStateId()
{
	//
	// BEWARE! Make sure this is the correspondent state or we fuck up everything!
	//
	return GameStateId::Credits;
}

/** Called when the state has to be paused */
void CreditsState::pause()
{
	//
	// TODO : Pause state
	//
}

/** Called when the state has to be resumed (from pause) */
void CreditsState::resume()
{
	//
	// TODO : Resume state
	//
}

/** Buffered input - keyboard key clicked */
bool CreditsState::keyReleased(const OIS::KeyEvent& e)
{
	switch(e.key)
	{
	case OIS::KC_RETURN:
		this->mNextGameStateId = GameStateId::MainMenu;
		break;
	}

	return true;
}
#include "..\include\States\CreditsState.h"

using namespace WyvernsAssault;

CreditsState::CreditsState(GraphicsManager& graphicsManager, InputManager& inputManager, AudioManager& audioManager)
: BaseState(graphicsManager,inputManager,audioManager)
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
	mGuiScreen = new GuiScreen(mSceneManager, GuiScreenId::CreditsGui, "CreditsScreen");
	//mGuiScreen = new GuiScreen(mSceneManager, GuiScreenId::IntroGui, "CreditsScreen");
	
	GuiBackground* guiBackground = new GuiBackground();
	guiBackground->setImage("Credits.png","CreditsBackground","General");

	mGuiScreen->setBackground(guiBackground);

	// Gui Widgets for this state
	GuiButton* menuButton = new GuiButton();
	menuButton->setSize(0.20, 0.20);
	menuButton->setPosition(0.80, 0.50);
	//menuButton->setImageNormal("MenuButton.png");
	//menuButton->setImageDown("MenuButtonDown.png");
	mGuiScreen->addWidget(menuButton,GuiWidgetCreditsId::CreditsToMenu);

	//
	// Register the screen as input event listener, so it can receive events
	//
	mInputManager->addListener(mGuiScreen);
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
	case OIS::KC_M:
		this->mNextGameStateId = GameStateId::MainMenu;
		break;
	case OIS::KC_ESCAPE:
		this->mNextGameStateId = GameStateId::Exit;
		break;
	}

	return true;
}
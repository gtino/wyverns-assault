#include "..\include\States\EndingState.h"

using namespace WyvernsAssault;

EndingState::EndingState(GraphicsManager& graphicsManager, InputManager& inputManager, AudioManager& audioManager)
: BaseState(graphicsManager,inputManager,audioManager)
{
	//
	// TODO Constructor logic HERE
	//
}

EndingState::~EndingState()
{
	//
	// TODO Distructor logic HERE
	//
	exit();
}

/** Initialize current state */
void EndingState::initialize()
{
	BaseState::initialize();
}

/** Load resources */
void EndingState::load()
{
	//
	// Gui Screen for this state
	//
	mGuiScreen = new GuiScreen(mSceneManager, GuiScreenId::IntroGui, "EndingScreen");
	
	GuiBackground* guiBackground = new GuiBackground();
	guiBackground->setImage("Ending.png","EndingBackground","General");

	mGuiScreen->setBackground(guiBackground);
	
	// Gui Widgets for this state
	GuiButton* menuButton = new GuiButton();
	menuButton->setSize(0.20);
	menuButton->setPosition(0.20);
	//menuButton->setImageNormal("MenuButton.png");
	//menuButton->setImageDown("MenuButtonDown.png");
	mGuiScreen->addWidget(menuButton,GuiWidgetEndingId::EndingToMenu);
	
	GuiButton* creditsButton = new GuiButton();
	creditsButton->setSize(0.20);
	creditsButton->setPosition(0.60);
	//creditsButton->setImageNormal("CreditsButton.png");
	//creditsButton->setImageDown("CreditsButtonDown.png");
	mGuiScreen->addWidget(creditsButton,GuiWidgetEndingId::EndingToCredits);
	//
	// Register the screen as input event listener, so it can receive events
	//
	mInputManager->addListener(mGuiScreen);
}

/** Manage input */
void EndingState::input()
{
	//
	// TODO Read input
	//
	this->mInputManager->capture();
}

/** Update internal stuff */
void EndingState::update(const float elapsedSeconds)
{
	//
	// TODO Update
	//
}

/** Render */
void EndingState::render(const float elapsedSeconds)
{
	//
	// TODO Render
	//
}

/** Unload resources */
void EndingState::unload() 
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
void EndingState::finalize()
{
	BaseState::finalize();
}

/** Get state Id */
GameStateId EndingState::getStateId()
{
	//
	// BEWARE! Make sure this is the correspondent state or we fuck up everything!
	//
	return GameStateId::Ending;
}

/** Called when the state has to be paused */
void EndingState::pause()
{
	//
	// TODO : Pause state
	//
}

/** Called when the state has to be resumed (from pause) */
void EndingState::resume()
{
	//
	// TODO : Resume state
	//
}

/** Buffered input - keyboard key clicked */
bool EndingState::keyReleased(const OIS::KeyEvent& e)
{
	switch(e.key)
	{
	case OIS::KC_M:
		this->mNextGameStateId = GameStateId::MainMenu;
		break;
	case OIS::KC_C:
		this->mNextGameStateId = GameStateId::Credits;
		break;
	}

	return true;
}
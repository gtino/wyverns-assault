#include "..\include\States\MainMenuState.h"
using namespace WyvernsAssault;

MainMenuState::MainMenuState(GraphicsManager& graphicsManager, InputManager& inputManager, AudioManager& audioManager)
: BaseState(graphicsManager,inputManager,audioManager)
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
	mGuiScreen = new GuiScreen(mSceneManager, GuiScreenId::IntroGui, "MainMenuScreen");
	
	GuiBackground* guiBackground = new GuiBackground();
	guiBackground->setImage("MainMenu.png","MainMenuBackground","General");

	mGuiScreen->setBackground(guiBackground);
	
	// Gui Widgets for this state
	GuiButton* playButton = new GuiButton();
	playButton->setSize(0.20);
	playButton->setPosition(0.20);
	//playButton->setImageNormal("Play.png");
	//playButton->setImageDown("PlayDown.png");
	mGuiScreen->addWidget(playButton,GuiWidgetMenuId::PlayMenu);
	
	GuiButton* optionsButton = new GuiButton();
	optionsButton->setSize(0.20);
	optionsButton->setPosition(0.20);
	//optionsButton->setImageNormal("OptionsButton.png");
	//optionsButton->setImageDown("OptionsDown.png");
	mGuiScreen->addWidget(optionsButton,GuiWidgetMenuId::OptionsMenu);
	
	GuiButton* creditsButton = new GuiButton();
	creditsButton->setSize(0.20);
	creditsButton->setPosition(0.20);
	//creditsButton->setImageNormal("Credits.png");
	//creditsButton->setImageDown("CreditsDown.png");
	mGuiScreen->addWidget(creditsButton,GuiWidgetMenuId::CreditsMenu);
	
	GuiButton* quitButton = new GuiButton();
	quitButton->setSize(0.20);
	quitButton->setPosition(0.20);
	//quitButton->setImageNormal("Quit.png");
	//quitButton->setImageDown("QuitDown.png");
	mGuiScreen->addWidget(quitButton,GuiWidgetMenuId::QuitMenu);

	//
	// Register the screen as input event listener, so it can receive events
	//
	mInputManager->addListener(mGuiScreen);
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
	//
	// TODO : Pause state
	//
}

/** Called when the state has to be resumed (from pause) */
void MainMenuState::resume()
{
	//
	// TODO : Resume state
	//
}

/** Buffered input - keyboard key clicked */
bool MainMenuState::keyReleased(const OIS::KeyEvent& e)
{
	switch(e.key)
	{
	case OIS::KC_P:
		this->mNextGameStateId = GameStateId::Play;
		break;
	case OIS::KC_E:
	case OIS::KC_ESCAPE:
		this->mNextGameStateId = GameStateId::Exit;
		break;
	}

	return true;
}

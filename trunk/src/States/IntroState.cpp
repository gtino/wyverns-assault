#include "..\include\States\IntroState.h"

using namespace WyvernsAssault;

IntroState::IntroState(GraphicsManager& graphicsManager, InputManager& inputManager, AudioManager& audioManager)
: BaseState(graphicsManager,inputManager,audioManager)
{
	//
	// TODO Constructor
	//
}

IntroState::~IntroState()
{
	//
	// TODO Destructor
	//
	exit();
}

/** Initialize current state */
void IntroState::initialize()
{
	BaseState::initialize();
}

/** Manage input */
void IntroState::input()
{
	//
	// TODO Read input
	//
	this->mInputManager->capture();
}

/** Load resources */
void IntroState::load()
{
	//
	// Gui Screen for this state
	//
	mGuiScreen = new GuiScreen(mSceneManager, GuiScreenId::IntroGui, "IntroScreen");
	
	GuiBackground* guiBackground = new GuiBackground();
	guiBackground->setImage("Intro.png","IntroBackground","General");

	mGuiScreen->setBackground(guiBackground);

	// Gui Widgets for this state
	GuiButton* skipButton = new GuiButton();
	skipButton->setSize(0.20, 0.20);
	skipButton->setPosition(0.70, 0.50);
	//playButton->setImageNormal("Skip.png");
	//playButton->setImageDown("SkipDown.png");
	mGuiScreen->addWidget(skipButton,GuiWidgetIntroId::SkipIntro);
	//
	// Register the screen as input event listener, so it can receive events
	//
	mInputManager->addListener(mGuiScreen);
}

/** Update internal stuff */
void IntroState::update(const float elapsedSeconds)
{
	//
	// TODO Update
	//
}

/** Render */
void IntroState::render(const float elapsedSeconds)
{
	//
	// TODO Render
	//
}

/** Unload resources */
void IntroState::unload() 
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
void IntroState::finalize()
{
	BaseState::finalize();
}

/** Get state Id */
GameStateId IntroState::getStateId()
{
	//
	// BEWARE! Make sure this is the correspondent state or we fuck up everything!
	//
	return GameStateId::Intro;
}

/** Called when the state has to be paused */
void IntroState::pause()
{
	//
	// TODO : Pause state
	//
}

/** Called when the state has to be resumed (from pause) */
void IntroState::resume()
{
	//
	// TODO : Resume state
	//
}

/** Buffered input - keyboard key clicked */
bool IntroState::keyReleased(const OIS::KeyEvent& e)
{
	switch(e.key)
	{
	case OIS::KC_ESCAPE:
		this->mNextGameStateId = GameStateId::Exit;
		break;
	}

	return true;
}

bool IntroState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID buttonId)
{
	switch(buttonId)
	{
		case OIS::MouseButtonID::MB_Left:
			this->mNextGameStateId = GameStateId::MainMenu;
			break;
	}

	return true;
}

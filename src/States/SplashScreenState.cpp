#include "..\include\States\SplashScreenState.h"

using namespace WyvernsAssault;

SplashScreenState::SplashScreenState(GraphicsManager& graphicsManager, InputManager& inputManager, AudioManager& audioManager)
: BaseState(graphicsManager,inputManager,audioManager)
{	
	//
	// TODO Constructor
	//
}

SplashScreenState::~SplashScreenState()
{
	//
	// TODO Destructor
	//
	finalize();
}

/** Initialize current state */
void SplashScreenState::initialize()
{
	BaseState::initialize();
}

/** Load resources */
void SplashScreenState::load()
{
	//
	// Gui Screen for this state
	//
	mGuiScreen = new GuiScreen(mSceneManager, GuiScreenId::IntroGui, "SplashScreenScreen");
	
	GuiBackground* guiBackground = new GuiBackground();
	guiBackground->setImage("SplashScreen.png","SplashScreenBackground","General");

	mGuiScreen->setBackground(guiBackground);
	
	// Gui Widgets for this state
	GuiButton* goIntro = new GuiButton();
	goIntro->setSize(0.20, 0.20);
	goIntro->setPosition(0.70, 0.40);
	//goIntro->setImageNormal("IntroButton.png");
	//goIntro->setImageDown("IntroButtonDown.png");
	mGuiScreen->addWidget(goIntro,GuiWidgetPlayScreenId::GoIntro);
	
	//
	// Register the screen as input event listener, so it can receive events
	//
	mInputManager->addListener(mGuiScreen);
}

/** Manage input */
void SplashScreenState::input()
{
	//
	// TODO Read input
	//
	this->mInputManager->capture();
}

/** Update internal stuff */
void SplashScreenState::update(const float elapsedSeconds)
{
	//
	// TODO Update
	//
}

/** Render */
void SplashScreenState::render(const float elapsedSeconds)
{
	//
	// TODO Render
	//
}

/** Unload resources */
void SplashScreenState::unload() 
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
void SplashScreenState::finalize()
{
	BaseState::finalize();
}

/** Get state Id */
GameStateId SplashScreenState::getStateId()
{
	//
	// BEWARE! Make sure this is the correspondent state or we fuck up everything!
	//
	return GameStateId::SplashScreen;
}

/** Called when the state has to be paused */
void SplashScreenState::pause()
{
	//
	// TODO : Pause state
	//
}

/** Called when the state has to be resumed (from pause) */
void SplashScreenState::resume()
{
	//
	// TODO : Resume state
	//
}

/** Buffered input - keyboard key clicked */
bool SplashScreenState::keyReleased(const OIS::KeyEvent& e)
{
	switch(e.key)
	{
	case OIS::KC_ESCAPE:
		this->mNextGameStateId = GameStateId::Exit;
		break;
	}

	return true;
}

bool SplashScreenState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID buttonId)
{
	switch(buttonId)
	{
		case OIS::MouseButtonID::MB_Left:
			this->mNextGameStateId = GameStateId::Intro;
			break;
	}

	return true;
}
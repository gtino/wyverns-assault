#include "..\include\States\SplashScreenState.h"

using namespace WyvernsAssault;

SplashScreenState::SplashScreenState(GraphicsManagerPtr graphicsManager, 
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
	mGuiScreen = mGuiManager->createScreen(GuiScreenId::SplashScreenGui, "SplashScreen");
	
	GuiBackgroundPtr guiBackground = GuiBackgroundPtr(new GuiBackground());
	guiBackground->setImage("SplashScreen.png","SplashBackground","General");

	mGuiScreen->setBackground(guiBackground);

	// Gui Widgets for this state
	mMenu = new GuiMenu(mWindow->getViewport(0)->getCamera()->getAspectRatio(), GuiScreenId::SplashScreenGui);
	
	// Add menu to screen
	mGuiScreen->addMenu(mMenu);

	//
	// Play soft soundtrack
	//
	mAudioManager->playSoundTrack("hard_track.mp3");
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
		// Remove gui
		//
		mGuiManager->removeScreen(GuiScreenId::SplashScreenGui);
		mGuiScreen.reset();
	}
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
	case OIS::KC_RETURN:
		this->mNextGameStateId = GameStateId::Intro;
		break;
	}

	return true;
}

bool SplashScreenState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID buttonId)
{
	/*switch(buttonId)
	{
		case OIS::MouseButtonID::MB_Left:
			this->mNextGameStateId = GameStateId::MainMenu;
			break;
	}*/

	return true;
}

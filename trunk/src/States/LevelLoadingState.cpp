#include "..\include\States\LevelLoadingState.h"

using namespace WyvernsAssault;

LevelLoadingState::LevelLoadingState(GraphicsManagerPtr graphicsManager, 
									 InputManagerPtr inputManager, 
									 AudioManagerPtr audioManager, 
									 CameraManagerPtr cameraManager, 
									 GuiManagerPtr guiManager, 
									 LevelManagerPtr levelManager)
: BaseState(graphicsManager,inputManager,audioManager, cameraManager, guiManager, levelManager)
, mLevel(0)
{
	//
	// TODO Constructor
	//
}

LevelLoadingState::~LevelLoadingState()
{
	//
	// TODO Destructor
	//
	exit();
}

/** Initialize current state */
void LevelLoadingState::initialize()
{
	BaseState::initialize();
}

/** Manage input */
void LevelLoadingState::input()
{
	//
	// TODO Read input
	//
	this->mInputManager->capture();
}

/** Load resources */
void LevelLoadingState::load()
{
	mAudioManager->stopSoundTrack();

	//
	// Gui Screen for this state
	//
	mGuiScreen = mGuiManager->createScreen(GuiScreenId::LevelLoadingGui, "LevelLoading");
	
	GuiBackgroundPtr guiBackground = GuiBackgroundPtr(new GuiBackground());
	guiBackground->setImage("LevelLoading.png","LevelLoadingBackground","General");

	mGuiScreen->setBackground(guiBackground);

	// Gui Widgets for this state
	mMenu = new GuiMenu(mWindow->getViewport(0)->getCamera()->getAspectRatio(), GuiScreenId::LevelLoadingGui);
	
	// Add menu to screen
	mGuiScreen->addMenu(mMenu);
}

/** Update internal stuff */
void LevelLoadingState::update(const float elapsedSeconds)
{
	//
	// HACK : This will make the game draw the 'Loading' screen once, and it will
	// be displayed during the whole loading process. It kinda sucks but is way
	// better than a black screen...
	//
	this->mNextGameStateId = GameStateId::Play;
}

/** Render */
void LevelLoadingState::render(const float elapsedSeconds)
{
	//
	// TODO Render
	//
}

/** Unload resources */
void LevelLoadingState::unload() 
{
	if(mGuiScreen)
	{
		//
		// Remove gui
		//
		mGuiManager->removeScreen(GuiScreenId::LevelLoadingGui);
		mGuiScreen.reset();
	}
}

/** Destroy the state */
void LevelLoadingState::finalize()
{
	BaseState::finalize();
}

/** Get state Id */
GameStateId LevelLoadingState::getStateId()
{
	//
	// BEWARE! Make sure this is the correspondent state or we fuck up everything!
	//
	return GameStateId::LevelLoading;
}

/** Called when the state has to be paused */
void LevelLoadingState::pause()
{
	//
	// TODO : Pause state
	//
}

/** Called when the state has to be resumed (from pause) */
void LevelLoadingState::resume()
{
	//
	// TODO : Resume state
	//
}

/** Buffered input - keyboard key clicked */
bool LevelLoadingState::keyReleased(const OIS::KeyEvent& e)
{
	switch(e.key)
	{
	case OIS::KC_RETURN:
		this->mNextGameStateId = GameStateId::Play;
		break;
	}

	return true;
}

bool LevelLoadingState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID buttonId)
{
	/*switch(buttonId)
	{
		case OIS::MouseButtonID::MB_Left:
			this->mNextGameStateId = GameStateId::MainMenu;
			break;
	}*/

	return true;
}
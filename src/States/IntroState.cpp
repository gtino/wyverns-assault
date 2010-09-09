#include "..\include\States\IntroState.h"

using namespace WyvernsAssault;

IntroState::IntroState(GraphicsManagerPtr graphicsManager, 
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

	//
	// Set game camera 
	// NOTE : I do not know why this is required, but IT IS! 
	//
	mCameraManager->gameCamera();

	mVideoManager = VideoManagerPtr(new VideoManager(mSceneManager));
	mVideoManager->initialize();
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
	mGuiScreen = mGuiManager->createScreen(GuiScreenId::IntroGui, "IntroScreen");
	
	GuiMovieClipPtr guiMovieClip = GuiMovieClipPtr(new GuiMovieClip());
	guiMovieClip->setMovieClip("video_material");

	mGuiScreen->setBackground(guiMovieClip);

	// Gui Widgets for this state
	mMenu = new GuiMenu(mWindow->getViewport(0)->getCamera()->getAspectRatio(), GuiScreenId::IntroGui);
	
	// Add menu to screen
	mGuiScreen->addMenu(mMenu);

	mAudioManager->playSoundTrack("intro.mp3");
	mVideoManager->play();
}

/** Update internal stuff */
void IntroState::update(const float elapsedSeconds)
{
	//
	// TODO Update
	//
	if(mVideoManager->isDone())
		this->mNextGameStateId = GameStateId::MainMenu;
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
		// Remove gui
		//
		mGuiManager->removeScreen(GuiScreenId::IntroGui);
		mGuiScreen.reset();
	}
	
	mAudioManager->stopSoundTrack();

	mVideoManager.reset();
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
	case OIS::KC_RETURN:
		this->mNextGameStateId = GameStateId::MainMenu;
		break;
	}

	return true;
}

bool IntroState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID buttonId)
{
	/*switch(buttonId)
	{
		case OIS::MouseButtonID::MB_Left:
			this->mNextGameStateId = GameStateId::MainMenu;
			break;
	}*/

	return true;
}
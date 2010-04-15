#include "..\include\States\PauseState.h"

using namespace WyvernsAssault;

PauseState::PauseState(GraphicsManager& graphicsManager, InputManager& inputManager, AudioManager& audioManager)
: BaseState(graphicsManager,inputManager,audioManager)
{
	//
	// TODO Constructor
	//
}

PauseState::~PauseState()
{
	//
	// TODO Destructor
	//
	exit();
}

/** Initialize current state */
void PauseState::initialize()
{
	this->mNextGameStateId = this->getStateId();

	mGuiScreen = 0;

	mRoot = mGraphicsManager->getRoot();

	mWindow = mGraphicsManager->getRenderWindow();

	mSceneManager = Root::getSingleton().getSceneManager("Default");

	//mRoot->_pushCurrentSceneManager(mSceneManager);

	if(mSceneManager->hasCamera( "DefaultCamera" ))
		mCamera = mSceneManager->getCamera( "DefaultCamera" );
	else
	{
		mCamera = mSceneManager->createCamera( "DefaultCamera" );
		mViewport = mWindow->addViewport( mCamera );
		mCamera->setAspectRatio(Real(mViewport->getActualWidth()) / Real(mViewport->getActualHeight()));
		mViewport->setBackgroundColour( Ogre::ColourValue( 0.95, 0.95, 0.95 ) );
	}
}

/** Load resources */
void PauseState::load()
{
	//
	// Gui Screen for this state
	//
	mGuiScreen = new GuiScreen(mSceneManager, GuiScreenId::PauseGui, "PauseScreen");
	
	GuiBackground* guiBackground = new GuiBackground();
	guiBackground->setImage("Pause.png","PauseBackground","General");

	mGuiScreen->setBackground(guiBackground);

	// Gui Widgets for this state
	GuiButton* menuButton = new GuiButton();
	menuButton->setSize(0.20, 0.20);
	menuButton->setPosition(0.80, 0.50);
	mGuiScreen->addWidget(menuButton,GuiWidgetPauseId::PlayGame);

	//
	// Register the screen as input event listener, so it can receive events
	//
	mInputManager->addListener(mGuiScreen);
}

/** Manage input */
void PauseState::input()
{
	//
	// TODO Read input
	//
	this->mInputManager->capture();
}

/** Update internal stuff */
void PauseState::update(const float elapsedSeconds)
{
	//
	// TODO Update
	//
}

/** Render */
void PauseState::render(const float elapsedSeconds)
{
	//
	// TODO Render
	//
}

/** Unload resources */
void PauseState::unload() 
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
void PauseState::finalize()
{
	if(mSceneManager)
	{
		mSceneManager->clearScene();
		mSceneManager->destroyAllCameras();		
	}

	if(mRoot)
	{
		mRoot->getAutoCreatedWindow()->removeAllViewports();
	}

	/*mSceneManager = mRoot->_getCurrentSceneManager();
	mRoot->_popCurrentSceneManager(mSceneManager);*/
	mSceneManager = mRoot->getSceneManager("Game");
}

/** Get state Id */
GameStateId PauseState::getStateId()
{
	//
	// BEWARE! Make sure this is the correspondent state or we fuck up everything!
	//
	return GameStateId::Pause;
}

/** Called when the state has to be paused */
void PauseState::pause()
{
	//
	// TODO : Pause state
	//
}

/** Called when the state has to be resumed (from pause) */
void PauseState::resume()
{
	//
	// TODO : Resume state
	//
}

/** Buffered input - keyboard key clicked */
bool PauseState::keyReleased(const OIS::KeyEvent& e)
{
	switch(e.key)
	{
	case OIS::KC_M:
		this->mNextGameStateId = GameStateId::MainMenu;
		break;
	case OIS::KC_ESCAPE:
		this->mNextGameStateId = GameStateId::Exit;
		break;
	case OIS::KC_P:
		// Resume
		this->mNextGameStateId = GameStateId::Play;
		break;
	}

	return true;
}
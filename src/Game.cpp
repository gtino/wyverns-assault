#include "..\include\Game.h"

using namespace WyvernsAssault;

Game::Game() :
mElapsedSeconds(0.0f),
mTotalSeconds(0.0f)
{
	mGraphicsManager = GraphicsManagerPtr(new GraphicsManager());
	mAudioManager = AudioManagerPtr(new AudioManager());
	mInputManager = InputManagerPtr(new InputManager());
	mStatesManager = StatesManagerPtr(new StatesManager());
}

Game::~Game()
{
	//
	// TODO Destructor
	//
}

/** Initialize */ 
bool Game::initialize()
{
	// Initialize graphic manager
	bool carryOn = mGraphicsManager->initialize();

	if(carryOn)
	{
	// Initialize Audio Layer
	mAudioManager->initialize();

	// Initialize input manager. A render window is needed in order to setup mouse coords and boundaries.
	mInputManager->initialize(mGraphicsManager->getRenderWindow(), true);
	
	// Load graphic resourcrs
	mGraphicsManager->loadResources();

	// Then we load audio
	mAudioManager->loadResources();

	// Initialize Game states (FSM) manager
	mStatesManager->initialize(mGraphicsManager, mInputManager, mAudioManager);
	}

	return carryOn;
}

/** Main loop */
void Game::go()
{

	bool continueRunning = true;
	while ( continueRunning )
	{
		Ogre::WindowEventUtilities::messagePump();

		// Get elaspes microseconds
		mElapsedSeconds = mLoopTimer.getMicroseconds() * 1.0 / 1000000;

		// Updte total seconds
		mTotalSeconds += mElapsedSeconds;

		//
		// Should we keep going? (Game input,logic,rendering)
		//
		continueRunning = mStatesManager->loop(mElapsedSeconds);

		//
		// Did the user close the application window?
		//
		bool windowClosed = mGraphicsManager->getRenderWindow()->isClosed();
		continueRunning &= ! windowClosed;

		mLoopTimer.reset();

		bool renderFrameSuccess = mGraphicsManager->renderOneFrame();
		continueRunning &= renderFrameSuccess;

		//continueRunning &= ! m_exitRequested;	
	}
}

/** Finalize the game */
void Game::finalize()
{
	// Unregister and dispose of the StatesManager
	mStatesManager.reset();
	// Unload all graphics data
	mGraphicsManager->unloadResources();
	// Finalize all
	mGraphicsManager.reset();
	// Unacquire all inputs
	mInputManager->unacquireAll();
	// Finalize all
	mInputManager.reset();
	// Unload audio/fx
	mAudioManager->unloadResources();
	// Release Audio Manager
	mAudioManager.reset();
}
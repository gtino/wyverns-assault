#include "..\include\Game.h"

using namespace WyvernsAssault;

Game::Game()
{
	//
	// TODO Constructor
	//
}

Game::~Game()
{
	//
	// TODO Destructor
	//
}

/** Initialize */ 
void Game::initialize()
{
	// Initialize graphic manager
	mGraphicsManager.initialize();

	// Initialize Audio Layer
	mAudioManager.initialize();

	// Initialize input manager. A render window is needed in order to setup mouse coords and boundaries.
	mInputManager.initialize(mGraphicsManager.getRenderWindow(), true);

	// Initialize Game states (FSM) manager
	mStatesManager.initialize(mGraphicsManager, mInputManager, mAudioManager);

	// Load graphic resourcrs
	mGraphicsManager.loadResources();

	// Then we load audio
	mAudioManager.loadResources();
}

/** Main loop */
void Game::go()
{
	//
	// TODO Main game loop
	//
	Ogre::Timer loopTimer;

	bool continueRunning = true;
	while ( continueRunning )
	{
		Ogre::WindowEventUtilities::messagePump();

		// Get elaspes microseconds
		float elapsedSeconds = loopTimer.getMicroseconds() * 1.0 / 1000000;

		//
		// Should we keep going? (Game logic)
		//
		continueRunning = mStatesManager.loop();

		//
		// Did the user close the application window?
		//
		bool windowClosed = mGraphicsManager.getRenderWindow()->isClosed();
		continueRunning &= ! windowClosed;

		loopTimer.reset();

		bool renderFrameSuccess = mGraphicsManager.renderOneFrame();
		continueRunning &= renderFrameSuccess;

		//continueRunning &= ! m_exitRequested;	
	}
}

/** Finalize the game */
void Game::finalize()
{
	// Unregister and dispose of the StatesManager
	mStatesManager.finalize();
	// Unload all graphics data
	mGraphicsManager.unloadResources();
	// Finalize all
	mGraphicsManager.finalize();
	// Unacquire all inputs
	mInputManager.unacquireAll();
	// Finalize all
	mInputManager.finalize();
	// Unload audio/fx
	mAudioManager.unloadResources();
	// Finalize all
	mAudioManager.finalize();
}
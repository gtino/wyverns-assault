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
	mGameStatesManager.initialize();

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
}

/** Finalize the game */
void Game::finalize()
{
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
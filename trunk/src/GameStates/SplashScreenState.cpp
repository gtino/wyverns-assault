#include "..\include\GameStates\SplashScreenState.h"

using namespace WyvernsAssault;

SplashScreenState::SplashScreenState(GameInputManager& inputManager, AudioManager& audioManager)
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
}

/** Initialize current state */
void SplashScreenState::initialise()
{
	//
	// TODO Initialize
	//
}

/** Load resources */
void SplashScreenState::load()
{
	//
	// TODO Load 
	//
}

/** Manage input */
void SplashScreenState::input()
{
	//
	// TODO Read input
	//
}

/** Update internal stuff */
void SplashScreenState::update()
{
	//
	// TODO Update
	//
}

/** Render */
void SplashScreenState::render()
{
	//
	// TODO Render
	//
}

/** Unload resources */
void SplashScreenState::unload() 
{
	//
	// TODO Unload
	//
}

/** Destroy the state */
void SplashScreenState::destroy()
{
	//
	// TODO Destroy
	//
}

/** Get state Id */
GameStateId SplashScreenState::getStateId()
{
	//
	// BEWARE! Make sure this is the correspondent state or we fuck up everything!
	//
	return GameStateId::SplashScreen;
}
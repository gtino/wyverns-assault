#include "..\include\States\SplashScreenState.h"

using namespace WyvernsAssault;

SplashScreenState::SplashScreenState(GraphicsManager& graphicsManager, InputManager& inputManager, AudioManager& audioManager)
: BaseState(graphicsManager,inputManager,audioManager)
{	
	//
	// TODO Constructor
	//
	this->mNextGameStateId = this->getStateId();
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
	this->mInputManager->capture();
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
	this->mNextGameStateId = GameStateId::Exit;

	return true;
}
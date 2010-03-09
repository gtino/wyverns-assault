#include "..\include\States\CreditsState.h"

using namespace WyvernsAssault;

CreditsState::CreditsState(GraphicsManager& graphicsManager, InputManager& inputManager, AudioManager& audioManager)
: BaseState(graphicsManager,inputManager,audioManager)
{
	//
	// TODO Constructor
	//
}

CreditsState::~CreditsState()
{
	//
	// TODO Destructor
	//
}

/** Initialize current state */
void CreditsState::initialise()
{
	//
	// TODO Initialize
	//
}

/** Load resources */
void CreditsState::load()
{
	//
	// TODO Load 
	//
}

/** Manage input */
void CreditsState::input()
{
	//
	// TODO Read input
	//
}

/** Update internal stuff */
void CreditsState::update(const float elapsedSeconds)
{
	//
	// TODO Update
	//
}

/** Render */
void CreditsState::render(const float elapsedSeconds)
{
	//
	// TODO Render
	//
}

/** Unload resources */
void CreditsState::unload() 
{
	//
	// TODO Unload
	//
}

/** Destroy the state */
void CreditsState::destroy()
{
	//
	// TODO Destroy
	//
}

/** Get state Id */
GameStateId CreditsState::getStateId()
{
	//
	// BEWARE! Make sure this is the correspondent state or we fuck up everything!
	//
	return GameStateId::Credits;
}

/** Called when the state has to be paused */
void CreditsState::pause()
{
	//
	// TODO : Pause state
	//
}

/** Called when the state has to be resumed (from pause) */
void CreditsState::resume()
{
	//
	// TODO : Resume state
	//
}
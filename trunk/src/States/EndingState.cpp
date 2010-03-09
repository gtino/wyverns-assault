#include "..\include\States\EndingState.h"

using namespace WyvernsAssault;

EndingState::EndingState(GraphicsManager& graphicsManager, InputManager& inputManager, AudioManager& audioManager)
: BaseState(graphicsManager,inputManager,audioManager)
{
	//
	// TODO Constructor logic HERE
	//
}

EndingState::~EndingState()
{
	//
	// TODO Distructor logic HERE
	//
}

/** Initialize current state */
void EndingState::initialise()
{
	//
	// TODO Initialize
	//
}

/** Load resources */
void EndingState::load()
{
	//
	// TODO Load 
	//
}

/** Manage input */
void EndingState::input()
{
	//
	// TODO Read input
	//
}

/** Update internal stuff */
void EndingState::update(const float elapsedSeconds)
{
	//
	// TODO Update
	//
}

/** Render */
void EndingState::render(const float elapsedSeconds)
{
	//
	// TODO Render
	//
}

/** Unload resources */
void EndingState::unload() 
{
	//
	// TODO Unload
	//
}

/** Destroy the state */
void EndingState::destroy()
{
	//
	// TODO Destroy
	//
}

/** Get state Id */
GameStateId EndingState::getStateId()
{
	//
	// BEWARE! Make sure this is the correspondent state or we fuck up everything!
	//
	return GameStateId::Ending;
}

/** Called when the state has to be paused */
void EndingState::pause()
{
	//
	// TODO : Pause state
	//
}

/** Called when the state has to be resumed (from pause) */
void EndingState::resume()
{
	//
	// TODO : Resume state
	//
}
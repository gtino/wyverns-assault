#include "..\include\GameStates\EndingState.h"

using namespace WyvernsAssault;

EndingState::EndingState(GraphicsManager& graphicsManager, InputManager& inputManager, AudioManager& audioManager)
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
void EndingState::update()
{
	//
	// TODO Update
	//
}

/** Render */
void EndingState::render()
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
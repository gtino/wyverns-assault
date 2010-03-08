#include "..\include\States\PlayState.h"

using namespace WyvernsAssault;

PlayState::PlayState(GraphicsManager& graphicsManager, InputManager& inputManager, AudioManager& audioManager)
: BaseState(graphicsManager,inputManager,audioManager)
{
	//
	// TODO Constructor logic HERE
	//
}

PlayState::~PlayState()
{
	//
	// TODO Distructor logic HERE
	//
}

/** Initialize current state */
void PlayState::initialise()
{
	//
	// TODO Initialize
	//
}

/** Load resources */
void PlayState::load()
{
	//
	// TODO Load 
	//
}

/** Manage input */
void PlayState::input()
{
	//
	// TODO Read input
	//
}

/** Update internal stuff */
void PlayState::update()
{
	//
	// TODO Update
	//
}

/** Render */
void PlayState::render()
{
	//
	// TODO Render
	//
}

/** Unload resources */
void PlayState::unload() 
{
	//
	// TODO Unload
	//
}

/** Destroy the state */
void PlayState::destroy()
{
	//
	// TODO Destroy
	//
}

/** Get state Id */
GameStateId PlayState::getStateId()
{
	//
	// BEWARE! Make sure this is the correspondent state or we fuck up everything!
	//
	return GameStateId::Play;
}

/** Called when the state has to be paused */
void PlayState::pause()
{
	//
	// TODO : Pause state
	//
}

/** Called when the state has to be resumed (from pause) */
void PlayState::resume()
{
	//
	// TODO : Resume state
	//
}
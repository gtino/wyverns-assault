#include "..\include\States\GameoverState.h"

using namespace WyvernsAssault;

GameoverState::GameoverState(GraphicsManager& graphicsManager, InputManager& inputManager, AudioManager& audioManager)
: BaseState(graphicsManager,inputManager,audioManager)
{
	//
	// TODO Constructor logic HERE
	//
}

GameoverState::~GameoverState()
{
	//
	// TODO Distructor logic HERE
	//
}

/** Initialize current state */
void GameoverState::initialize()
{
	//
	// TODO Initialize
	//
}

/** Manage input */
void GameoverState::input()
{
	//
	// TODO Read input
	//
}

/** Load resources */
void GameoverState::load()
{
	//
	// TODO Load 
	//
}

/** Update internal stuff */
void GameoverState::update(const float elapsedSeconds)
{
	//
	// TODO Update
	//
}

/** Render */
void GameoverState::render(const float elapsedSeconds)
{
	//
	// TODO Render
	//
}

/** Unload resources */
void GameoverState::unload() 
{
	//
	// TODO Unload
	//
}

/** Destroy the state */
void GameoverState::finalize()
{
	//
	// TODO Destroy
	//
}

/** Get state Id */
GameStateId GameoverState::getStateId()
{
	//
	// BEWARE! Make sure this is the correspondent state or we fuck up everything!
	//
	return GameStateId::GameOver;
}

/** Called when the state has to be paused */
void GameoverState::pause()
{
	//
	// TODO : Pause state
	//
}

/** Called when the state has to be resumed (from pause) */
void GameoverState::resume()
{
	//
	// TODO : Resume state
	//
}
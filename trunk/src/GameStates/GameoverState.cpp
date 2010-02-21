#include "..\include\GameStates\GameoverState.h"

using namespace WyvernsAssault;

GameoverState::GameoverState(GameInputManager& inputManager, AudioManager& audioManager)
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
void GameoverState::initialise()
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
void GameoverState::update()
{
	//
	// TODO Update
	//
}

/** Render */
void GameoverState::render()
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
void GameoverState::destroy()
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
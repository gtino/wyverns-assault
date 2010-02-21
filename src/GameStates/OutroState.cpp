#include "..\include\GameStates\OutroState.h"

using namespace WyvernsAssault;

OutroState::OutroState(GraphicsManager& graphicsManager, GameInputManager& inputManager, AudioManager& audioManager)
{
	//
	// TODO Constructor logic HERE
	//
}

OutroState::~OutroState()
{
	//
	// TODO Distructor logic HERE
	//
}

/** Initialize current state */
void OutroState::initialise()
{
	//
	// TODO Initialize
	//
}

/** Load resources */
void OutroState::load()
{
	//
	// TODO Load 
	//
}

/** Manage input */
void OutroState::input()
{
	//
	// TODO Read input
	//
}

/** Update internal stuff */
void OutroState::update()
{
	//
	// TODO Update
	//
}

/** Render */
void OutroState::render()
{
	//
	// TODO Render
	//
}

/** Unload resources */
void OutroState::unload() 
{
	//
	// TODO Unload
	//
}

/** Destroy the state */
void OutroState::destroy()
{
	//
	// TODO Destroy
	//
}

/** Get state Id */
GameStateId OutroState::getStateId()
{
	//
	// BEWARE! Make sure this is the correspondent state or we fuck up everything!
	//
	return GameStateId::Outro;
}
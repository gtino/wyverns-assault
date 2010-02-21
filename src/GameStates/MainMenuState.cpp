#include "..\include\GameStates\MainMenuState.h"

using namespace WyvernsAssault;

MainMenuState::MainMenuState(GameInputManager& inputManager, AudioManager& audioManager)
{
	//
	// TODO Constructor logic HERE
	//
}

MainMenuState::~MainMenuState()
{
	//
	// TODO Distructor logic HERE
	//
}

/** Initialize current state */
void MainMenuState::initialise()
{
	//
	// TODO Initialize
	//
}

/** Manage input */
void MainMenuState::input()
{
	//
	// TODO Read input
	//
}

/** Load resources */
void MainMenuState::load()
{
	//
	// TODO Load 
	//
}

/** Update internal stuff */
void MainMenuState::update()
{
	//
	// TODO Update
	//
}

/** Render */
void MainMenuState::render()
{
	//
	// TODO Render
	//
}

/** Unload resources */
void MainMenuState::unload() 
{
	//
	// TODO Unload
	//
}

/** Destroy the state */
void MainMenuState::destroy()
{
	//
	// TODO Destroy
	//
}

/** Get state Id */
GameStateId MainMenuState::getStateId()
{
	//
	// BEWARE! Make sure this is the correspondent state or we fuck up everything!
	//
	return GameStateId::MainMenu;
}
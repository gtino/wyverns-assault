#include "..\include\States\MainMenuState.h"

using namespace WyvernsAssault;

MainMenuState::MainMenuState(GraphicsManager& graphicsManager, InputManager& inputManager, AudioManager& audioManager)
: BaseState(graphicsManager,inputManager,audioManager)
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
void MainMenuState::update(const float elapsedSeconds)
{
	//
	// TODO Update
	//
}

/** Render */
void MainMenuState::render(const float elapsedSeconds)
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

/** Called when the state has to be paused */
void MainMenuState::pause()
{
	//
	// TODO : Pause state
	//
}

/** Called when the state has to be resumed (from pause) */
void MainMenuState::resume()
{
	//
	// TODO : Resume state
	//
}
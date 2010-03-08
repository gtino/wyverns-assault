#include "..\include\States\IntroState.h"

using namespace WyvernsAssault;

IntroState::IntroState(GraphicsManager& graphicsManager, InputManager& inputManager, AudioManager& audioManager)
: BaseState(graphicsManager,inputManager,audioManager)
{
	//
	// TODO Constructor
	//
}

IntroState::~IntroState()
{
	//
	// TODO Destructor
	//
}

/** Initialize current state */
void IntroState::initialise()
{
	//
	// TODO Initialize
	//
}

/** Manage input */
void IntroState::input()
{
	//
	// TODO Read input
	//
}

/** Load resources */
void IntroState::load()
{
	//
	// TODO Load 
	//
}

/** Update internal stuff */
void IntroState::update()
{
	//
	// TODO Update
	//
}

/** Render */
void IntroState::render()
{
	//
	// TODO Render
	//
}

/** Unload resources */
void IntroState::unload() 
{
	//
	// TODO Unload
	//
}

/** Destroy the state */
void IntroState::destroy()
{
	//
	// TODO Destroy
	//
}

/** Get state Id */
GameStateId IntroState::getStateId()
{
	//
	// BEWARE! Make sure this is the correspondent state or we fuck up everything!
	//
	return GameStateId::Intro;
}

/** Called when the state has to be paused */
void IntroState::pause()
{
	//
	// TODO : Pause state
	//
}

/** Called when the state has to be resumed (from pause) */
void IntroState::resume()
{
	//
	// TODO : Resume state
	//
}
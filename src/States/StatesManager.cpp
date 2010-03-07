#include "..\include\States\StatesManager.h"

using namespace WyvernsAssault;

StatesManager::StatesManager()
: mCurrentStateId ( GameStateId::SplashScreen )
{
	//
	// TODO Constructor
	//
	this->mCurrentState = getGameStateById(mCurrentStateId);
}

StatesManager::~StatesManager()
{
	//
	// TODO Destructor
	//
	finalize();
}

void StatesManager::initialize(GraphicsManager& graphicsManager, InputManager& inputManager, AudioManager& audioManager)
{
	//
	// NOTE Here we simply create the game states list, hard coded.
	//		Maybe it is better to provide some kind of way to load it
	//		'on the fly', reading it from a .cfg file?
	//
	this->mStates[GameStateId::SplashScreen]	= new SplashScreenState(graphicsManager, inputManager, audioManager);
	this->mStates[GameStateId::Intro]			= new IntroState(graphicsManager, inputManager, audioManager);
	this->mStates[GameStateId::MainMenu]		= new MainMenuState(graphicsManager, inputManager, audioManager);
	this->mStates[GameStateId::Ending]			= new EndingState(graphicsManager, inputManager, audioManager);
	this->mStates[GameStateId::GameOver]		= new GameoverState(graphicsManager, inputManager, audioManager);
	this->mStates[GameStateId::Play]			= new PlayState(graphicsManager, inputManager, audioManager);
	this->mStates[GameStateId::Credits]			= new CreditsState(graphicsManager, inputManager, audioManager);
	this->mStates[GameStateId::Outro]			= new OutroState(graphicsManager, inputManager, audioManager);
}

void StatesManager::finalize()
{
	//
	// TODO Destroy
	//
	//StatesMapIterator it;
	//for(it = mStates.begin(); it != mStates.end(); it++)
	//{
	//	delete it->second;
	//}

	mStates.clear(); // destructors should be called automatically...
	mStatesStack.clear(); // TBD states have already been destroyed. Make sure this call does not fail for this reason!
}

void StatesManager::loop()
{
	//
	// Read input
	//
	this->mCurrentState->input();

	//
	// Process game logic/physics/state
	//
	this->mCurrentState->update();

	//
	// Render output
	//
	this->mCurrentState->render();

	//
	// Read next state. If it is not the current one, perform a state change!
	// 
	GameStateId nextStateId = mCurrentState->getNextStateId();

	if(mCurrentStateId != nextStateId)
	{
		//
		// Retrieve the state corrisponding to the given state id
		//
		BaseState* newState = this->getGameStateById(nextStateId);

		//
		// Perform the state change
		//
		this->changeState(newState);
	}
}

// ------------ PRIVATE METHODS -------------------

void StatesManager::changeState(BaseState* state)
{
	// cleanup the current state
	if ( !mStatesStack.empty() ) {
		//
		// Exit from current state
		//
		mStatesStack.back()->exit();
		mStatesStack.pop_back();
	}

	//
	// Set new state as current state
	//
	this->mCurrentState = state;

	//
	// Save current state Id
	//
	this->mCurrentStateId = this->mCurrentState->getStateId();

	//
	// Store and init the new state
	//
	mStatesStack.push_back(state);
	mStatesStack.back()->enter();
}


void StatesManager::pushState(BaseState* state)
{
	//
	// Pause current state
	//
	if ( !mStatesStack.empty() ) {
		mStatesStack.back()->pause();
	}

	//
	// Set new state as current state
	//
	this->mCurrentState = state;

	//
	// Save current state Id
	//
	this->mCurrentStateId = this->mCurrentState->getStateId();

	//
	// Store and init the new state
	//
	mStatesStack.push_back(state);
	mStatesStack.back()->enter();
}

void StatesManager::popState()
{
	//
	// Cleanup the current state
	//
	if ( !mStatesStack.empty() ) {
		mStatesStack.back()->exit();
		mStatesStack.pop_back();
	}

	//
	// Set new state as current state
	//
	this->mCurrentState = mStatesStack.back();

	//
	// Save current state Id
	//
	this->mCurrentStateId = this->mCurrentState->getStateId();

	//
	// Resume previous state
	//
	if ( !mStatesStack.empty() ) {
		mStatesStack.back()->resume();
	}
}

BaseState* StatesManager::getGameStateById(const GameStateId gameStateId)
{
	return this->mStates[gameStateId];
}
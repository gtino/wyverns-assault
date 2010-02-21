#include "..\include\GameStates\GameStatesManager.h"

using namespace WyvernsAssault;

GameStatesManager::GameStatesManager()
: mCurrentStateId ( GameStateId::SplashScreen )
{
	//
	// TODO Constructor
	//
	this->mCurrentState = getGameStateById(mCurrentStateId);
}

GameStatesManager::~GameStatesManager()
{
	//
	// TODO Destructor
	//
	finalize();
}

void GameStatesManager::initialize()
{
	this->mAudioManager.initialize();
	//this->mInputManager.initialize();

	//
	// NOTE Here we simply create the game states list, hard coded.
	//		Maybe it is better to provide some kind of way to load it
	//		'on the fly', reading it from a .cfg file?
	//
	this->mGameStates[GameStateId::SplashScreen]	= new SplashScreenState(this->mInputManager, this->mAudioManager);
	this->mGameStates[GameStateId::Intro]			= new IntroState(this->mInputManager, this->mAudioManager);
	this->mGameStates[GameStateId::MainMenu]		= new MainMenuState(this->mInputManager, this->mAudioManager);
	this->mGameStates[GameStateId::Ending]		= new EndingState(this->mInputManager, this->mAudioManager);
	this->mGameStates[GameStateId::GameOver]		= new GameoverState(this->mInputManager, this->mAudioManager);
	this->mGameStates[GameStateId::Play]			= new PlayState(this->mInputManager, this->mAudioManager);
	this->mGameStates[GameStateId::Credits]		= new CreditsState(this->mInputManager, this->mAudioManager);
	this->mGameStates[GameStateId::Outro]			= new OutroState(this->mInputManager, this->mAudioManager);
}

void GameStatesManager::finalize()
{
	//
	// TODO Destroy
	//
	//GameStatesMapIterator it;
	//for(it = mGameStates.begin(); it != mGameStates.end(); it++)
	//{
	//	delete it->second;
	//}

	mGameStates.clear(); // destructors should be called automatically...
}

void GameStatesManager::loop()
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

void GameStatesManager::changeState(BaseState* newState)
{
	// 
	// Unload resources for current state
	//
	this->mCurrentState->unload();

	//
	// Destroy and finalize current state
	//
	this->mCurrentState->destroy();

	//
	// Set new state as current state
	//
	this->mCurrentState = newState;

	//
	// Save current state Id
	//
	this->mCurrentStateId = this->mCurrentState->getStateId();

	//
	// Initialize it
	//
	this->mCurrentState->initialise();

	//
	// ...and load its resources
	//
	this->mCurrentState->load();
}

BaseState* GameStatesManager::getGameStateById(const GameStateId gameStateId)
{
	return this->mGameStates[gameStateId];
}
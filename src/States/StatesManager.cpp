#include "..\include\States\StatesManager.h"

using namespace WyvernsAssault;

// BEGIN SINGLETON
template<> StatesManager* Ogre::Singleton<StatesManager>::ms_Singleton = 0;
StatesManager* StatesManager::getSingletonPtr(void)
{
    return ms_Singleton;
}
StatesManager& StatesManager::getSingleton(void)
{  
    assert( ms_Singleton );  return ( *ms_Singleton );  
}
// END SINGLETON

StatesManager::StatesManager()
: mCurrentStateId ( GameStateId::SplashScreen )
, mInitialized(false)
{
	//
	// TODO Constructor
	//
}

StatesManager::~StatesManager()
{
	//
	// TODO Destructor
	//
	if(mInitialized)
	{
		finalize();
	}
}

void StatesManager::initialize(GraphicsManagerPtr graphicsManager, InputManagerPtr inputManager, AudioManagerPtr audioManager, GuiManagerPtr guiManager, CameraManagerPtr cameraManager, LevelManagerPtr levelManager)
{
	//
	// Keep a reference to the managers
	//
	this->mGraphicsManager = graphicsManager;
	this->mInputManager = inputManager;
	this->mAudioManager = audioManager;
	this->mGuiManager = guiManager;
	this->mCameraManager = cameraManager;
	this->mLevelManager = levelManager;

	//
	// Register this class as listerer for the input manager (buffered input!)
	// Buffered events will call buffered input methods of this class
	// Fo unbuffered input we will have to access the mInputManager directly
	// in the game loop
	this->mInputManager->addListener(this);

	//
	// NOTE Here we simply create the game states list, hard coded.
	//		Maybe it is better to provide some kind of way to load it
	//		'on the fly', reading it from a .cfg file?
	//
	this->mStates[GameStateId::SplashScreen]	= new SplashScreenState(graphicsManager, inputManager, audioManager, cameraManager, guiManager, levelManager);
	this->mStates[GameStateId::Intro]			= new IntroState(graphicsManager, inputManager, audioManager, cameraManager, guiManager, levelManager);
	this->mStates[GameStateId::MainMenu]		= new MainMenuState(graphicsManager, inputManager, audioManager, cameraManager, guiManager, levelManager);
	this->mStates[GameStateId::Ending]			= new EndingState(graphicsManager, inputManager, audioManager, cameraManager, guiManager, levelManager);
	this->mStates[GameStateId::GameOver]		= new GameoverState(graphicsManager, inputManager, audioManager, cameraManager, guiManager, levelManager);
	this->mStates[GameStateId::Play]			= new PlayState(graphicsManager, inputManager, audioManager, cameraManager, guiManager, levelManager);
	this->mStates[GameStateId::LevelLoading]	= new LevelLoadingState(graphicsManager, inputManager, audioManager, cameraManager, guiManager, levelManager);
	this->mStates[GameStateId::Credits]			= new CreditsState(graphicsManager, inputManager, audioManager, cameraManager, guiManager, levelManager);
	this->mStates[GameStateId::Outro]			= new OutroState(graphicsManager, inputManager, audioManager, cameraManager, guiManager, levelManager);
	this->mStates[GameStateId::Options]			= new OptionsState(graphicsManager, inputManager, audioManager, cameraManager, guiManager, levelManager);
	this->mStates[GameStateId::Pause]			= new PauseState(graphicsManager, inputManager, audioManager, cameraManager, guiManager, levelManager);

	//
	// Let's start with the first state!
	//
	changeState(getGameStateById(mCurrentStateId));

	mInitialized = true;
}

void StatesManager::finalize()
{
	StatesMapIterator it;
	for(it = mStates.begin(); it != mStates.end(); it++)
	{
		delete it->second;
		it->second = 0;
	}

	mStates.clear();

	this->mInputManager->removeListener(this);

	mInitialized = false;
}

bool StatesManager::loop(const float elapsedSeconds)
{
		//
		// Read (UNBUFFERED) input
		//
		this->mCurrentState->input();

		//
		// Process game logic/physics/state
		//
		this->mCurrentState->update(elapsedSeconds);

		//
		// Render output
		//
		this->mCurrentState->render(elapsedSeconds);

		//
		// Read next state. If it is not the current one, perform a state change!
		// 
		GameStateId nextStateId = mCurrentState->getNextStateId();


		//
		// Special case, we just exit the application
		//
		if(nextStateId == GameStateId::Exit)
			return false;

		if(mCurrentStateId != nextStateId)
		{
			//
			// Retrieve the state corrisponding to the given state id
			//
			BaseState* newState = this->getGameStateById(nextStateId);

			//
			// Perform the state change
			//
			if(mCurrentStateId == GameStateId::Play && nextStateId == GameStateId::Pause)
			{
				this->pushState(newState);
			}
			else if(mCurrentStateId == GameStateId::Pause && nextStateId == GameStateId::Play)
			{
				this->popState();
			}
			else if(mCurrentStateId == GameStateId::MainMenu && nextStateId == GameStateId::Options)
			{
				this->pushState(newState);
			}
			// Exit to menu
			else if(mCurrentStateId == GameStateId::Pause && nextStateId == GameStateId::MainMenu)
			{
				this->popState();
				this->changeState(newState);
			}
			// Options from pause 
			else if(mCurrentStateId == GameStateId::Pause && nextStateId == GameStateId::Options)
			{
				// 
				// TODO
				//
				this->pushState(newState);
			}
			else if(nextStateId == GameStateId::Previous)
			{
				this->popState();
			}
			else
			{
				this->changeState(newState);
			}
			
		}

		//
		// Keep going 
		//
		return true;
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

// ------------------
// Keyboard listeners
// ------------------
/** Buffered input - keyboard key clicked */
bool StatesManager::keyClicked(const OIS::KeyEvent& e)
{
	// call keyClicked of current state
	return this->mCurrentState->keyClicked(e);
}

/** Buffered input - keyboard key clicked */
bool StatesManager::keyPressed(const OIS::KeyEvent& e)
{
	// call keyPressed of current state
	return this->mCurrentState->keyPressed(e);
}

/** Buffered input - keyboard key clicked */
bool StatesManager::keyReleased(const OIS::KeyEvent& e)
{
	// call keyReleased of current state
	return this->mCurrentState->keyReleased(e);
}

//
// MouseListener
//
/** Buffered input - mouse moved */
bool StatesManager::mouseMoved(const OIS::MouseEvent &evt)
{
	return this->mCurrentState->mouseMoved(evt);
}

/** Buffered input - mouse button pressed */
bool StatesManager::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID buttonId)
{
	return this->mCurrentState->mousePressed(evt,buttonId);
}

/** Buffered input - mouse button released */
bool StatesManager::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID buttonId)
{
	return this->mCurrentState->mouseReleased(evt,buttonId);
}

//
// JoyStickListener
//
/** Buffered input - joystick button pressed */
bool StatesManager::buttonPressed(const OIS::JoyStickEvent &evt, int index)
{
	return this->mCurrentState->buttonPressed(evt,index);
}

/** Buffered input - joystick button released */
bool StatesManager::buttonReleased(const OIS::JoyStickEvent &evt, int index)
{
	return this->mCurrentState->buttonReleased(evt,index);
}

/** Buffered input - axis pad moved */
bool StatesManager::axisMoved(const OIS::JoyStickEvent &evt, int index)
{
	return this->mCurrentState->axisMoved(evt,index);
}

/** Buffered input - pov moved */
bool StatesManager::povMoved(const OIS::JoyStickEvent &evt, int index)
{
	return this->mCurrentState->povMoved(evt,index);
}
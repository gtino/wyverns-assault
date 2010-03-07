#include "..\include\States\BaseState.h"

using namespace WyvernsAssault;

BaseState::BaseState()
{
}

BaseState::BaseState(GraphicsManager& graphicsManager, InputManager& inputManager, AudioManager& audioManager)
{
	this->mGraphicsManager	= &graphicsManager;
	this->mInputManager		= &inputManager;
	this->mAudioManager		= &audioManager;


	this->mIsActive = false;
	this->mIsDead = false;
	this->mIsPaused = false;
}

BaseState::~BaseState()
{
	//
	// TODO Distructor logic HERE
	//
}

void BaseState::enter()
{
	//
	// Initialize the state...
	//
	this->initialise();

	//
	// ...and load its resources
	//
	this->load();
}

void BaseState::exit()
{
	// 
	// Unload resources for current state
	//
	this->unload();

	//
	// Destroy and finalize current state
	//
	this->destroy();
}
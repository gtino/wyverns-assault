#include "..\include\States\BaseState.h"

using namespace WyvernsAssault;

BaseState::BaseState()
{
	this->mIsActive = false;
	this->mIsDead = false;
	this->mIsPaused = false;
}

BaseState::BaseState(GraphicsManager& graphicsManager, InputManager& inputManager, AudioManager& audioManager)
{
	this->mIsActive = false;
	this->mIsDead = false;
	this->mIsPaused = false;

	this->mGraphicsManager	= &graphicsManager;
	this->mInputManager		= &inputManager;
	this->mAudioManager		= &audioManager;

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
	this->initialize();

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
	this->finalize();
}
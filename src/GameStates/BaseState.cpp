#include "..\include\GameStates\BaseState.h"

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
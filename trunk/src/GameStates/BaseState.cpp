#include "..\include\GameStates\BaseState.h"

using namespace WyvernsAssault;

BaseState::BaseState()
{
}

BaseState::BaseState(GameInputManager& inputManager, AudioManager& audioManager)
{
	this->mInputManager = &inputManager;
	this->mAudioManager = &audioManager;


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
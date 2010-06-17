#include "..\include\States\BaseState.h"

using namespace WyvernsAssault;

BaseState::BaseState()
: mSceneManager(0)
, mRoot(0)
, mWindow(0)
, mCamera(0)
, mViewport(0)
, mIsActive(false)
, mIsDead(false)
, mIsPaused(false)
{
	this->mIsActive = false;
	this->mIsDead = false;
	this->mIsPaused = false;
}

BaseState::BaseState(GraphicsManagerPtr graphicsManager, InputManagerPtr inputManager, AudioManagerPtr audioManager, CameraManagerPtr cameraManager, GuiManagerPtr guiManager)
: mSceneManager(0)
, mRoot(0)
, mWindow(0)
, mCamera(0)
, mViewport(0)
, mIsActive(false)
, mIsDead(false)
, mIsPaused(false)
{
	this->mGraphicsManager	= graphicsManager;
	this->mInputManager		= inputManager;
	this->mAudioManager		= audioManager;
	this->mGuiManager		= guiManager;
	this->mCameraManager	= cameraManager;
}

BaseState::~BaseState()
{
	//
	// TODO Distructor logic HERE
	//
}

void BaseState::initialize()
{
	//
	// TODO Initialize
	//
	this->mNextGameStateId = this->getStateId();

	mRoot = mGraphicsManager->getRoot();

	mWindow = mGraphicsManager->getRenderWindow();

	mSceneManager = mGraphicsManager->getSceneManager();

	mCamera = mCameraManager->getGuiCamera();

	mViewport = mCameraManager->getGuiViewport();
}

void BaseState::finalize()
{
	// TODO : Common finalize stuff
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
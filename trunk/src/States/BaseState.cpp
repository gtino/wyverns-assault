#include "..\include\States\BaseState.h"

using namespace WyvernsAssault;

BaseState::BaseState()
: mGuiScreen(0)
, mSceneManager(0)
, mRoot(0)
, mWindow(0)
, mCamera(0)
, mViewport(0)
, mGraphicsManager(0)
, mInputManager(0)
, mAudioManager(0)
, mIsActive(false)
, mIsDead(false)
, mIsPaused(false)
{
	this->mIsActive = false;
	this->mIsDead = false;
	this->mIsPaused = false;
}

BaseState::BaseState(GraphicsManager& graphicsManager, InputManager& inputManager, AudioManager& audioManager)
: mGuiScreen(0)
, mSceneManager(0)
, mRoot(0)
, mWindow(0)
, mCamera(0)
, mViewport(0)
, mGraphicsManager(0)
, mInputManager(0)
, mAudioManager(0)
, mIsActive(false)
, mIsDead(false)
, mIsPaused(false)
{
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

void BaseState::initialize()
{
	//
	// TODO Initialize
	//
	this->mNextGameStateId = this->getStateId();

	mGuiScreen = 0;

	mRoot = mGraphicsManager->getRoot();

	mWindow = mGraphicsManager->getRenderWindow();

	mSceneManager = mGraphicsManager->getSceneManager();

	if(mSceneManager->hasCamera( "DefaultCamera" ))
		mCamera = mSceneManager->getCamera( "DefaultCamera" );
	else
	{
		mCamera = mSceneManager->createCamera( "DefaultCamera" );
		mViewport = mWindow->addViewport( mCamera );
		mCamera->setAspectRatio(Real(mViewport->getActualWidth()) / Real(mViewport->getActualHeight()));
		mViewport->setBackgroundColour( Ogre::ColourValue( 0.95, 0.95, 0.95 ) );
	}
}

void BaseState::finalize()
{
	if(mSceneManager)
	{
		mSceneManager->clearScene();
		mSceneManager->destroyAllCameras();
	}

	if(mRoot)
	{
		mRoot->getAutoCreatedWindow()->removeAllViewports();
	}
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
#include "..\include\States\BaseState.h"

using namespace WyvernsAssault;

BaseState::BaseState()
: mGuiScreen(0)
, mSceneManager(0)
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

BaseState::BaseState(GraphicsManagerPtr graphicsManager, InputManagerPtr inputManager, AudioManagerPtr audioManager)
: mGuiScreen(0)
, mSceneManager(0)
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

	if(mSceneManager->hasCamera( "GuiCamera" ))
	{
		mCamera = mSceneManager->getCamera( "GuiCamera" );
		mViewport = mCamera->getViewport();

		// Create viewport if not present already
		if(!mViewport)
		{
			mViewport = mGraphicsManager->createViewport( mCamera );
			mCamera->setAspectRatio(Real(mViewport->getActualWidth()) / Real(mViewport->getActualHeight()));
			mViewport->setBackgroundColour( Ogre::ColourValue( 0.3, 0.6, 0.9 ) );
		}
	}
	else
	{
		mCamera = mSceneManager->createCamera( "GuiCamera" );
		mViewport = mGraphicsManager->createViewport( mCamera );
		mCamera->setAspectRatio(Real(mViewport->getActualWidth()) / Real(mViewport->getActualHeight()));
		mViewport->setBackgroundColour( Ogre::ColourValue( 0.3, 0.6, 0.9 ) );
	}
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
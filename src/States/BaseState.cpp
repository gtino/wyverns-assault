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

	mCamera = mSceneManager->createCamera( "DefaultCamera" );

	mViewport = mWindow->addViewport( mCamera );

	mViewport->setBackgroundColour( Ogre::ColourValue( 1, 1, 1 ) );
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
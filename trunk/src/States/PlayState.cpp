#include "..\include\States\PlayState.h"

using namespace WyvernsAssault;

PlayState::PlayState(GraphicsManager& graphicsManager, InputManager& inputManager, AudioManager& audioManager)
: BaseState(graphicsManager,inputManager,audioManager)
{
	//
	// TODO Constructor logic HERE
	//
	this->mNextGameStateId = this->getStateId();
}

PlayState::~PlayState()
{
	//
	// TODO Distructor logic HERE
	//
	finalize();
}

/** Initialize current state */
void PlayState::initialize()
{
	mInputManager->setKeyMode(false);
	mInputManager->setMouseMode(false);
	mInputManager->setJoyStickMode(false);


	//
	// TODO Initialize
	//
	mCamera = mGraphicsManager->getSceneManager()->createCamera( "GameCamera" );

	mViewport = mGraphicsManager->getRenderWindow()->addViewport( mCamera );

	mViewport->setBackgroundColour( Ogre::ColourValue( 1, 1, 1 ) );
}

/** Load resources */
void PlayState::load()
{
	//
	// TODO Load 
	//
}

/** Manage input */
void PlayState::input()
{
	//
	// TODO Read input
	//
	mInputManager->capture();
}

/** Update internal stuff */
void PlayState::update(const float elapsedSeconds)
{
	//
	// TODO Update
	//
	float r,g;

	if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_DOWN))
	{
		r = mViewport->getBackgroundColour().r-0.01f;
		mViewport->setBackgroundColour( Ogre::ColourValue( r >= 0.0f ? r : 0.0f, 1 , 1 ) );
	}
	else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_UP))
	{
		r = mViewport->getBackgroundColour().r+0.01f;
		mViewport->setBackgroundColour( Ogre::ColourValue( r <= 1.0f ? r : 1.0f, 1, 1 ) );
	}

	if(this->mInputManager->getMouse()->getMouseState().Y.abs > this->mInputManager->getMouse()->getMouseState().height/2)
	{
		g = mViewport->getBackgroundColour().g-0.01f;
		mViewport->setBackgroundColour( Ogre::ColourValue( mViewport->getBackgroundColour().r, 1.0f , 1 ) );
	}
	else
	{
		g = mViewport->getBackgroundColour().g-0.01f;
		mViewport->setBackgroundColour( Ogre::ColourValue( mViewport->getBackgroundColour().r, 0.0f , 1 ) );
	}
}

/** Render */
void PlayState::render(const float elapsedSeconds)
{
	//
	// TODO Render
	//
}

/** Unload resources */
void PlayState::unload() 
{
	//
	// TODO Unload
	//
}

/** Destroy the state */
void PlayState::finalize()
{
	//
	// TODO Destroy
	//
	mGraphicsManager->getSceneManager()->clearScene();

	mGraphicsManager->getSceneManager()->destroyAllCameras();

	mGraphicsManager->getRoot()->getAutoCreatedWindow()->removeAllViewports();
}

/** Get state Id */
GameStateId PlayState::getStateId()
{
	//
	// BEWARE! Make sure this is the correspondent state or we fuck up everything!
	//
	return GameStateId::Play;
}

/** Called when the state has to be paused */
void PlayState::pause()
{
	//
	// TODO : Pause state
	//
}

/** Called when the state has to be resumed (from pause) */
void PlayState::resume()
{
	//
	// TODO : Resume state
	//
}
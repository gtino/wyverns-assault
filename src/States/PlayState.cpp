#include "..\include\States\PlayState.h"
#include "..\include\SceneLoader\DotSceneLoader.h"

using namespace WyvernsAssault;

PlayState::PlayState(GraphicsManager& graphicsManager, InputManager& inputManager, AudioManager& audioManager)
: BaseState(graphicsManager,inputManager,audioManager)
{
	//
	// TODO Constructor logic HERE
	//
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
	mInputManager->setKeyMode(true);
	mInputManager->setMouseMode(true);
	mInputManager->setJoyStickMode(true);


	//
	// TODO Initialize
	//
	this->mNextGameStateId = this->getStateId();

	mCamera = mGraphicsManager->getSceneManager()->createCamera( "Camera" );

	mViewport = mGraphicsManager->getRenderWindow()->addViewport( mCamera );

	mViewport->setBackgroundColour( Ogre::ColourValue( 1, 1, 1 ) );

	std::auto_ptr<DotSceneLoader> sceneLoader(new DotSceneLoader());
	sceneLoader->parseDotScene("Stage1_1.XML","General",mGraphicsManager->getSceneManager());

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

/*
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
	*/
}

/** Render */
void PlayState::render(const float elapsedSeconds)
{
	//
	// TODO Render
	//
//	if (mGraphicsManager->getRoot())
//		mGraphicsManager->getRoot()->startRendering();
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

/** Buffered input - keyboard key clicked */
bool PlayState::keyReleased(const OIS::KeyEvent& e)
{
	switch(e.key)
	{
	case OIS::KeyCode::KC_W:
		this->mNextGameStateId = GameStateId::Ending;
		break;
	case OIS::KeyCode::KC_G:
		this->mNextGameStateId = GameStateId::GameOver;
		break;
	}

	return true;
}
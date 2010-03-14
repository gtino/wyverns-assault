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
	BaseState::initialize();

	mInputManager->setKeyMode(true);
	mInputManager->setMouseMode(true);
	mInputManager->setJoyStickMode(true);

	this->mNextGameStateId = this->getStateId();

	// Camera manager constructor
	mCameraManager = new CameraManager(mGraphicsManager->getSceneManager(), mGraphicsManager->getRenderWindow(), mViewport);
	mCameraManager->initialize();

	// Lights manager constructor
	mLightsManager = new LightsManager(mGraphicsManager->getSceneManager());
	mLightsManager->initialize();

	//Load scene XML file
	std::auto_ptr<DotSceneLoader> sceneLoader(new DotSceneLoader());
	sceneLoader->parseDotScene("Stage1_1.XML","General",mGraphicsManager->getSceneManager(), mCameraManager, mLightsManager);

	/** DEBUG Labels **/
	mFpsDebugText.init();
	mFpsDebugText.setColor(ColourValue(0.0f,0.0f,0.0f,1.0f));

	// Player manager constructor
	mPlayerManager = new PlayerManager();
	mPlayerManager->initialize("redWyvern","redwyvern.mesh",mGraphicsManager->getSceneManager(),Vector3(100,-20,-45));

	mCameraManager->gameCamera(mPlayerManager->GetPlayerSceneNode());

}

/** Load resources */
void PlayState::load()
{
	//
	// Gui Screen for this state
	//
	mGuiScreen = new GuiScreen(mSceneManager, GuiScreenId::IntroGui, "PlayScreen");
	
	//
	// TODO Add Gui Widgets
	//

	//
	// Register the screen as input event listener, so it can receive events
	//
	mInputManager->addListener(mGuiScreen);
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
	// FREE DEBUG STUFF
	//
	mFpsDebugText.frameStarted();
	mFpsDebugText.print(0.01f,0.01f,"FPS : %f",elapsedSeconds);

	// Camera switch key control
	if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_1))
	{
		mCameraManager->gameCamera(mPlayerManager->GetPlayerSceneNode());
	}
	else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_2))
	{
		mCameraManager->fpsCamera(mPlayerManager->GetPlayerSceneNode());
	}
	else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_3))
	{
		mCameraManager->fixedCamera(0);
	}
	else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_4))
	{
		mCameraManager->fixedCamera(1);
	}
	else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_5))
	{
		mCameraManager->fixedCamera(2);
	}
	else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_6))
	{
		mCameraManager->fixedCamera(3);
	}
	else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_7))
	{
		mCameraManager->fixedCamera(4);
	}
	else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_8))
	{
		mCameraManager->fixedCamera(5);
	}
	else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_9))
	{
		mCameraManager->fixedCamera(6);
	}
	else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_0))
	{
		mCameraManager->travelCamera(0);
	}
	
	// Movement
	if(mCameraManager->getCameraType() == GAMECAMERA)
	{
		// 4 directions move
		if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_RIGHT))
		{
			mPlayerManager->setPosition(mPlayerManager->getPosition() + Vector3(SPEED,0,0));
		}
		if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_LEFT))
		{
			mPlayerManager->setPosition(mPlayerManager->getPosition() + Vector3(-SPEED,0,0));
		}
		if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_UP))
		{
			mPlayerManager->setPosition(mPlayerManager->getPosition() + Vector3(0,0,-SPEED));
		}
		if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_DOWN))
		{
			mPlayerManager->setPosition(mPlayerManager->getPosition() + Vector3(0,0,SPEED));
		}
		// Update camera position
		mCameraManager->updateCamera(mPlayerManager->GetPlayerSceneNode());
	}
	else if(mCameraManager->getCameraType() == FPSCAMERA)
	{
		if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_RIGHT))
		{
			mPlayerManager->setPosition(mPlayerManager->getPosition() + Vector3(0,0,SPEED));
		}
		if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_LEFT))
		{
			mPlayerManager->setPosition(mPlayerManager->getPosition() + Vector3(0,0,-SPEED));
		}
		if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_UP))
		{
			mPlayerManager->setPosition(mPlayerManager->getPosition() + Vector3(SPEED,0,0));
		}
		if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_DOWN))
		{
			mPlayerManager->setPosition(mPlayerManager->getPosition() + Vector3(-SPEED,0,0));
		}

		// Update camera position
		mCameraManager->updateCamera(mPlayerManager->GetPlayerSceneNode());
	}
}

/** Render */
void PlayState::render(const float elapsedSeconds)
{
	//
	// FREE DEBUG STUFF
	//
	mFpsDebugText.update();
}

/** Unload resources */
void PlayState::unload() 
{
	if(mGuiScreen)
	{
		//
		// Register the screen as input event listener, so it can receive events
		//
		mInputManager->removeListener(mGuiScreen);

		delete mGuiScreen;
		mGuiScreen = 0;
	}
	//
	// TODO Unload
	//
}

/** Destroy the state */
void PlayState::finalize()
{
	BaseState::finalize();

	mPlayerManager->finalize();
	
	/** Dispose of Debug text **/
	mFpsDebugText.finalize();
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
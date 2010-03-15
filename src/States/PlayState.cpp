#include "..\include\States\PlayState.h"
#include "..\include\SceneLoader\DotSceneLoader.h"

using namespace WyvernsAssault;

PlayState::PlayState(GraphicsManager& graphicsManager, InputManager& inputManager, AudioManager& audioManager)
: BaseState(graphicsManager,inputManager,audioManager)
, mPlayerManager(NULL)
, mLightsManager(NULL)
, mCameraManager(NULL)
, mEnemysManager(NULL)
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

	//Enemys manager constructor
	mEnemysManager = new EnemysManager();

	//Load scene XML file
	std::auto_ptr<DotSceneLoader> sceneLoader(new DotSceneLoader());
	sceneLoader->parseDotScene("Stage1_1.XML","General",mGraphicsManager->getSceneManager(), mCameraManager, mLightsManager, mEnemysManager);

	/** DEBUG Labels **/
	mFpsDebugText.init();
	mFpsDebugText.setColor(ColourValue(0.0f,0.0f,0.0f,1.0f));

	// Player UI
	//GuiImage* mPlayerUI = new GuiImage();
	//mPlayerUI->setImage("UI.png", "UI", "Interface");
	//mPlayerUI->setPosition();

	// Player manager constructor
	mPlayerManager = new PlayerManager();
	mPlayerManager->initialize("redWyvern","redwyvern.mesh",mGraphicsManager->getSceneManager(),Vector3(125,-35,-45));

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
	String poly = mCameraManager->getPolygonMode();
	mFpsDebugText.print(0.01f,0.01f,
		"POSITION: %4.0f, %4.0f, %4.0f     MODE: %s     FPS : %4.0f",
		mPlayerManager->getPosition().x, mPlayerManager->getPosition().y, mPlayerManager->getPosition().z,
		mCameraManager->getPolygonMode().c_str(),
		mWindow->getAverageFPS()
		);

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

	// WIN game - TEMP
	if(mPlayerManager->getPosition().x > 730)this->mNextGameStateId = GameStateId::Ending;;

	// LOSE game - TEMP
	if(mPlayerManager->getPosition().z < -80) this->mNextGameStateId = GameStateId::GameOver;;
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

	if(mPlayerManager)
	{
		delete mPlayerManager;
		mPlayerManager = NULL;
	}
	
	if(mCameraManager)
	{
		delete mCameraManager;
		mCameraManager = NULL;
	}

	if(mLightsManager)
	{
		delete mLightsManager;
		mLightsManager = NULL;
	}

	if(mEnemysManager)
	{
		delete mEnemysManager;
		mEnemysManager = NULL;
	}

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
	// Debug polygon mode
	case OIS::KeyCode::KC_F1:		
		mCameraManager->switchtPolygonMode();
		break;
	// Debug frames per second
	case OIS::KeyCode::KC_F2:		
		mFpsDebugText.toogle();
		break;
	}

	return true;
}
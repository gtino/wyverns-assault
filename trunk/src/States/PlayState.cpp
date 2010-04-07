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

	// Player manager constructor
	mPlayerManager = new PlayerManager();
	mPlayerManager->initialize("redWyvern","redwyvern.mesh",mGraphicsManager->getSceneManager(),Vector3(-150,620,2340));

	// Camera manager constructor
	mCameraManager = new CameraManager(mGraphicsManager->getSceneManager(), mGraphicsManager->getRenderWindow(), mViewport);
	mCameraManager->initialize(mPlayerManager->getPlayerSceneNode());

	// Lights manager constructor
	mLightsManager = new LightsManager(mGraphicsManager->getSceneManager());
	mLightsManager->initialize();

	//Enemys manager constructor
	mEnemysManager = new EnemysManager();

	//Load scene XML file
	std::auto_ptr<DotSceneLoader> sceneLoader(new DotSceneLoader());
	sceneLoader->parseDotScene("Stage1_1.XML","General", mGraphicsManager->getSceneManager(), mCameraManager, mLightsManager, mEnemysManager);

	/** DEBUG Labels **/
	mFpsDebugText.init();
	mFpsDebugText.setColor(ColourValue(0.0f,0.0f,0.0f,1.0f));

	// Player UI
	mGuiScreen = new GuiScreen(mSceneManager, GuiScreenId::PlayGui, "General");
	GuiImage* mPlayerUI = new GuiImage();
	mPlayerUI->setImage("UI.png", "PlayScreen", "General");
	mPlayerUI->setPosition(-0.95, 0.95);
	float width = 0.4f;
	float height = 0.3f;
	mPlayerUI->setSize(width, height);

	mGuiScreen->addWidget(mPlayerUI,GuiWidgetPlayId::UserInterface1);	
	mInputManager->addListener(mGuiScreen);

	mCameraManager->gameCamera();
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
	mFpsDebugText.setColor(ColourValue(1.0f, 1.0f, 1.0f, 1.0f));
	mFpsDebugText.print(0.01f,0.01f,
		"MODE: %s     PLAYER: %4.0f, %4.0f, %4.0f     CAMERA: %4.0f, %3.0f, %4.0f     FPS : %4.0f     CAMERA MODE: %s",
		mCameraManager->getPolygonMode().c_str(),
		mPlayerManager->getPlayerPosition().x, mPlayerManager->getPlayerPosition().y, mPlayerManager->getPlayerPosition().z,
		mCameraManager->getCameraPosition().x, mCameraManager->getCameraPosition().y, mCameraManager->getCameraPosition().z,		
		mWindow->getAverageFPS(),
		mCameraManager->getCameraMode().c_str()
		);
	
	// Movement
	if(mCameraManager->getCameraMode() == "First Person")
	{
		// 8 directions move
		if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_RIGHT) && this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_UP))
		{
			mPlayerManager->move(0.75,0,0.75);
		}
		else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_RIGHT) && this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_DOWN))
		{
			mPlayerManager->move(-0.75,0,0.75);
		}
		else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_LEFT) && this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_UP))
		{
			mPlayerManager->move(0.75,0,-0.75);
		}
		else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_LEFT) && this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_DOWN))
		{
			mPlayerManager->move(-0.75,0,-0.75);
		}
		else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_RIGHT))
		{
			mPlayerManager->move(0,0,1);
		}
		else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_LEFT))
		{
			mPlayerManager->move(0,0,-1);
		}
		else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_UP))
		{
			mPlayerManager->move(1,0,0);
		}
		else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_DOWN))
		{
			mPlayerManager->move(-1,0,0);
		}
	}
	else
	{
		// 8 directions move
		if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_RIGHT) && this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_UP))
		{
			mPlayerManager->move(0.75,0,-0.75);
		}
		else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_RIGHT) && this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_DOWN))
		{
			mPlayerManager->move(0.75,0,0.75);
		}
		else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_LEFT) && this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_UP))
		{
			mPlayerManager->move(-0.75,0,-0.75);
		}
		else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_LEFT) && this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_DOWN))
		{
			mPlayerManager->move(-0.75,0,0.75);
		}
		else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_RIGHT))
		{
			mPlayerManager->move(1,0,0);
		}
		else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_LEFT))
		{
			mPlayerManager->move(-1,0,0);
		}
		else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_UP))
		{
			mPlayerManager->move(0,0,-1);
		}
		else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_DOWN))
		{
			mPlayerManager->move(0,0,1);
		}
	}

	if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_M))
	{
		mCameraManager->zoom(-1);
		mCameraManager->updateCamera(elapsedSeconds);
	}
	if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_N))
	{
		mCameraManager->zoom(1);
		mCameraManager->updateCamera(elapsedSeconds);
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
		mCameraManager->switchPolygonMode();
		break;
	// Debug text
	case OIS::KeyCode::KC_F2:		
		mFpsDebugText.toogle();
		break;
	// Camera keys
	case OIS::KeyCode::KC_1:		
		mCameraManager->gameCamera();
		break;
	case OIS::KeyCode::KC_2:
		mCameraManager->fpsCamera();
		break;
	case OIS::KeyCode::KC_3:
		mCameraManager->fixedCamera(0);
		break;
	case OIS::KeyCode::KC_4:
		mCameraManager->fixedCamera(1);
		break;
	case OIS::KeyCode::KC_5:
		mCameraManager->fixedCamera(2);
		break;
	case OIS::KeyCode::KC_6:
		mCameraManager->fixedCamera(3);
		break;
	case OIS::KeyCode::KC_7:
		//mCameraManager->fixedCamera(4);
		break;
	case OIS::KeyCode::KC_8:
		//mCameraManager->fixedCamera(5);
		break;
	case OIS::KeyCode::KC_9:
		//mCameraManager->fixedCamera(6);
		break;
	case OIS::KeyCode::KC_0:
		mCameraManager->scenarioCamera();
		break;
	case OIS::KeyCode::KC_SPACE:
		mCameraManager->nextCamera();		
		break;
	/*case OIS::KeyCode::KC_M:
		mCameraManager->zoom(-1);		
		break;
	case OIS::KeyCode::KC_N:
		mCameraManager->zoom(1);		
		break;*/
	case OIS::KeyCode::KC_B:
		mLightsManager->disable();	
		break;
	}
	return true;
}
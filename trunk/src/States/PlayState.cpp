#include "..\include\States\PlayState.h"
#include "..\include\SceneLoader\DotSceneLoader.h"

using namespace WyvernsAssault;

PlayState::PlayState(GraphicsManager& graphicsManager, InputManager& inputManager, AudioManager& audioManager)
: BaseState(graphicsManager,inputManager,audioManager)
, mPlayerManager(NULL)
, mLightsManager(NULL)
, mCameraManager(NULL)
, mEnemysManager(NULL)
, mTrayMgr(NULL)
, mDetailsPanel(NULL)
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

	// Add Playstate as frame listener
	mRoot->addFrameListener(this);

	// Player manager constructor
	mPlayerManager = new PlayerManager();
	mPlayerManager->initialize("redWyvern","redwyvern.mesh",mGraphicsManager->getSceneManager(),Vector3(600,680,3400));

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

	// SdkTrays Manager
	mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", mWindow, mInputManager->getMouse());
	mTrayMgr->hideCursor();
	
	// Create a params panel for displaying sample details
	StringVector items;
	items.push_back("Player X");
	items.push_back("Player Y");
	items.push_back("Player Z");
	items.push_back("");
	items.push_back("Mode");
	items.push_back("Camera X");
	items.push_back("Camera Y");
	items.push_back("Camera Z");
	items.push_back("");	
	items.push_back("Filtering");
	items.push_back("Poly Mode");
 
	mDetailsPanel = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "DetailsPanel", 200, items);
	mDetailsPanel->setParamValue(9, "Bilinear");
	mDetailsPanel->setParamValue(10, "Solid");
	mDetailsPanel->hide();

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

/** Rendering queue */
bool PlayState::frameRenderingQueued(const Ogre::FrameEvent& evt)
{ 
	mTrayMgr->frameRenderingQueued(evt);
	if (!mTrayMgr->isDialogVisible())
	{
		if (mDetailsPanel->isVisible())   // if details panel is visible, then update its contents
		{
			mDetailsPanel->setParamValue(0, StringConverter::toString(mPlayerManager->getPlayerPosition().x));
			mDetailsPanel->setParamValue(1, StringConverter::toString(mPlayerManager->getPlayerPosition().y));
			mDetailsPanel->setParamValue(2, StringConverter::toString(mPlayerManager->getPlayerPosition().z));
			mDetailsPanel->setParamValue(4, mCameraManager->getCameraMode().c_str());
			mDetailsPanel->setParamValue(5, StringConverter::toString(mCameraManager->getCameraPosition().x));
			mDetailsPanel->setParamValue(6, StringConverter::toString(mCameraManager->getCameraPosition().y));
			mDetailsPanel->setParamValue(7, StringConverter::toString(mCameraManager->getCameraPosition().z));
		}
	}

	return true;
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
		"PLAYER: %4.0f, %4.0f, %4.0f     CAMERA: %4.0f, %3.0f, %4.0f     FPS : %4.0f     CAMERA MODE: %s",
		mPlayerManager->getPlayerPosition().x, mPlayerManager->getPlayerPosition().y, mPlayerManager->getPlayerPosition().z,
		mCameraManager->getCameraPosition().x, mCameraManager->getCameraPosition().y, mCameraManager->getCameraPosition().z,		
		mWindow->getAverageFPS(),
		mCameraManager->getCameraMode().c_str()
		);

	// Movement
	if(mCameraManager->getCameraMode() == "Free")
	{
		// Free camera mode movement
		CCS::CameraControlSystem* cameraCS = mCameraManager->getCameraCS();
		CCS::FreeCameraMode* freeCameraMode = (CCS::FreeCameraMode*)cameraCS->getCameraMode("Free");

		// Keyboard movement
		if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_RIGHT))
		{
			freeCameraMode->goRight();
		}
		if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_LEFT))
		{
			freeCameraMode->goLeft();
		}
		if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_UP))
		{
			freeCameraMode->goForward();
		}
		if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_DOWN))
		{
			freeCameraMode->goBackward();
		}
		if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_PGUP))
		{
			freeCameraMode->goUp();
		}
		if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_PGDOWN))
		{
			freeCameraMode->goDown();
		}

		// Mouse movement
		const OIS::MouseState &ms = this->mInputManager->getMouse()->getMouseState();
		freeCameraMode->yaw(ms.X.rel);
        freeCameraMode->pitch(ms.Y.rel);

		mCameraManager->updateCamera(elapsedSeconds);
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

	// Zoom for Fixed Direction camera mode (DEBUG only)
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
	
	if(mTrayMgr)
	{
		delete mTrayMgr;
		mTrayMgr = NULL;
	}

	/** Dispose of Debug text **/
	mFpsDebugText.finalize();

	mRoot->removeFrameListener(this);
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
	String newVal;
	PolygonMode pm;
	TextureFilterOptions tfo;
	unsigned int aniso;

	switch(e.key)
	{
	case OIS::KeyCode::KC_W:
		this->mNextGameStateId = GameStateId::Ending;
		break;
	case OIS::KeyCode::KC_O:
		this->mNextGameStateId = GameStateId::GameOver;
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
		break;
	case OIS::KeyCode::KC_8:
		break;
	case OIS::KeyCode::KC_9:
		mCameraManager->travelCamera(0);
		break;
	case OIS::KeyCode::KC_0:
		mCameraManager->scenarioCamera();
		break;
	case OIS::KeyCode::KC_SPACE:
		mCameraManager->nextCamera();		
		break;
	// Disable lights
	case OIS::KeyCode::KC_L:
		mLightsManager->disable();	
		break;
	//Debug text
	case OIS::KeyCode::KC_F2:		
		mFpsDebugText.toogle();
		break;
	// Toogle visibility of advanced stats frame
	case OIS::KeyCode::KC_F:
		mTrayMgr->toggleAdvancedFrameStats();
		break;
	// Toogle visibility of debugging details
	case OIS::KeyCode::KC_G:		
		if (mDetailsPanel->getTrayLocation() == OgreBites::TL_NONE)
		{
			mTrayMgr->moveWidgetToTray(mDetailsPanel, OgreBites::TL_TOPRIGHT, 0);
			mDetailsPanel->show();
			mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
			mTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
		}
		else
		{
			mTrayMgr->removeWidgetFromTray(mDetailsPanel);
			mDetailsPanel->hide();
			mTrayMgr->hideFrameStats();
			mTrayMgr->hideLogo();
		}
		break;
	// Cycle filtering mode
	case OIS::KeyCode::KC_T:
		switch (mDetailsPanel->getParamValue(9).asUTF8()[0])
		{
		case 'B':
			newVal = "Trilinear";
			tfo = Ogre::TFO_TRILINEAR;
			aniso = 1;
			break;
		case 'T':
			newVal = "Anisotropic";
			tfo = Ogre::TFO_ANISOTROPIC;
			aniso = 8;
			break;
		case 'A':
			newVal = "None";
			tfo = Ogre::TFO_NONE;
			aniso = 1;
			break;
		default:
			newVal = "Bilinear";
			tfo = Ogre::TFO_BILINEAR;
			aniso = 1;
		} 
		MaterialManager::getSingleton().setDefaultTextureFiltering(tfo);
		MaterialManager::getSingleton().setDefaultAnisotropy(aniso);
		mDetailsPanel->setParamValue(9, newVal);	
		break;
	// Cycle polygon rendering mode
	case OIS::KeyCode::KC_R: 
		switch (mCameraManager->getPolygonMode())
		{
		case Ogre::PM_SOLID:
			newVal = "Wireframe";
			pm = Ogre::PM_WIREFRAME;
			break;
		case Ogre::PM_WIREFRAME:
			newVal = "Points";
			pm = Ogre::PM_POINTS;
			break;
		default:
			newVal = "Solid";
			pm = Ogre::PM_SOLID;
		} 
		mCameraManager->setPolygonMode(pm);
		mDetailsPanel->setParamValue(10, newVal);
		break;
	// Reload all textures
	case OIS::KeyCode::KC_F5:
		TextureManager::getSingleton().reloadAll();
		break;

	}

	return true;
}
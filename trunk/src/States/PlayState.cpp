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
	mPlayerManager->initialize("redWyvern","redwyvern.mesh",mGraphicsManager->getSceneManager(),Vector3(150,25,850));

	// Camera manager constructor
	mCameraManager = new CameraManager(mGraphicsManager->getSceneManager(), mGraphicsManager->getRenderWindow(), mViewport);
	mCameraManager->initialize(mPlayerManager->getPlayerSceneNode());

	// Lights manager constructor
	mLightsManager = new LightsManager(mGraphicsManager->getSceneManager());
	mLightsManager->initialize();

	//Enemys manager constructor
	mEnemysManager = new EnemysManager(mSceneManager);

	//Load scene XML file
	std::auto_ptr<DotSceneLoader> sceneLoader(new DotSceneLoader());
	sceneLoader->parseDotScene("Stage1_1.XML","General", mGraphicsManager->getSceneManager(), mCameraManager, mLightsManager, mEnemysManager);	

	// Set game camera
	mCameraManager->gameCamera();

	// SdkTrays Manager
	mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", mWindow, mInputManager->getMouse());
	if(mTrayMgr->isCursorVisible()) mTrayMgr->hideCursor();
	
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

	//
	// Item Manager
	//
	mItemManager = new ItemManager();
	mItemManager->initialize();

	//
	// Physics Manager
	// 
	mPhysicsManager = new PhysicsManager();
	mPhysicsManager->initialize();

	//
	// Lua Manager
	//
	mLuaManager = new LuaManager(mLightsManager,mPhysicsManager,mEnemysManager,mPlayerManager,mItemManager,mAudioManager);
	mLuaManager->initialize();
}

/** Load resources */
void PlayState::load()
{
	//
	// Gui Screen for this state
	//
	mGuiScreen = new GuiScreen(mSceneManager, GuiScreenId::PlayGui, "General");

	//
	// Wigdets for this state
	//	
	mPlayerUI = new GuiUserInterface(mWindow->getViewport(0)->getCamera()->getAspectRatio(), GuiWidgetPlayId::UserInterface1);

	// Add Gui Widgets to Manager
	mGuiScreen->addWidget(mPlayerUI,GuiWidgetPlayId::UserInterface1);	
	
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
		else
		{
			// No movement, iddle animation
			mPlayerManager->move(0,0,0);
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

	// UI DEBUG KEYS - Increments/Decrements kills and points
	if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_PGUP))	
		mPlayerUI->setTextKills(mPlayerUI->getTextKills() + 1);

	if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_PGDOWN))
		mPlayerUI->setTextKills(mPlayerUI->getTextKills() - 1);

	if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_HOME))	
		mPlayerUI->setTextPoints(mPlayerUI->getTextPoints() + 1);

	if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_END))
		mPlayerUI->setTextPoints(mPlayerUI->getTextPoints() - 1);

	// UI DEBUG KEYS - Increments/Decrements life and special bars
	if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_F7))	
		mPlayerUI->setLifeBar(mPlayerUI->getLifeBar() - 2);

	if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_F8))
		mPlayerUI->setLifeBar(mPlayerUI->getLifeBar() + 2);

	if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_F9))	
		mPlayerUI->setSpecialBar(mPlayerUI->getSpecialBar() - 2);

	if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_F10))
		mPlayerUI->setSpecialBar(mPlayerUI->getSpecialBar() + 2);

	//
	// LUA MANAGER
	// 
	mLuaManager->run(elapsedSeconds);

	//
	// Update animation state
	//
	mPlayerManager->updateAnimation(elapsedSeconds);
}

/** Render */
void PlayState::render(const float elapsedSeconds)
{
	//
	// FREE DEBUG STUFF
	//
}

/** Unload resources */
void PlayState::unload() 
{
	if(mGuiScreen)
	{
		//
		// Remove gui listener
		//
		mInputManager->removeListener(mGuiScreen);
		//
		// Remove gui
		//
		mGuiScreen->removeGui();
	}
	if(mTrayMgr)
	{
		//
		// Remove all sdktrays
		//
		mTrayMgr->hideAll();
		mTrayMgr->clearAllTrays();
	}
	if(mRoot)
	{
		//
		// Remove frame listener
		//
		mRoot->removeFrameListener(this);
	}
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

	if(mGuiScreen)
	{
		delete mGuiScreen;
		mGuiScreen = NULL;
	}

	if(mPhysicsManager)
	{
		delete mPhysicsManager;
		mPhysicsManager = NULL;
	}

	if(mItemManager)
	{
		delete mItemManager;
		mItemManager = NULL;
	}

	if(mLuaManager)
	{
		delete mLuaManager;
		mLuaManager = NULL;
	}
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
	// Hide gui without removing it
	//
	mGuiScreen->hide();
	// Hide sdktrays if visible
	mTrayMgr->hideAll();
	if(mTrayMgr->isCursorVisible()) mTrayMgr->hideCursor();
}

/** Called when the state has to be resumed (from pause) */
void PlayState::resume()
{
	//
	// Set next state to this state (Play)	
	//
	this->mNextGameStateId = this->getStateId();
	//
	// Show gui (hided when pause)
	//
	mGuiScreen->show();
	// Show sdktrays if was visible
	mTrayMgr->showAll();
	if(mTrayMgr->isCursorVisible()) mTrayMgr->hideCursor();
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
	case OIS::KeyCode::KC_ESCAPE:
		this->mNextGameStateId = GameStateId::Exit;
		break;
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
	// Pause menu
	case OIS::KeyCode::KC_P:		
		this->mNextGameStateId = GameStateId::Pause;
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
	// Reload all Lua scripts
	case OIS::KeyCode::KC_F6:
		mLuaManager->reload();
		break;
	// Toggle Lua manager active state
	case OIS::KeyCode::KC_U:
		if(mLuaManager->isEnabled())
			mLuaManager->disable();
		else
			mLuaManager->enable();
		break;
	}

	return true;
}
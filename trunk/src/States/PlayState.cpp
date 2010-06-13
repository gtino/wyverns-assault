#include "..\include\States\PlayState.h"

using namespace WyvernsAssault;

PlayState::PlayState(GraphicsManagerPtr graphicsManager, InputManagerPtr inputManager, AudioManagerPtr audioManager)
: BaseState(graphicsManager,inputManager,audioManager)
, mTrayMgr(NULL)
, mDetailsPanel(NULL)
, mRootSceneNode(0)
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

	//
	// Particle Manager
	//
	mParticleManager = ParticleManagerPtr(new ParticleManager(mSceneManager));
	mParticleManager->initialize();

	//
	// Post Processing manager
	//
	mPostProcessManager = PostProcessManagerPtr(new PostProcessManager(mViewport));
	mPostProcessManager->initialize();

	// Player manager constructor
	mPlayerManager = PlayerManagerPtr(new PlayerManager(mSceneManager));
	mPlayerManager->initialize();

	// Create a single player (TEST!)
	PlayerPtr player1 = mPlayerManager->createPlayer("Player1","redWyvern.mesh");
	player1->setPosition(Vector3(80, 20, 870));
	// Add fire breath (TEST!)
	player1->setFireBreath(mParticleManager->create("firebreath","WyvernsAssault/DragonBreath"));

	// Camera manager constructor
	mCameraManager = CameraManagerPtr(new CameraManager(mSceneManager, mWindow, mViewport));
	mCameraManager->initialize();

	// Lights manager constructor
	mLightsManager = LightsManagerPtr(new LightsManager(mSceneManager));
	mLightsManager->initialize();

	//
	// Physics Manager
	// 
	mPhysicsManager = PhysicsManagerPtr(new PhysicsManager(mSceneManager));
	mPhysicsManager->initialize();
	mPhysicsManager->addPlayer(player1);

	//Enemys manager constructor
	mEnemyManager = EnemyManagerPtr(new EnemyManager(mSceneManager));
	mEnemyManager->initialize();

	// SdkTrays Manager
	mTrayMgr = new OgreBites::SdkTrayManager("DebugInterface", mWindow, mInputManager->getMouse());
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
	items.push_back("Look At X");
	items.push_back("Look At Y");
	items.push_back("Look At Z");
	items.push_back("");	
	items.push_back("Filtering");
	items.push_back("Poly Mode");
 
	mDetailsPanel = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "DetailsPanel", 200, items);
	mDetailsPanel->setParamValue(12, "Bilinear");
	mDetailsPanel->setParamValue(13, "Solid");
	mDetailsPanel->hide();

	//
	// Item Manager
	//
	mItemManager = ItemManagerPtr(new ItemManager(mSceneManager));
	mItemManager->initialize();

	//
	// Logic Manager
	//
	mLogicManager = LogicManagerPtr(new LogicManager());
	mLogicManager->initialize();

	// -----------
	// Lua Manager
	// -----------
	// Create this manager after ALL other managers. Never alter
	// the oreder of the operations!
	//
	// FIRST : Instatniate the Lua Manager
	mLuaManager = LuaManagerPtr(new LuaManager());

	//
	// SECOND : Register all the LuaInterfaces you want. 
	//
	mLuaManager->registerInterface(mLightsManager);
	mLuaManager->registerInterface(mLogicManager);
	mLuaManager->registerInterface(mEnemyManager);
	mLuaManager->registerInterface(mPhysicsManager);
	mLuaManager->registerInterface(mItemManager);
	mLuaManager->registerInterface(mPlayerManager);

	//
	// THIRD :	This call to 'initialize' will initialize Lua,
	//			open default Lua libs, add custom libs exported by
	//			our registered LuaInterfaces, setup the environment.
	//
	mLuaManager->initialize();

	//Load scene XML file
	mScenarioManager = ScenarioManagerPtr(new ScenarioManager(mSceneManager));
	mScenarioManager->initialize(mCameraManager, mLightsManager, mEnemyManager, mPhysicsManager, mItemManager, mParticleManager);
	mScenarioManager->load("Level1_1.scene");
	mScenarioManager->load("Stage1_1.XML");

	//
	// Events Manager 
	//
	mEventsManager = EventsManagerPtr(new EventsManager());
	
	//
	// Register all events listeners/callers
	//
	mEventsManager->registerInterface(mEnemyManager.get());
	mEventsManager->registerInterface(mPhysicsManager.get());
	mEventsManager->registerInterface(mParticleManager.get());
	mEventsManager->registerInterface(mAudioManager.get());
	mEventsManager->registerInterface(mItemManager.get());
	mEventsManager->registerInterface(mPlayerManager.get());

	//
	// Set game camera
	//
	mCameraManager->gameCamera();

	//
	// Audio: playState track
	//
	mAudioManager->playSoundTrack("main_track.mp3");
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
	mPlayerUI = new GuiUserInterface(mWindow->getViewport(0)->getCamera()->getAspectRatio(), GuiScreenId::PlayGui, GuiWidgetPlayId::UserInterface1);

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
			PlayerPtr player1 = mPlayerManager->getPlayer(PLAYER1);
			mDetailsPanel->setParamValue(0, StringConverter::toString(player1->getPosition().x, 5));
			mDetailsPanel->setParamValue(1, StringConverter::toString(player1->getPosition().y, 5));
			mDetailsPanel->setParamValue(2, StringConverter::toString(player1->getPosition().z, 5));
			mDetailsPanel->setParamValue(4, mCameraManager->getCameraMode().c_str());
			mDetailsPanel->setParamValue(5, StringConverter::toString(mCameraManager->getCameraPosition().x, 5));
			mDetailsPanel->setParamValue(6, StringConverter::toString(mCameraManager->getCameraPosition().y, 5));
			mDetailsPanel->setParamValue(7, StringConverter::toString(mCameraManager->getCameraPosition().z, 5));
			mDetailsPanel->setParamValue(8, StringConverter::toString(mCameraManager->getCameraLookAt().x, 5));
			mDetailsPanel->setParamValue(9, StringConverter::toString(mCameraManager->getCameraLookAt().y, 5));
			mDetailsPanel->setParamValue(10, StringConverter::toString(mCameraManager->getCameraLookAt().z, 5));
		}
	}

	if(mCameraManager->getCameraMode() == "Free")
	{
		mCameraManager->frameRenderingQueued(evt);
	}

	return true;
}

/** Update internal stuff */
void PlayState::update(const float elapsedSeconds)
{
	PlayerPtr player1 = mPlayerManager->getPlayer(PLAYER1);

	if ( !player1->isDeath() )
	{
		// Movement
		if(mCameraManager->getCameraMode() == "Game")
		{
			// Move if not using special attack or attacking
			if(!player1->isSpecial() && !player1->isAttacking())
			{
				// 8 directions move
				if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_RIGHT) && this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_UP))
				{
					player1->move(1,0,-1);
					mPhysicsManager->move(player1, mCameraManager->getDirection(Vector3(1,0,-1)));
				}
				else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_RIGHT) && this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_DOWN))
				{
					player1->move(1,0,1);
					mPhysicsManager->move(player1, mCameraManager->getDirection(Vector3(1,0,1)));
				}
				else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_LEFT) && this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_UP))
				{
					player1->move(-1,0,-1);
					mPhysicsManager->move(player1, mCameraManager->getDirection(Vector3(-1,0,-1)));
				}
				else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_LEFT) && this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_DOWN))
				{
					player1->move(-1,0,1);
					mPhysicsManager->move(player1, mCameraManager->getDirection(Vector3(-1,0,1)));
				}
				else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_RIGHT))
				{
					player1->move(1,0,0);
					mPhysicsManager->move(player1, mCameraManager->getDirection(Vector3(1,0,0)));
				}
				else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_LEFT))
				{
					player1->move(-1,0,0);
					mPhysicsManager->move(player1, mCameraManager->getDirection(Vector3(-1,0,0)));
				}
				else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_UP))
				{
					player1->move(0,0,-1);
					mPhysicsManager->move(player1, mCameraManager->getDirection(Vector3(0,0,-1)));
				}
				else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_DOWN))
				{
					player1->move(0,0,1);
					mPhysicsManager->move(player1, mCameraManager->getDirection(Vector3(0,0,1)));
				}		
				else
				{
					// No movement, iddle animation
					player1->move(0,0,0);
					mPhysicsManager->move(player1, Vector3::ZERO);
				}
			}
			else
			{
				// No movement
				mPhysicsManager->move(player1, Vector3::ZERO);
			}
		}
		else
		{
			// No movement, iddle animation
			player1->move(0,0,0);
			mPhysicsManager->move(player1, Vector3(0,0,0));
		}
	}
	else
	{
		if( !player1->isDying() )
			this->mNextGameStateId = GameStateId::GameOver;
	}
/*
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
*/
	//
	//Update Physic
	//
	mPhysicsManager->update(elapsedSeconds);

	//
	// Logic manager
	//
	mLogicManager->update(elapsedSeconds);

	//
	// LUA manager
	// 
	mLuaManager->update(elapsedSeconds);

	mLightsManager->update(elapsedSeconds);

	mEnemyManager->update(elapsedSeconds);

	mItemManager->update(elapsedSeconds);

	//
	// Update animation state
	//
	player1->updateAnimation(elapsedSeconds);

	//
	// Update camera
	//
	mCameraManager->updateCamera(player1->getSceneNode()->getPosition(), elapsedSeconds);

	//
	// Update sounds
	//
	mAudioManager->update(mCameraManager->getCamera()->getParentSceneNode(), elapsedSeconds);

	//
	// Update post processing compositors
	//
	mPostProcessManager->update(elapsedSeconds);

	//
	// Dispatch events. Managers have probably raised events, that are now in the 
	// EventsManager queue. The events manager will then dispatch them, calling
	// for each of them the registered handler(s).
	//
	mEventsManager->dispatchEvents();
}

/** Render */
void PlayState::render(const float elapsedSeconds)
{
	//
	// TODO
	//
}

/** Unload resources */
void PlayState::unload() 
{
	//
	// Audio manager
	//
	mAudioManager->playSoundTrack("hard_track.mp3");

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
	////// FIRST!
	mLightsManager.reset();

	mLuaManager.reset();

	mPlayerManager.reset();

	mCameraManager.reset();

	mEnemyManager.reset();

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

	mPhysicsManager.reset();

	mItemManager.reset();

	mLogicManager.reset();

	mEventsManager.reset();

	mParticleManager.reset();

	mScenarioManager.reset();

	mPostProcessManager.reset();

	mGraphicsManager->clearScene();

	BaseState::finalize();
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
	if(mTrayMgr->isCursorVisible()) 
		mTrayMgr->hideCursor();

	// Hide axes if visible
	mCameraManager->toogleAxes();

	// Disable all post process
	mPostProcessManager->pause();
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
	if(mTrayMgr->isCursorVisible())
		mTrayMgr->hideCursor();

	// Show axes if visible
	mCameraManager->toogleAxes();

	// Resume previous camera
	mCameraManager->resumeCamera();

	// Resume all post process previous state
	mPostProcessManager->resume();
}

/** Buffered input - keyboard key clicked */
bool PlayState::keyPressed(const OIS::KeyEvent& e)
{
	String newVal;
	PolygonMode pm;
	TextureFilterOptions tfo;
	unsigned int aniso;
	PlayerPtr player1 = mPlayerManager->getPlayer(PLAYER1);

	switch(e.key)
	{
	case OIS::KeyCode::KC_I:
		this->mNextGameStateId = GameStateId::Ending;
		break;
	case OIS::KeyCode::KC_O:
		player1->Die();		
		break;
	// Pause menu
	case OIS::KeyCode::KC_P:		
		this->mNextGameStateId = GameStateId::Pause;
		break;
	// Camera keys
	case OIS::KeyCode::KC_1:		
		mCameraManager->gameCamera();
		break;
	case OIS::KeyCode::KC_2:
		mCameraManager->freeCamera();
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
		mCameraManager->fixedCamera(4);
		break;
	case OIS::KeyCode::KC_8:
		mCameraManager->fixedCamera(5);
		break;
	// Toogle visibility of debugging details
	case OIS::KeyCode::KC_G:		
		if (mDetailsPanel->getTrayLocation() == OgreBites::TL_NONE)
		{
			mTrayMgr->moveWidgetToTray(mDetailsPanel, OgreBites::TL_TOPRIGHT, 0);
			mDetailsPanel->show();
			mTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
			mTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
			mCameraManager->showAxes();

			mEnemyManager->setDebugEnabled(true);
			mPlayerManager->setDebugEnabled(true);
		}
		else
		{
			mTrayMgr->removeWidgetFromTray(mDetailsPanel);
			mDetailsPanel->hide();
			mTrayMgr->hideFrameStats();
			mTrayMgr->hideLogo();
			mCameraManager->hideAxes();

			mEnemyManager->setDebugEnabled(false);
			mPlayerManager->setDebugEnabled(false);
		}		
		break;
	// Cycle filtering mode
	case OIS::KeyCode::KC_T:
		switch (mDetailsPanel->getParamValue(12).asUTF8()[0])
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
		mDetailsPanel->setParamValue(12, newVal);	
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
		mDetailsPanel->setParamValue(13, newVal);
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
	// Physics debug mode
	case OIS::KeyCode::KC_F:
		mPhysicsManager->showDebugObjects();
		break;

	// Attack A
	case OIS::KeyCode::KC_SPACE:
		mPlayerManager->attack("Player1");
		break;
	// Special Attack
	case OIS::KeyCode::KC_BACK:
		mPlayerManager->attackSpecial("Player1");
		break;	

	// Motion Blur
	case OIS::KeyCode::KC_J:
		mPostProcessManager->motionBlur(2);	
		break;

	// Depth of Field
	case OIS::KeyCode::KC_K:
		mPostProcessManager->depthOfField();
		break;	

	// Radial Blur
	case OIS::KeyCode::KC_L:
		mPostProcessManager->radialBlur(2);
		break;

	// Normals On/Off
	case OIS::KeyCode::KC_HOME:
		mPostProcessManager->showNormal();
		break;	

	// Depth On/Off
	case OIS::KeyCode::KC_END:
		mPostProcessManager->showDepth();
		break;	

	// Camera rumble - Debug
	case OIS::KeyCode::KC_N:
		mCameraManager->rumble(5);
		break;
	// Camera tremor - Debug
	case OIS::KeyCode::KC_M:
		mCameraManager->tremor(5);
		break;

	// Blood particle system - Debug
	case OIS::KeyCode::KC_Z:		
		mParticleManager->bloodKill(player1->getSceneNode());
		break;
	case OIS::KeyCode::KC_X:
		mParticleManager->hit(player1->getSceneNode());
		break;
	case OIS::KeyCode::KC_C:
		mParticleManager->bloodHit(player1->getSceneNode());
		break;
	case OIS::KeyCode::KC_V:
		mParticleManager->bloodLens();
		break;	
	}

	// Free camera mode move
	if(mCameraManager->getCameraMode() == "Free")
	{
		mCameraManager->freeCameraKeyboardDown(e);
	}

	return true;
}

bool PlayState::keyReleased(const OIS::KeyEvent& e)
{
	// Free camera mode move
	if(mCameraManager->getCameraMode() == "Free")
	{
		mCameraManager->freeCameraKeyboardUp(e);
	}

	return true;
}

/** Mouse input */
bool PlayState::mouseMoved(const OIS::MouseEvent& e)
{
	mCameraManager->freeCameraMouse(e);
	return true;
}

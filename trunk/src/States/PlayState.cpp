#include "..\include\States\PlayState.h"

using namespace WyvernsAssault;

PlayState::PlayState(GraphicsManagerPtr graphicsManager, InputManagerPtr inputManager, AudioManagerPtr audioManager, CameraManagerPtr cameraManager, GuiManagerPtr guiManager)
: BaseState(graphicsManager,inputManager,audioManager, cameraManager, guiManager)
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

	//
	// Use Game camera and viewport (by default Gui ones are used!)
	//
	mCamera = mCameraManager->getGameCamera();
	mViewport = mCameraManager->getGameViewport();

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
	
	//
	// Gui for this state
	//
	mGuiScreen = mGuiManager->createGui(GuiScreenId::PlayGui, "PlayScreen");
	mPlayerUI = GuiUserInterfacePtr( new GuiUserInterface(mWindow->getViewport(0)->getCamera()->getAspectRatio(), GuiScreenId::PlayGui, GuiWidgetPlayId::UserInterface1) );
	mGuiScreen->addWidget(mPlayerUI,GuiWidgetPlayId::UserInterface1);

	// Player manager constructor
	mPlayerManager = PlayerManagerPtr(new PlayerManager(mSceneManager));
	mPlayerManager->initialize();

	// Create a single player (TEST!)
	PlayerPtr player1 = mPlayerManager->createPlayer("Player1","redWyvern.mesh");
	player1->setPosition(Vector3(80, 20, 870));
	// Add particle systems
	player1->setFireBreath(mParticleManager->create("firebreath","WyvernsAssault/DragonBreath"));
	player1->setDust(mParticleManager->create("dustR","WyvernsAssault/Dust"), mParticleManager->create("dustL","WyvernsAssault/Dust"));
	// Set player gui
	player1->setGuiId(GuiWidgetPlayId::UserInterface1);

	// Lights manager constructor
	mLightsManager = LightsManagerPtr(new LightsManager(mSceneManager));
	mLightsManager->initialize();

	//
	// Physics Manager
	// 
	mPhysicsManager = PhysicsManagerPtr(new PhysicsManager(mSceneManager));
	mPhysicsManager->initialize();
	mPhysicsManager->addPhysicPlayer(player1);

	//Enemies manager constructor
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

	//
	// Create ScenarioManager
	//
	mScenarioManager = ScenarioManagerPtr(new ScenarioManager(mSceneManager));
	mScenarioManager->initialize();

	//
	// Cut scene manager
	//
	mCutSceneManager = CutSceneManagerPtr(new CutSceneManager(mSceneManager));
	mCutSceneManager->initialize();

	//
	// Events Manager 
	//
	mEventsManager = EventsManagerPtr(new EventsManager());
	
	//
	// Register all events listeners/callers
	//
	mEventsManager->registerInterface(mEnemyManager);
	mEventsManager->registerInterface(mPhysicsManager);
	mEventsManager->registerInterface(mParticleManager);
	mEventsManager->registerInterface(mAudioManager);
	mEventsManager->registerInterface(mItemManager);
	mEventsManager->registerInterface(mPlayerManager);
	mEventsManager->registerInterface(mCameraManager);
	mEventsManager->registerInterface(mGuiManager);
	mEventsManager->registerInterface(mPostProcessManager);
	mEventsManager->registerInterface(mCutSceneManager);

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
	mLuaManager->registerInterface(mPlayerManager);
	mLuaManager->registerInterface(mCutSceneManager);
	mLuaManager->registerInterface(mAudioManager);
	mLuaManager->registerInterface(mGuiManager);
	mLuaManager->registerInterface(mPostProcessManager);
	mLuaManager->registerInterface(mCameraManager);
	mLuaManager->registerInterface(mInputManager);

	//
	// THIRD :	This call to 'initialize' will initialize Lua,
	//			open default Lua libs, add custom libs exported by
	//			our registered LuaInterfaces, setup the environment.
	//
	mLuaManager->initialize();

	//
	// Load scene!
	//
	boost::scoped_ptr<DotSceneLoader> dotSceneLoader ( new DotSceneLoader );
	dotSceneLoader->parseDotScene("Level1_1.scene","General", mSceneManager, mScenarioManager, mCameraManager, mLightsManager, mEnemyManager, mPhysicsManager, mItemManager, mParticleManager, mScenarioManager->_getSceneNode());

	//dotSceneLoader->parseDotScene("Level1_1_scenario.scene","General", mSceneManager, mScenarioManager, mCameraManager, mLightsManager, mEnemyManager, mPhysicsManager, mItemManager, mParticleManager, mScenarioManager->_getSceneNode());
	//dotSceneLoader->parseDotScene("Level1_1_enemies.scene","General", mSceneManager, mScenarioManager, mCameraManager, mLightsManager, mEnemyManager, mPhysicsManager, mItemManager, mParticleManager, mEnemyManager->_getSceneNode());
	dotSceneLoader->parseDotScene("Level1_1_cameras.scene","General", mSceneManager, mScenarioManager, mCameraManager, mLightsManager, mEnemyManager, mPhysicsManager, mItemManager, mParticleManager, mCameraManager->_getSceneNode());
	dotSceneLoader->parseDotScene("Level1_1_lights.scene","General", mSceneManager, mScenarioManager, mCameraManager, mLightsManager, mEnemyManager, mPhysicsManager, mItemManager, mParticleManager, mLightsManager->_getSceneNode());
	//dotSceneLoader->parseDotScene("Level1_1_items.scene","General", mSceneManager, mScenarioManager, mCameraManager, mLightsManager, mEnemyManager, mPhysicsManager, mItemManager, mParticleManager, mItemManager->_getSceneNode());
	dotSceneLoader->parseDotScene("Level1_1_physics.scene","General", mSceneManager, mScenarioManager, mCameraManager, mLightsManager, mEnemyManager, mPhysicsManager, mItemManager, mParticleManager, mPhysicsManager->_getSceneNode());
	dotSceneLoader->parseDotScene("Level1_1_objects.scene","General", mSceneManager, mScenarioManager, mCameraManager, mLightsManager, mEnemyManager, mPhysicsManager, mItemManager, mParticleManager, mPhysicsManager->_getSceneNode());

	Debug::Out(mSceneManager->getRootSceneNode());

	//
	// Set game camera
	//
	mCameraManager->gameCamera();

	Viewport* gameViewport = mCameraManager->getGameViewport();
	//
	// Audio: playState track
	//
	mAudioManager->playSoundTrack("main_track.mp3");

	buttonTimer = 0.0;
	lastKey = OIS::KeyCode::KC_UNASSIGNED;
}

/** Load resources */
void PlayState::load()
{
	//
	// TODO: Load?
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
			mDetailsPanel->setParamValue(4, mCameraManager->getCameraModeString().c_str());
			mDetailsPanel->setParamValue(5, StringConverter::toString(mCameraManager->getCameraPosition().x, 5));
			mDetailsPanel->setParamValue(6, StringConverter::toString(mCameraManager->getCameraPosition().y, 5));
			mDetailsPanel->setParamValue(7, StringConverter::toString(mCameraManager->getCameraPosition().z, 5));
			mDetailsPanel->setParamValue(8, StringConverter::toString(mCameraManager->getCameraLookAt().x, 5));
			mDetailsPanel->setParamValue(9, StringConverter::toString(mCameraManager->getCameraLookAt().y, 5));
			mDetailsPanel->setParamValue(10, StringConverter::toString(mCameraManager->getCameraLookAt().z, 5));
		}
	}

	if(mCameraManager->getCameraMode() == CameraManager::CameraModes::Free)
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
		if(mCameraManager->getCameraMode() == CameraManager::CameraModes::Game)
		{
			// Trick for key hit inaccuracies
			buttonTimer += elapsedSeconds;
			if ( buttonTimer >= 0.2)
			{
				buttonTimer = 0;
				lastKey = OIS::KeyCode::KC_UNASSIGNED;
			}

			// Special Attack
			if ( this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_LCONTROL) && lastKey != OIS::KeyCode::KC_LCONTROL )
			{
				mPlayerManager->attackSpecial("Player1");
				lastKey = OIS::KeyCode::KC_LCONTROL;		
			}
			// Main attack
			else if( this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_SPACE) && lastKey != OIS::KeyCode::KC_SPACE )
			{
				mPlayerManager->attack("Player1");
				lastKey = OIS::KeyCode::KC_SPACE;				
			}

			// Movement if not attacking
			if( !player1->isAttacking() && !player1->isSpecial() )
			{
				// 8 directions move
				if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_RIGHT) && this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_UP))
				{
					mPlayerManager->move(PLAYER1,mCameraManager->getDirection(UP_RIGHT_DIRECTION));
					lastKey = OIS::KeyCode::KC_UP;
				}
				else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_RIGHT) && this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_DOWN))
				{
					mPlayerManager->move(PLAYER1,mCameraManager->getDirection(DOWN_RIGHT_DIRECTION));
					lastKey = OIS::KeyCode::KC_DOWN;
				}
				else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_LEFT) && this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_UP))
				{
					mPlayerManager->move(PLAYER1,mCameraManager->getDirection(UP_LEFT_DIRECTION));
					lastKey = OIS::KeyCode::KC_UP;
				}
				else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_LEFT) && this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_DOWN))
				{
					mPlayerManager->move(PLAYER1,mCameraManager->getDirection(DOWN_LEFT_DIRECTION));
					lastKey = OIS::KeyCode::KC_DOWN;
				}
				else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_RIGHT))
				{
					mPlayerManager->move(PLAYER1,mCameraManager->getDirection(RIGHT_DIRECTION));
					lastKey = OIS::KeyCode::KC_RIGHT;
				}
				else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_LEFT))
				{
					mPlayerManager->move(PLAYER1,mCameraManager->getDirection(LEFT_DIRECTION));
					lastKey = OIS::KeyCode::KC_LEFT;
				}
				else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_UP))
				{
					mPlayerManager->move(PLAYER1,mCameraManager->getDirection(UP_DIRECTION));
					lastKey = OIS::KeyCode::KC_UP;
				}
				else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_DOWN))
				{
					mPlayerManager->move(PLAYER1,mCameraManager->getDirection(DOWN_DIRECTION));
					lastKey = OIS::KeyCode::KC_DOWN;
				}
				else
				{
					// Iddle
					mPlayerManager->move(PLAYER1,Vector3::ZERO);
				}
			}
			else
			{
				// No movement
				mPlayerManager->stop(PLAYER1);
			}
		}
		// Other cameras not allowing movement
		else
		{
			// No movement, iddle animation
			mPlayerManager->move(PLAYER1,Vector3::ZERO);
			lastKey = OIS::KeyCode::KC_UNASSIGNED;
		}
	}
	else
	{
		mPlayerManager->stop(PLAYER1);

		if( !player1->isDying() )
			this->mNextGameStateId = GameStateId::GameOver;
	}

	/** Game Updates **/

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
	mPlayerManager->update(elapsedSeconds);

	//
	// Update camera
	//
	mCameraManager->updateCamera(player1->getPosition(), elapsedSeconds);

	//
	// Update sounds
	//
	mAudioManager->update(Vector3::ZERO, Quaternion::ZERO, elapsedSeconds);

	//
	// Update post processing compositors
	//
	mPostProcessManager->update(elapsedSeconds);

	//
	// Update CutScene Manager
	//
	mCutSceneManager->update(elapsedSeconds);

	//
	// Dispatch events. Managers have probably raised events, that are now in the 
	// EventsManager queue. The events manager will then dispatch them, calling
	// for each of them the registered handler(s).
	//
	mEventsManager->update(elapsedSeconds);
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
		// Remove gui
		//
		mGuiManager->removeGui(GuiScreenId::PlayGui);
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
	mScenarioManager.reset();

	mLightsManager.reset();

	mLuaManager.reset();

	mPlayerManager.reset();

	mEnemyManager.reset();

	if(mTrayMgr)
	{
		delete mTrayMgr;
		mTrayMgr = NULL;
	}

	mGuiScreen.reset();

	mPhysicsManager.reset();

	mItemManager.reset();

	mLogicManager.reset();

	mEventsManager.reset();

	mParticleManager.reset();

	mPostProcessManager.reset();

	mCutSceneManager.reset();

	//mGraphicsManager->clearScene();

	BaseState::finalize();

	Debug::Out("**************************");

	//Debug::Out(mSceneManager->getRootSceneNode());
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

	// Pause all particles

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
		player1->die();		
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
			mPhysicsManager->setDebugEnabled(true);
			mItemManager->setDebugEnabled(true);
			mScenarioManager->setDebugEnabled(true);
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
			mPhysicsManager->setDebugEnabled(false);
			mItemManager->setDebugEnabled(false);
			mScenarioManager->setDebugEnabled(false);
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
	// Physics debug
	case OIS::KeyCode::KC_F:
		mPhysicsManager->showDebugObjects();
		break;
/*
	// Attack A
	case OIS::KeyCode::KC_SPACE:
		mPlayerManager->attack("Player1");
		break;
	// Special Attack
	case OIS::KeyCode::KC_LCONTROL:
		mPlayerManager->attackSpecial("Player1");
		break;
*/

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

	case OIS::KeyCode::KC_V:
		mCameraManager->shake(0.25, 20);
		break;
	case OIS::KeyCode::KC_B:
		mCameraManager->rumble(0.5, 5);
		break;
	case OIS::KeyCode::KC_N:
		mCameraManager->tremor(0.5, 20);
		break;
	case OIS::KeyCode::KC_M:
		mCameraManager->drunk(2, 15);
		break;
	}

	// Free camera mode move
	if(mCameraManager->getCameraMode() == CameraManager::CameraModes::Free)
	{
		mCameraManager->freeCameraKeyboardDown(e);
	}

	return true;
}

bool PlayState::keyReleased(const OIS::KeyEvent& e)
{
	// Free camera mode move
	if(mCameraManager->getCameraMode() == CameraManager::CameraModes::Free)
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

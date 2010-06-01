#include "..\include\States\PlayState.h"
#include "..\include\SceneLoader\DotSceneLoader.h"

using namespace WyvernsAssault;

PlayState::PlayState(GraphicsManager& graphicsManager, InputManager& inputManager, AudioManager& audioManager)
: BaseState(graphicsManager,inputManager,audioManager)
, mLogicManager(NULL)
, mLightsManager(NULL)
, mCameraManager(NULL)
, mTrayMgr(NULL)
, mDetailsPanel(NULL)
, mCompositorsEnabled(false)
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

	// Player manager constructor
	mPlayerManager = PlayerManagerPtr(new PlayerManager(mSceneManager));
	mPlayerManager->initialize();

	// Create a single player (TEST!)
	mPlayer1 = mPlayerManager->createPlayer("Player1","redWyvern.mesh");
	mPlayer1->setPosition(Vector3(80, 20, 870));
	// Add fire breath (TEST!)
	mPlayer1->setFireBreath(mParticleManager->create("firebreath","WyvernsAssault/DragonBreath"));

	// Camera manager constructor
	mCameraManager = new CameraManager(mSceneManager, mWindow, mViewport);
	mCameraManager->initialize();

	// Lights manager constructor
	mLightsManager = new LightsManager(mSceneManager);
	mLightsManager->initialize();

	//
	// Physics Manager
	// 
	mPhysicsManager = PhysicsManagerPtr(new PhysicsManager(mSceneManager));
	mPhysicsManager->initialize();
	mPhysicsManager->addPlayer(mPlayer1);

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
	mLogicManager = new LogicManager();
	mLogicManager->initialize();

	// -----------
	// Lua Manager
	// -----------
	// Create this manager after ALL other managers. Never alter
	// the oreder of the operations!
	//
	// FIRST : Instatniate the Lua Manager
	mLuaManager = new LuaManager();

	//
	// SECOND : Register all the LuaInterfaces you want. 
	//
	mLuaManager->registerInterface(mLightsManager);
	mLuaManager->registerInterface(mLogicManager);
	mLuaManager->registerInterface(mEnemyManager.get());
	mLuaManager->registerInterface(mPhysicsManager.get());
	mLuaManager->registerInterface(mItemManager.get());
	mLuaManager->registerInterface(mPlayerManager.get());

	//
	// THIRD :	This call to 'initialize' will initialize Lua,
	//			open default Lua libs, add custom libs exported by
	//			our registered LuaInterfaces, setup the environment.
	//
	mLuaManager->initialize();

	//Load scene XML file
	std::auto_ptr<DotSceneLoader> sceneLoader(new DotSceneLoader());
	sceneLoader->parseDotScene("Level1_1.scene","General", mSceneManager, mCameraManager, mLightsManager, mEnemyManager.get(), mPhysicsManager.get(), mItemManager.get(), mParticleManager.get());
	sceneLoader->parseDotScene("Stage1_1.XML","General", mSceneManager, mCameraManager, mLightsManager, mEnemyManager.get(), mPhysicsManager.get(), mItemManager.get(), mParticleManager.get());

	//
	// Events Manager 
	//
	mEventsManager = new EventsManager();
	
	//
	// Register all events listeners/callers
	//
	mEventsManager->registerInterface(mEnemyManager.get());
	mEventsManager->registerInterface(mPhysicsManager.get());
	mEventsManager->registerInterface(mParticleManager.get());
	mEventsManager->registerInterface(mAudioManager);
	mEventsManager->registerInterface(mItemManager.get());
	mEventsManager->registerInterface(mPlayerManager.get());

	//
	// Set game camera
	//
	mCameraManager->gameCamera(mPlayer1->getSceneNode()->getPosition());

	mGraphicsManager->addCompositor(COMPOSITOR);
	mGraphicsManager->setCompositorEnabled(COMPOSITOR, mCompositorsEnabled);

	//
	// Audio manager
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
			mDetailsPanel->setParamValue(0, StringConverter::toString(mPlayer1->getPosition().x, 5));
			mDetailsPanel->setParamValue(1, StringConverter::toString(mPlayer1->getPosition().y, 5));
			mDetailsPanel->setParamValue(2, StringConverter::toString(mPlayer1->getPosition().z, 5));
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
	if ( !mPlayer1->isDeath() )
	{
		// Movement
		if(mCameraManager->getCameraMode() == "Game")
		{
			// Move if not using special attack or attacking
			if(!mPlayer1->isSpecial() && !mPlayer1->isAttacking())
			{
				// 8 directions move
				if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_RIGHT) && this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_UP))
				{
					mPlayer1->move(1,0,-1);
					mPhysicsManager->move(mPlayer1, mCameraManager->getDirection(Vector3(1,0,-1)));
				}
				else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_RIGHT) && this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_DOWN))
				{
					mPlayer1->move(1,0,1);
					mPhysicsManager->move(mPlayer1, mCameraManager->getDirection(Vector3(1,0,1)));
				}
				else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_LEFT) && this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_UP))
				{
					mPlayer1->move(-1,0,-1);
					mPhysicsManager->move(mPlayer1, mCameraManager->getDirection(Vector3(-1,0,-1)));
				}
				else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_LEFT) && this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_DOWN))
				{
					mPlayer1->move(-1,0,1);
					mPhysicsManager->move(mPlayer1, mCameraManager->getDirection(Vector3(-1,0,1)));
				}
				else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_RIGHT))
				{
					mPlayer1->move(1,0,0);
					mPhysicsManager->move(mPlayer1, mCameraManager->getDirection(Vector3(1,0,0)));
				}
				else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_LEFT))
				{
					mPlayer1->move(-1,0,0);
					mPhysicsManager->move(mPlayer1, mCameraManager->getDirection(Vector3(-1,0,0)));
				}
				else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_UP))
				{
					mPlayer1->move(0,0,-1);
					mPhysicsManager->move(mPlayer1, mCameraManager->getDirection(Vector3(0,0,-1)));
				}
				else if(this->mInputManager->getKeyboard()->isKeyDown(OIS::KeyCode::KC_DOWN))
				{
					mPlayer1->move(0,0,1);
					mPhysicsManager->move(mPlayer1, mCameraManager->getDirection(Vector3(0,0,1)));
				}		
				else
				{
					// No movement, iddle animation
					mPlayer1->move(0,0,0);
					mPhysicsManager->move(mPlayer1, Vector3::ZERO);
				}
			}
			else
			{
				// No movement
				mPhysicsManager->move(mPlayer1, Vector3::ZERO);
			}
		}
		else
		{
			// No movement, iddle animation
			mPlayer1->move(0,0,0);
			mPhysicsManager->move(mPlayer1, Vector3(0,0,0));
		}
	}
	else
	{
		if( !mPlayer1->isDying() )
			this->mNextGameStateId = GameStateId::GameOver;
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
	mPlayer1->updateAnimation(elapsedSeconds);

	//
	// Update camera
	//
	mCameraManager->updateCamera(mPlayer1->getSceneNode()->getPosition(), elapsedSeconds);

	//
	// Update sounds
	//
	mAudioManager->update(mCameraManager->getCamera()->getParentSceneNode(), elapsedSeconds);

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
	// FIRST!
	if(mLuaManager)
	{
		delete mLuaManager;
		mLuaManager = NULL;
	}

	mPlayerManager.reset();

	
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

	if(mLogicManager)
	{
		delete mLogicManager;
		mLogicManager = NULL;
	}

	if(mEventsManager)
	{
		//delete mEventsManager;
		mEventsManager = NULL;
	}

	if(mParticleManager)
	{	
		mParticleManager->finalize();
		mParticleManager.reset();
	}

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
}

/** Buffered input - keyboard key clicked */
bool PlayState::keyPressed(const OIS::KeyEvent& e)
{
	String newVal;
	PolygonMode pm;
	TextureFilterOptions tfo;
	unsigned int aniso;

	switch(e.key)
	{
	case OIS::KeyCode::KC_I:
		this->mNextGameStateId = GameStateId::Ending;
		break;
	case OIS::KeyCode::KC_D:
		mPlayer1->Die();		
		break;
	// Pause menu
	case OIS::KeyCode::KC_P:		
		this->mNextGameStateId = GameStateId::Pause;
		break;
	// Camera keys
	case OIS::KeyCode::KC_1:		
		mCameraManager->gameCamera(mPlayer1->getSceneNode()->getPosition());
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
	case OIS::KeyCode::KC_C:
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

	// Compositors On/Off
	case OIS::KeyCode::KC_K:
		mCompositorsEnabled = !mCompositorsEnabled;

		mGraphicsManager->setCompositorEnabled(COMPOSITOR, mCompositorsEnabled);
		break;	

	// Camera rumble - Debug
	case OIS::KeyCode::KC_B:
		mCameraManager->rumble(5);
		break;
	// Camera tremor - Debug
	case OIS::KeyCode::KC_V:
		mCameraManager->tremor(5);
		break;

	// Blood particle system - Debug
	case OIS::KeyCode::KC_Z:		
		mParticleManager->bloodKill(mPlayer1->getSceneNode());
		break;
	case OIS::KeyCode::KC_X:
		mParticleManager->hit(mPlayer1->getSceneNode());
		break;
	case OIS::KeyCode::KC_Y:
		mParticleManager->bloodHit(mPlayer1->getSceneNode());
		break;
	case OIS::KeyCode::KC_L:
		mParticleManager->bloodHit(mPlayer1->getSceneNode());
		mParticleManager->bloodKill(mPlayer1->getSceneNode());
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

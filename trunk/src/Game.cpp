/*
-----------------------------------------------------------------------------
Wyverns' Assault 3D Videgame.
Copyright (C) 2010  Giorgio Tino, Javier Soto Huesa, Jordi Carreras Ribot, 
					Marc Serena, Elm Oliver Torres

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

-----------------------------------------------------------------------------
*/
#include "Game.h"

#include "LoadingBar.h"
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "../resources/resource.h"
#endif

template<> Game* Ogre::Singleton<Game>::ms_Singleton = 0;

CEGUI::MouseButton convertOISMouseButtonToCegui(int buttonID)
{
    switch (buttonID)
    {
	case 0: return CEGUI::LeftButton;
	case 1: return CEGUI::RightButton;
	case 2:	return CEGUI::MiddleButton;
	case 3: return CEGUI::X1Button;
	default: return CEGUI::LeftButton;
    }
}

//-------------------------------------------------------------------------------------
Game::Game(void)
	: mRoot(0),
	mCamera(0),
	mSceneMgr(0),
	mWindow(0),
	mLoadingBar(0),
	mGUISystem(0),
	mGUIRenderer(0),
	mShutdownRequested(false),
	mSceneDetailIndex(0),
	mMoveSpeed(100),
	mRotateSpeed(36),
	mDebugOverlay(0),
	mDebugText(""),
	mInputManager(0),
	mMouse(0),
	mKeyboard(0),
	mTranslateVector(Vector3::ZERO),
	mStatsOn(true),
	mUseBufferedInputKeys(false),
	mUseBufferedInputMouse(false),
	mInputTypeSwitchingOn(false),
	mNumScreenShots(0),
	mMoveScale(0.0f),
	mRotScale(0.0f),
	mTimeUntilNextToggle(0),
	mRotX(0),
	mRotY(0),
	mFiltering(TFO_BILINEAR),
	mAniso(1)
{
}

//-------------------------------------------------------------------------------------
Game::~Game(void)
{
	//Remove ourself as a Window listener
	WindowEventUtilities::removeWindowEventListener(mWindow, this);
	windowClosed(mWindow);
	if(mGUISystem)
	{
		delete mGUISystem;
		mGUISystem = 0;
	}
	if(mGUIRenderer)
	{
		delete mGUIRenderer;
		mGUIRenderer = 0;
	}
	delete mRoot;
}

//-------------------------------------------------------------------------------------
bool Game::configure(void)
{
	// Show the configuration dialog and initialise the system
	// You can skip this and use root.restoreConfig() to load configuration
	// settings if you were sure there are valid ones saved in ogre.cfg
		if(mRoot->restoreConfig() || mRoot->showConfigDialog())
		{
		// If returned true, user clicked OK so initialise
		// Here we choose to let the system create a default rendering window by passing 'true'
		mWindow = mRoot->initialise(true);

		// Let's add a nice window icon
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		HWND hwnd;
		mWindow->getCustomAttribute("WINDOW", (void*)&hwnd);
		LONG iconID   = (LONG)LoadIcon( GetModuleHandle(0), MAKEINTRESOURCE(IDI_APPICON) );
		SetClassLong( hwnd, GCL_HICON, iconID );
#endif
		return true;
	}
	else
	{
		return false;
	}
}
//-------------------------------------------------------------------------------------
void Game::chooseSceneManager(void)
{
	// Get the SceneManager, in this case a generic one
	mSceneMgr = mRoot->createSceneManager(ST_GENERIC);
}
//-------------------------------------------------------------------------------------
void Game::createCamera(void)
{
	// Create the camera
	mCamera = mSceneMgr->createCamera("PlayerCam");

	// Position it at 500 in Z direction
	mCamera->setPosition(Vector3(0,0,80));
	// Look back along -Z
	mCamera->lookAt(Vector3(0,0,-300));
	mCamera->setNearClipDistance(5);

}
//-------------------------------------------------------------------------------------
void Game::createFrameListener(void)
{
	mDebugOverlay = OverlayManager::getSingleton().getByName("Core/DebugOverlay");
	mUseBufferedInputKeys = false;
	mUseBufferedInputMouse = true;
	mInputTypeSwitchingOn = mUseBufferedInputKeys || mUseBufferedInputMouse;
	mRotateSpeed = 36;
	mMoveSpeed = 100;

	using namespace OIS;

	LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
	ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	mInputManager = InputManager::createInputSystem( pl );

	mKeyboard = static_cast<Keyboard*>(mInputManager->createInputObject( OISKeyboard, mUseBufferedInputKeys ));
	mMouse = static_cast<Mouse*>(mInputManager->createInputObject( OISMouse, mUseBufferedInputMouse ));

	mMouse->setEventCallback(this);
	mKeyboard->setEventCallback(this);

	mShutdownRequested = false;
	//Set initial mouse clipping size
	windowResized(mWindow);

	//Register as a Window listener
	WindowEventUtilities::addWindowEventListener(mWindow, this);

	mStatsOn = true;
	mNumScreenShots = 0;
	mTimeUntilNextToggle = 0;
	mSceneDetailIndex = 0;
	mMoveScale = 0.0f;
	mRotScale = 0.0f;
	mTranslateVector = Vector3::ZERO;
	mAniso = 1;
	mFiltering = TFO_BILINEAR;

	showDebugOverlay(true);
	mRoot->addFrameListener(this);
}
//-------------------------------------------------------------------------------------
void Game::createScene(void)
{
}
//-------------------------------------------------------------------------------------
void Game::destroyScene(void)
{
}
//-------------------------------------------------------------------------------------
void Game::createViewports(void)
{
	// Create one viewport, entire window
	Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(ColourValue(0,0,0));

	// Alter the camera aspect ratio to match the viewport
	mCamera->setAspectRatio(
		Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
}
//-------------------------------------------------------------------------------------
void Game::setupResources(void)
{
	// Load resource paths from config file
	ConfigFile cf;
	cf.load("resources.cfg");

	// Go through all sections & settings in the file
	ConfigFile::SectionIterator seci = cf.getSectionIterator();

	String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		ConfigFile::SettingsMultiMap *settings = seci.getNext();
		ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			ResourceGroupManager::getSingleton().addResourceLocation(
				archName, typeName, secName);
		}
	}
}
//-------------------------------------------------------------------------------------
void Game::createResourceListener(void)
{

}
//-------------------------------------------------------------------------------------
void Game::loadResources(void)
{
	mLoadingBar = new LoadingBar();

	mLoadingBar->start(mWindow, 1, 1, 0.75);

	// Turn off rendering of everything except overlays
	mSceneMgr->clearSpecialCaseRenderQueues();
	mSceneMgr->addSpecialCaseRenderQueue(RENDER_QUEUE_OVERLAY);
	mSceneMgr->setSpecialCaseRenderQueueMode(SceneManager::SCRQM_INCLUDE);

	// Initialise the rest of the resource groups, parse scripts etc
	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	ResourceGroupManager::getSingleton().loadResourceGroup(
		ResourceGroupManager::getSingleton().getWorldResourceGroupName(),
		false, true);

	// Back to full rendering
	mSceneMgr->clearSpecialCaseRenderQueues();
	mSceneMgr->setSpecialCaseRenderQueueMode(SceneManager::SCRQM_EXCLUDE);

	mLoadingBar->finish();
	delete mLoadingBar;
}
//-------------------------------------------------------------------------------------
void Game::go(void)
{
	if (!setup())
		return;

	showDebugOverlay(true);

	mRoot->startRendering();

	// clean up
	destroyScene();
}
//-------------------------------------------------------------------------------------
bool Game::setup(void)
{
	mRoot = new Root();

	setupResources();

	bool carryOn = configure();
	if (!carryOn) return false;

	chooseSceneManager();
	createCamera();
	createViewports();

	// Set default mipmap level (NB some APIs ignore this)
	TextureManager::getSingleton().setDefaultNumMipmaps(5);

	// Create any resource listeners (for loading screens)
	createResourceListener();
	// Load resources
	loadResources();

	// Create the scene
	createScene();

	createFrameListener();

	return true;
};
//-------------------------------------------------------------------------------------
void Game::updateStats(void)
{
	static String currFps = "Current FPS: ";
	static String avgFps = "Average FPS: ";
	static String bestFps = "Best FPS: ";
	static String worstFps = "Worst FPS: ";
	static String tris = "Triangle Count: ";

	// update stats when necessary
	try {
		OverlayElement* guiAvg = OverlayManager::getSingleton().getOverlayElement("Core/AverageFps");
		OverlayElement* guiCurr = OverlayManager::getSingleton().getOverlayElement("Core/CurrFps");
		OverlayElement* guiBest = OverlayManager::getSingleton().getOverlayElement("Core/BestFps");
		OverlayElement* guiWorst = OverlayManager::getSingleton().getOverlayElement("Core/WorstFps");

		const RenderTarget::FrameStats& stats = mWindow->getStatistics();

		guiAvg->setCaption(avgFps + StringConverter::toString(stats.avgFPS));
		guiCurr->setCaption(currFps + StringConverter::toString(stats.lastFPS));
		guiBest->setCaption(bestFps + StringConverter::toString(stats.bestFPS)
			+" "+StringConverter::toString(stats.bestFrameTime)+" ms");
		guiWorst->setCaption(worstFps + StringConverter::toString(stats.worstFPS)
			+" "+StringConverter::toString(stats.worstFrameTime)+" ms");

		OverlayElement* guiTris = OverlayManager::getSingleton().getOverlayElement("Core/NumTris");
		guiTris->setCaption(tris + StringConverter::toString(stats.triangleCount));

		OverlayElement* guiDbg = OverlayManager::getSingleton().getOverlayElement("Core/DebugText");
		guiDbg->setCaption(mDebugText);
	}
	catch(...)
	{
		// ignore
	}
}
//-------------------------------------------------------------------------------------
bool Game::processUnbufferedKeyInput(const FrameEvent& evt)
{
	using namespace OIS;
	OIS::Keyboard* mInputDevice = mKeyboard; // Nice hack, eh? :)

	if (mInputDevice->isKeyDown(KC_A))
	{
		// Move camera left
		mTranslateVector.x = -mMoveScale;
	}

	if (mInputDevice->isKeyDown(KC_D))
	{
		// Move camera RIGHT
		mTranslateVector.x = mMoveScale;
	}

	/* Move camera forward by keypress. */
	if (mInputDevice->isKeyDown(KC_UP) || mInputDevice->isKeyDown(KC_W) )
	{
		mTranslateVector.z = -mMoveScale;
	}

	/* Move camera backward by keypress. */
	if (mInputDevice->isKeyDown(KC_DOWN) || mInputDevice->isKeyDown(KC_S) )
	{
		mTranslateVector.z = mMoveScale;
	}

	if (mInputDevice->isKeyDown(KC_PGUP))
	{
		// Move camera up
		mTranslateVector.y = mMoveScale;
	}

	if (mInputDevice->isKeyDown(KC_PGDOWN))
	{
		// Move camera down
		mTranslateVector.y = -mMoveScale;
	}

	if (mInputDevice->isKeyDown(KC_RIGHT))
	{
		mCamera->yaw(-mRotScale);
	}

	if (mInputDevice->isKeyDown(KC_LEFT))
	{
		mCamera->yaw(mRotScale);
	}

	if( mInputDevice->isKeyDown( KC_ESCAPE) )
	{
		return false;
	}

	// see if switching is on, and you want to toggle
	if (mInputTypeSwitchingOn && mInputDevice->isKeyDown(KC_M) && mTimeUntilNextToggle <= 0)
	{
		switchMouseMode();
		mTimeUntilNextToggle = 1;
	}

	if (mInputTypeSwitchingOn && mInputDevice->isKeyDown(KC_K) && mTimeUntilNextToggle <= 0)
	{
		// must be going from immediate keyboard to buffered keyboard
		switchKeyMode();
		mTimeUntilNextToggle = 1;
	}
	if (mInputDevice->isKeyDown(KC_F) && mTimeUntilNextToggle <= 0)
	{
		mStatsOn = !mStatsOn;
		showDebugOverlay(mStatsOn);

		mTimeUntilNextToggle = 1;
	}
	if (mInputDevice->isKeyDown(KC_T) && mTimeUntilNextToggle <= 0)
	{
		switch(mFiltering)
		{
		case TFO_BILINEAR:
			mFiltering = TFO_TRILINEAR;
			mAniso = 1;
			break;
		case TFO_TRILINEAR:
			mFiltering = TFO_ANISOTROPIC;
			mAniso = 8;
			break;
		case TFO_ANISOTROPIC:
			mFiltering = TFO_BILINEAR;
			mAniso = 1;
			break;
		default:
			break;
		}
		MaterialManager::getSingleton().setDefaultTextureFiltering(mFiltering);
		MaterialManager::getSingleton().setDefaultAnisotropy(mAniso);


		showDebugOverlay(mStatsOn);

		mTimeUntilNextToggle = 1;
	}

	if (mInputDevice->isKeyDown(KC_SYSRQ) && mTimeUntilNextToggle <= 0)
	{
		std::ostringstream ss;
		ss << "screenshot_" << ++mNumScreenShots << ".png";
		mWindow->writeContentsToFile(ss.str());
		mTimeUntilNextToggle = 0.5;
		mDebugText = "Saved: " + ss.str();
	}

	if (mInputDevice->isKeyDown(KC_R) && mTimeUntilNextToggle <=0)
	{
		mSceneDetailIndex = (mSceneDetailIndex+1)%3 ;
		switch(mSceneDetailIndex) {
				case 0 : mCamera->setPolygonMode(PM_SOLID) ; break ;
				case 1 : mCamera->setPolygonMode(PM_WIREFRAME) ; break ;
				case 2 : mCamera->setPolygonMode(PM_POINTS) ; break ;
		}
		mTimeUntilNextToggle = 0.5;
	}

	static bool displayCameraDetails = false;
	if (mInputDevice->isKeyDown(KC_P) && mTimeUntilNextToggle <= 0)
	{
		displayCameraDetails = !displayCameraDetails;
		mTimeUntilNextToggle = 0.5;
		if (!displayCameraDetails)
			mDebugText = "";
	}
	if (displayCameraDetails)
	{
		// Print camera details
		mDebugText = "P: " + StringConverter::toString(mCamera->getDerivedPosition()) +
			" " + "O: " + StringConverter::toString(mCamera->getDerivedOrientation());
	}

	// Return true to continue rendering
	return true;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
bool Game::processUnbufferedMouseInput(const FrameEvent& evt)
{
	using namespace OIS;

	// Rotation factors, may not be used if the second mouse button is pressed
	// 2nd mouse button - slide, otherwise rotate
	const MouseState &ms = mMouse->getMouseState();
	if( ms.buttonDown( MB_Right ) )
	{
		mTranslateVector.x += ms.X.rel * 0.13;
		mTranslateVector.y -= ms.Y.rel * 0.13;
	}
	else
	{
		mRotX = Degree(-ms.X.rel * 0.13);
		mRotY = Degree(-ms.Y.rel * 0.13);
	}

	return true;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
void Game::moveCamera()
{

	// Make all the changes to the camera
	// Note that YAW direction is around a fixed axis (freelook style) rather than a natural YAW (e.g. airplane)
	mCamera->yaw(mRotX);
	mCamera->pitch(mRotY);
	mCamera->moveRelative(mTranslateVector);


}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
void Game::showDebugOverlay(bool show)
{
	if (mDebugOverlay)
	{
		if (show)
		{
			mDebugOverlay->show();
		}
		else
		{
			mDebugOverlay->hide();
		}
	}
}
//-------------------------------------------------------------------------------------
void Game::requestShutdown(void)
{
	mShutdownRequested = true;
}
//-------------------------------------------------------------------------------------
bool Game::frameStarted(const FrameEvent& evt)
{
	if(mWindow->isClosed())
		return false;

	//Need to capture/update each device
	mKeyboard->capture();
	mMouse->capture();

	mUseBufferedInputMouse = mMouse->buffered();
	mUseBufferedInputKeys = mKeyboard->buffered();


      if(mUseBufferedInputMouse)
      {
         CEGUI::MouseCursor::getSingleton().show( );
      }
      else
      {
         CEGUI::MouseCursor::getSingleton().hide( );
      }

	if ( !mUseBufferedInputMouse || !mUseBufferedInputKeys)
	{
		// one of the input modes is immediate, so setup what is needed for immediate mouse/key movement
		if (mTimeUntilNextToggle >= 0)
			mTimeUntilNextToggle -= evt.timeSinceLastFrame;

		// If this is the first frame, pick a speed
		if (evt.timeSinceLastFrame == 0)
		{
			mMoveScale = 1;
			mRotScale = 0.1;
		}
		// Otherwise scale movement units by time passed since last frame
		else
		{
			// Move about 100 units per second,
			mMoveScale = mMoveSpeed * evt.timeSinceLastFrame;
			// Take about 10 seconds for full rotation
			mRotScale = mRotateSpeed * evt.timeSinceLastFrame;
		}
		mRotX = 0;
		mRotY = 0;
		mTranslateVector = Vector3::ZERO;
	}

	if (mUseBufferedInputKeys)
	{
		// no need to do any processing here, it is handled by event processor and
		// you get the results as KeyEvents
	}
	else
	{
		if (processUnbufferedKeyInput(evt) == false)
		{
			return false;
		}
	}
	if (mUseBufferedInputMouse)
	{
		// no need to do any processing here, it is handled by event processor and
		// you get the results as MouseEvents
	}
	else
	{
		if (processUnbufferedMouseInput(evt) == false)
		{
			return false;
		}
	}

	if ( !mUseBufferedInputMouse || !mUseBufferedInputKeys)
	{
		// one of the input modes is immediate, so update the movement vector

		moveCamera();

	}

	return true;
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
bool Game::frameEnded(const FrameEvent& evt)
{
	if (mShutdownRequested)
		return false;
	updateStats();
	return true;
}
//-------------------------------------------------------------------------------------
void Game::switchMouseMode()
{
	mUseBufferedInputMouse = !mUseBufferedInputMouse;
	mMouse->setBuffered(mUseBufferedInputMouse);
}
//-------------------------------------------------------------------------------------
void Game::switchKeyMode()
{
	mUseBufferedInputKeys = !mUseBufferedInputKeys;
	mKeyboard->setBuffered(mUseBufferedInputKeys);
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
bool Game::keyPressed( const OIS::KeyEvent &arg )
{
	if( arg.key == OIS::KC_ESCAPE )
		mShutdownRequested = true;
	CEGUI::System::getSingleton().injectKeyDown( arg.key );
	CEGUI::System::getSingleton().injectChar( arg.text );
	return true;
}

bool Game::keyReleased( const OIS::KeyEvent &arg )
{
	if( arg.key == OIS::KC_M )
		mMouse->setBuffered( !mMouse->buffered() );
	else if( arg.key == OIS::KC_K )
		mKeyboard->setBuffered( !mKeyboard->buffered() );
		CEGUI::System::getSingleton().injectKeyUp( arg.key );
	return true;
}

bool Game::mouseMoved( const OIS::MouseEvent &arg )
{
	CEGUI::System::getSingleton().injectMouseMove( arg.state.X.rel, arg.state.Y.rel );
	return true;
}

bool Game::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	CEGUI::System::getSingleton().injectMouseButtonDown(convertOISMouseButtonToCegui(id));
	return true;
}

bool Game::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	CEGUI::System::getSingleton().injectMouseButtonUp(convertOISMouseButtonToCegui(id));
	return true;
}

//Adjust mouse clipping area
void Game::windowResized(RenderWindow* rw)
{
	unsigned int width, height, depth;
	int left, top;
	rw->getMetrics(width, height, depth, left, top);

	const OIS::MouseState &ms = mMouse->getMouseState();
	ms.width = width;
	ms.height = height;
}

//Unattach OIS before window shutdown (very important under Linux)
void Game::windowClosed(RenderWindow* rw)
{
	//Only close for window that created OIS (the main window in these demos)
	if( rw == mWindow )
	{
		if( mInputManager )
		{
			mInputManager->destroyInputObject( mMouse );
			mInputManager->destroyInputObject( mKeyboard );

			OIS::InputManager::destroyInputSystem(mInputManager);
			mInputManager = 0;
		}
	}
}
//-------------------------------------------------------------------------------------
void Game::setupEventHandlers(void)
{
}
//-------------------------------------------------------------------------------------
bool Game::handleQuit(const CEGUI::EventArgs& e)
{
	requestShutdown();
	return true;
}

#include "..\include\GUI\GuiManager.h"
#include <Ogre.h>
#include <CEGUI/CEGUI.h>
#include <OgreCEGUIRenderer.h>
#include <OgreRenderWindow.h>
#include <OIS/OIS.h>

using namespace Ogre;
using namespace CEGUI;
using namespace WyvernsAssault;

GuiManager::GuiManager(Ogre::Root* root, Ogre::RenderWindow* window)
{
	//
	// TODO Constructor
	//
	mRoot = root;
	mWindow = window;
}

GuiManager::~GuiManager()
{
	//
	// TODO Destructor
	//
}

bool GuiManager::initialize()
{
	CEGUI::OgreCEGUIRenderer* m_GUIrenderer = new CEGUI::OgreCEGUIRenderer(mWindow,Ogre::RENDER_QUEUE_OVERLAY, false, 3000);
	mSystem  =new CEGUI::System(m_GUIrenderer);
	CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative); // this is recomenned to help with debugging, but not necessary
	// Create the root
	//mRoot = new Ogre::Root( WYVERNS_ASSAULT_PLUGINS_FILE, WYVERNS_ASSAULT_CONFIG_FILE);

	// Setup resource locations
	//setupResources();

	// Configure (configuration window)
	//bool carryOn = configure();
	//if (!carryOn) return false;

	// Pcik a scene manager
	//chooseSceneManager();

	// Set default mipmap level (NB some APIs ignore this)
	//TextureManager::getSingleton().setDefaultNumMipmaps(5);

	// Create any resource listeners (for loading screens)
	//createResourceListener();

	// Load resources
	//loadResources();

	return true;
}

/*void GuiManager::setupResources()
{
	// Load resource paths from config file
	ConfigFile cf;
	cf.load(WYVERNS_ASSAULT_RESOURCES_FILE);

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
}*/

/*bool GuiManager::configure()
{
	// Show the configuration dialog and initialise the system
	// You can skip this and use root.restoreConfig() to load configuration
	// settings if you were sure there are valid ones saved in ogre.cfg
	if(mRoot->showConfigDialog())
	{
		// If returned true, user clicked OK so initialise
		// Here we choose to let the system create a default rendering window by passing 'true'
		mRenderWindow = mRoot->initialise(true);

		// Let's add a nice window icon
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		HWND hwnd;
		mRenderWindow->getCustomAttribute("WINDOW", (void*)&hwnd);
		LONG iconID   = (LONG)LoadIcon( GetModuleHandle(0), MAKEINTRESOURCE(IDI_APPICON) );
		SetClassLong( hwnd, GCL_HICON, iconID );
#endif
		return true;
	}
	else
	{
		return false;
	}
}*/

/*void GuiManager::createResourceListener(void)
{
	//
	// TODO Create a resource listener
	//
}*/

void GuiManager::finalize()
{
	//
	// TODO finalize
	//
}

void GuiManager::loadResources()
{
	//load Scheme
	//CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"TaharezLookSkin.scheme");
	//mSystem->setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");

	//chooseMenu();
}

/*void GuiManager::unloadResources()
{
	//
	// TODO Unload resources
	//
}*/

/*void GuiManager::chooseSceneManager(void)
{
	// Get the SceneManager, in this case a generic one
	mSceneManager = mRoot->createSceneManager(ST_GENERIC);
}*/

/*Root* GuiManager::getRoot()
{
	return this->mRoot;
}*/

/*RenderWindow* GuiManager::getRenderWindow()
{
	return this->mRenderWindow;
}*/

/*SceneManager* GuiManager::getSceneManager()
{
	return this->mSceneManager;
}*/

/** Render one frame */
/*bool GuiManager::renderOneFrame()
{
	//return mRoot->renderOneFrame();
	return true;
}*/

void GuiManager::chooseMenu(){
	//CEGUI::Window* m_sheet = CEGUI::WindowManager::getSingleton().loadWindowLayout((CEGUI::utf8*)"ogregui.layout"); 
	//mSystem->setGUISheet(m_sheet);
	
	//WindowManager* Wmgr = WindowManager::getSingletonPtr();
	//System* mGUISystem = System::getSingletonPtr();
	//Window* myRoot = Wmgr->getWindow("RootWindow"); // get default window

	// Menu Background
	//Window* MenuBackground = Wmgr->createWindow("TaharezLook/StaticImage", "Background");
	//myRoot->addChildWindow( MenuBackground );
	//MenuBackground->setPosition( UVector2( UDim( 0.0f, 0.0f ), UDim( 0.0f, 0.0f) ) );
	//MenuBackground->setSize( UVector2( UDim( 1.0f, 0.0f ), UDim( 1.0f, 0.0f ) ) );  // full screen

	// New game Button 
	//PushButton* NewGame = (PushButton*)Wmgr->createWindow("TaharezLook/Button", "NewGame");
	//MenuBackground->addChildWindow( NewGame );
	//NewGame->setPosition( UVector2( UDim( 0.2f, 0.0f), UDim( 0.2f, 0.0f ) ) );
	//NewGame->setSize( UVector2( UDim( 0.4f, 0.0f ), UDim( 0.2f, 0.0f ) ) );
	
	// Options game Button 
	//PushButton* OptionsGame= (PushButton*)Wmgr->createWindow("TaharezLook/Button", "OptionsGame");
	//MenuBackground->addChildWindow( OptionsGame );
	//OptionsGame->setPosition( UVector2( UDim( 0.2f, 0.0f ), UDim( 0.45f, 0.0f ) ) );
	//OptionsGame->setSize( UVector2( UDim( 0.4f, 0.0f ), UDim( 0.2f, 0.0f ) ) );

	// Credits game Button 
	//PushButton* CreditsGame= (PushButton*)Wmgr->createWindow("TaharezLook/Button", "CreditsGame");
	//MenuBackground->addChildWindow( CreditsGame );
	//CreditsGame->setPosition( UVector2( UDim( 0.2f, 0.0f ), UDim( 0.45f, 0.0f ) ) );
	//CreditsGame->setSize( UVector2( UDim( 0.4f, 0.0f ), UDim( 0.2f, 0.0f ) ) );

	// Quit game Button 
	//PushButton* QuitGame= (PushButton*)Wmgr->createWindow("TaharezLook/Button", "QuitGame");
	//MenuBackground->addChildWindow( QuitGame );
	//QuitGame->setPosition( UVector2( UDim( 0.2f, 0.0f ), UDim( 0.7f, 0.0f ) ) );
	//QuitGame->setSize( UVector2( UDim( 0.4f, 0.0f ), UDim( 0.2f, 0.0f ) ) );

	//mSystem->setGUISheet(myRoot); // this line is redundant since you didn't change gui sheets, but its here to make sure
	
}

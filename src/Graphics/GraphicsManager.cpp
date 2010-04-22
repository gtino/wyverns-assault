#include "..\include\Graphics\GraphicsManager.h"

using namespace Ogre;
using namespace WyvernsAssault;

GraphicsManager::GraphicsManager()
: mRenderWindow(0)
, mRoot(0)
, mSceneManager(0)
{
	//
	// TODO Constructor
	//
}

GraphicsManager::~GraphicsManager()
{
	//
	// TODO Destructor
	//
}

bool GraphicsManager::initialize()
{
	// create an instance of LogManager prior to using LogManager::getSingleton()
	Ogre::LogManager* logMgr = new Ogre::LogManager;
	Ogre::Log *log = Ogre::LogManager::getSingleton().createLog(WYVERNS_ASSAULT_LOG_FILE, true, true, false);
	log->setLogDetail(Ogre::LL_BOREME);

	// Create the root
	mRoot = new Ogre::Root( WYVERNS_ASSAULT_PLUGINS_FILE, WYVERNS_ASSAULT_CONFIG_FILE);
	
	// Setup resource locations
	setupResources();

	// Configure (configuration window)
	bool carryOn = configure();
	if (!carryOn) return false;

	// Pick a scene manager
	chooseSceneManager();

	// Set default mipmap level (NB some APIs ignore this)
	TextureManager::getSingleton().setDefaultNumMipmaps(5);

	// Create any resource listeners (for loading screens)
	createResourceListener();

	// Load resources
	loadResources();

	return true;
}

void GraphicsManager::setupResources()
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
}

bool GraphicsManager::configure()
{
	// Show the configuration dialog and initialise the system
	// You can skip this and use root.restoreConfig() to load configuration
	// settings if you were sure there are valid ones saved in ogre.cfg
	//if(mRoot->restoreConfig())
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
}

void GraphicsManager::createResourceListener(void)
{
	//
	// TODO Create a resource listener
	//
}

void GraphicsManager::finalize()
{
	//
	// TODO finalize
	//
}

void GraphicsManager::loadResources()
{
	//mLoadingBar = new LoadingBar();

	//mLoadingBar->start(mWindow, 1, 1, 0.75);

	// Turn off rendering of everything except overlays
	mSceneManager->clearSpecialCaseRenderQueues();
	mSceneManager->addSpecialCaseRenderQueue(RENDER_QUEUE_OVERLAY);
	mSceneManager->setSpecialCaseRenderQueueMode(SceneManager::SCRQM_INCLUDE);

	// Initialise the rest of the resource groups, parse scripts etc
	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	ResourceGroupManager::getSingleton().loadResourceGroup(
		ResourceGroupManager::getSingleton().getWorldResourceGroupName(),
		false, true);

	// Back to full rendering
	mSceneManager->clearSpecialCaseRenderQueues();
	mSceneManager->setSpecialCaseRenderQueueMode(SceneManager::SCRQM_EXCLUDE);

	// Load debug fonts
	// get the font manager
	FontManager &fontMgr = FontManager::getSingleton();
	// create a font resource
	ResourcePtr bbFont = fontMgr.create("Fonts\GuiDebugText","General");
	// set as truetype
	bbFont->setParameter("type","truetype");
	// set the .ttf file name
	bbFont->setParameter("source","BADABB_.ttf");
	// set the size
	bbFont->setParameter("size","26");
	// set the dpi
	bbFont->setParameter("resolution","96");
	// load the ttf
	bbFont->load();
}

void GraphicsManager::unloadResources()
{
	//
	// TODO Unload resources
	//
}

void GraphicsManager::chooseSceneManager(void)
{
	// Get the SceneManager, in this case a generic one
	mSceneManager = mRoot->createSceneManager(ST_GENERIC, "Default");
}

Root* GraphicsManager::getRoot()
{
	return this->mRoot;
}

RenderWindow* GraphicsManager::getRenderWindow()
{
	return this->mRenderWindow;
}

SceneManager* GraphicsManager::getSceneManager()
{
	return this->mSceneManager;
}

/** Render one frame */
bool GraphicsManager::renderOneFrame()
{
	return mRoot->renderOneFrame();
}

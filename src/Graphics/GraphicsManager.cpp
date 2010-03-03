#include "..\include\Graphics\GraphicsManager.h"

using namespace Ogre;
using namespace WyvernsAssault;

GraphicsManager::GraphicsManager()
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
	// Create the root
	mRoot = new Root();

	// Configure (configuration window)
	bool carryOn = configure();
	if (!carryOn) return false;

	// Pcik a scene manager
	chooseSceneManager();

	// Set default mipmap level (NB some APIs ignore this)
	TextureManager::getSingleton().setDefaultNumMipmaps(5);

	// Create any resource listeners (for loading screens)
	createResourceListener();

	// Load resources
	loadResources();

	return true;
}

bool GraphicsManager::configure()
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

	//mLoadingBar->finish();
	//delete mLoadingBar;
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
	mSceneManager = mRoot->createSceneManager(ST_GENERIC);
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
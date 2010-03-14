#include "..\include\GUI\GuiManager.h"

using namespace Ogre;
using namespace WyvernsAssault;

GuiManager::GuiManager(Ogre::Root* root, Ogre::SceneManager* sceneManager, Ogre::RenderWindow* window)
{
	//
	// TODO Constructor
	//
	mRoot = root;
	mSceneManager = sceneManager;
	mWindow = window;
}

GuiManager::~GuiManager()
{
	//
	// TODO Destructor
	//
	finalize();
}

bool GuiManager::initialize()
{
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

//----------------------------------------------------------------//
bool GuiManager::mouseMoved( const OIS::MouseEvent &arg )
{
	return true;
}

//----------------------------------------------------------------//
bool GuiManager::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	return true;
}

//----------------------------------------------------------------//
bool GuiManager::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	return true;
}

//----------------------------------------------------------------//
bool GuiManager::keyPressed( const OIS::KeyEvent &arg )
{
	return true;
}

//----------------------------------------------------------------//
bool GuiManager::keyReleased( const OIS::KeyEvent &arg )
{
	return true;
}
#include "..\include\GUI\GuiManager.h"

using namespace Ogre;
using namespace WyvernsAssault;

// BEGIN SINGLETON
template<> GuiManager* Ogre::Singleton<GuiManager>::ms_Singleton = 0;
GuiManager* GuiManager::getSingletonPtr(void)
{
	return ms_Singleton;
}
GuiManager& GuiManager::getSingleton(void)
{  
	assert( ms_Singleton );  return ( *ms_Singleton );  
}
// END SINGLETON

GuiManager::GuiManager(Ogre::Root* root, Ogre::SceneManager* sceneManager, Ogre::RenderWindow* window)
{
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
	//
	// TODO initialize
	//
	return true;
}

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
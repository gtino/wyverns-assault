#include "..\include\Game.h"

using namespace WyvernsAssault;

Game::Game()
{
	//
	// TODO Constructor
	//
}

Game::~Game()
{
	//
	// TODO Destructor
	//
}

/** Initialize */ 
void Game::initialize()
{
	// Setup resources
	setupResources();

	// Initialize graphic manager
	mGraphicsManager.initialize();

	// Initialize Audio Layer
	mAudioManager.initialize();

	// Initialize input manager. A render window is needed in order to setup mouse coords and boundaries.
	mInputManager.initialize(mGraphicsManager.getRenderWindow(), true);

	// Initialize Game states (FSM) manager
	mStatesManager.initialize(mGraphicsManager, mInputManager, mAudioManager);

	// Load graphic resourcrs
	mGraphicsManager.loadResources();

	// Then we load audio
	mAudioManager.loadResources();
}

void Game::setupResources()
{
	// Load resource paths from config file
	ConfigFile cf;
	cf.load(WYVERN_ASSAULT_RESOURCES_FILE);

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

/** Main loop */
void Game::go()
{
	//
	// TODO Main game loop
	//
}

/** Finalize the game */
void Game::finalize()
{
	// Unregister and dispose of the StatesManager
	mStatesManager.finalize();
	// Unload all graphics data
	mGraphicsManager.unloadResources();
	// Finalize all
	mGraphicsManager.finalize();
	// Unacquire all inputs
	mInputManager.unacquireAll();
	// Finalize all
	mInputManager.finalize();
	// Unload audio/fx
	mAudioManager.unloadResources();
	// Finalize all
	mAudioManager.finalize();
}
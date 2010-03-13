#include "..\include\States\MainMenuState.h"

using namespace WyvernsAssault;

MainMenuState::MainMenuState(GraphicsManager& graphicsManager, InputManager& inputManager, AudioManager& audioManager)
: BaseState(graphicsManager,inputManager,audioManager)
{
	//
	// TODO Constructor logic HERE
	//
	this->mNextGameStateId = this->getStateId();
}

MainMenuState::~MainMenuState()
{
	//
	// TODO Distructor logic HERE
	//
	finalize();
}

/** Initialize current state */
void MainMenuState::initialize()
{
	//
	// TODO Initialize
	//
	this->mNextGameStateId = this->getStateId();

	mCamera = mGraphicsManager->getSceneManager()->createCamera( "DefaultCamera" );

	mViewport = mGraphicsManager->getRenderWindow()->addViewport( mCamera );

	mViewport->setBackgroundColour( Ogre::ColourValue( 1, 1, 1 ) );

	mGuiManager->initialize(mGraphicsManager->getRenderWindow());
}

/** Manage input */
void MainMenuState::input()
{
	//
	// TODO Read input
	//
	this->mInputManager->capture();
}

/** Load resources */
void MainMenuState::load()
{
	//
	// TODO Load 
	//
	//mGuiManager->loadResources();
	//mGuiManager->loadMenu();
	// Create background material
	mBackgroundMaterial = MaterialManager::getSingleton().create("MainMenuBackground", "General");
	mBackgroundMaterial->getTechnique(0)->getPass(0)->createTextureUnitState("MainMenu.png");
	mBackgroundMaterial->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
	mBackgroundMaterial->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
	mBackgroundMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);

	// Create background rectangle covering the whole screen
	mRectangle = new Rectangle2D(true);
	mRectangle->setCorners(-1.0, 1.0, 1.0, -1.0);
	mRectangle->setMaterial("MainMenuBackground");

	// Render the background before everything else
	mRectangle->setRenderQueueGroup(RENDER_QUEUE_BACKGROUND);

	// Use infinite AAB to always stay visible
	AxisAlignedBox aabInf;
	aabInf.setInfinite();
	mRectangle->setBoundingBox(aabInf);

	// Attach background to the scene
	mBackgroundNode = mGraphicsManager->getSceneManager()->getRootSceneNode()->createChildSceneNode("MainMenuBackground");
	mBackgroundNode->attachObject(mRectangle);

	// Example of background scrolling
	//material->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setScrollAnimation(-0.25, 0.0);
}

/** Update internal stuff */
void MainMenuState::update(const float elapsedSeconds)
{
	//
	// TODO Update
	//
}

/** Render */
void MainMenuState::render(const float elapsedSeconds)
{
	//
	// TODO Render
	//
}

/** Unload resources */
void MainMenuState::unload() 
{
	//
	// TODO Unload
	//
	mBackgroundNode->detachAllObjects();
}

/** Destroy the state */
void MainMenuState::finalize()
{
	//
	// TODO Destroy
	//
	mGraphicsManager->getSceneManager()->clearScene();

	mGraphicsManager->getSceneManager()->destroyAllCameras();

	mGraphicsManager->getRoot()->getAutoCreatedWindow()->removeAllViewports();
}

/** Get state Id */
GameStateId MainMenuState::getStateId()
{
	//
	// BEWARE! Make sure this is the correspondent state or we fuck up everything!
	//
	return GameStateId::MainMenu;
}

/** Called when the state has to be paused */
void MainMenuState::pause()
{
	//
	// TODO : Pause state
	//
}

/** Called when the state has to be resumed (from pause) */
void MainMenuState::resume()
{
	//
	// TODO : Resume state
	//
}

/** Buffered input - keyboard key clicked */
bool MainMenuState::keyReleased(const OIS::KeyEvent& e)
{
	switch(e.key)
	{
	case OIS::KC_P:
		this->mNextGameStateId = GameStateId::Play;
		break;
	case OIS::KC_E:
	case OIS::KC_ESCAPE:
		this->mNextGameStateId = GameStateId::Exit;
		break;
	}

	return true;
}

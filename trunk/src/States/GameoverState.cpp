#include "..\include\States\GameoverState.h"

using namespace WyvernsAssault;

GameoverState::GameoverState(GraphicsManager& graphicsManager, InputManager& inputManager, AudioManager& audioManager)
: BaseState(graphicsManager,inputManager,audioManager)
{
	//
	// TODO Constructor logic HERE
	//
}

GameoverState::~GameoverState()
{
	//
	// TODO Distructor logic HERE
	//
	exit();
}

/** Initialize current state */
void GameoverState::initialize()
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
void GameoverState::input()
{
	//
	// TODO Read input
	//
	this->mInputManager->capture();
}

/** Load resources */
void GameoverState::load()
{
	//
	// TODO Load 
	//
	// Create background material
	mBackgroundMaterial = MaterialManager::getSingleton().create("GameOverBackground", "General");
	mBackgroundMaterial->getTechnique(0)->getPass(0)->createTextureUnitState("GameOver.png");
	mBackgroundMaterial->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
	mBackgroundMaterial->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
	mBackgroundMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);

	// Create background rectangle covering the whole screen
	mRectangle = new Rectangle2D(true);
	mRectangle->setCorners(-1.0, 1.0, 1.0, -1.0);
	mRectangle->setMaterial("GameOverBackground");

	// Render the background before everything else
	mRectangle->setRenderQueueGroup(RENDER_QUEUE_BACKGROUND);

	// Use infinite AAB to always stay visible
	AxisAlignedBox aabInf;
	aabInf.setInfinite();
	mRectangle->setBoundingBox(aabInf);

	// Attach background to the scene
	mBackgroundNode = mGraphicsManager->getSceneManager()->getRootSceneNode()->createChildSceneNode("GameOverBackground");
	mBackgroundNode->attachObject(mRectangle);

	// Example of background scrolling
	//material->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setScrollAnimation(-0.25, 0.0);
}

/** Update internal stuff */
void GameoverState::update(const float elapsedSeconds)
{
	//
	// TODO Update
	//
}

/** Render */
void GameoverState::render(const float elapsedSeconds)
{
	//
	// TODO Render
	//
}

/** Unload resources */
void GameoverState::unload() 
{
	//
	// TODO Unload
	//
	mBackgroundNode->detachAllObjects();
}

/** Destroy the state */
void GameoverState::finalize()
{
	//
	// TODO Destroy
	//
	mGraphicsManager->getSceneManager()->clearScene();

	mGraphicsManager->getSceneManager()->destroyAllCameras();

	mGraphicsManager->getRoot()->getAutoCreatedWindow()->removeAllViewports();
}

/** Get state Id */
GameStateId GameoverState::getStateId()
{
	//
	// BEWARE! Make sure this is the correspondent state or we fuck up everything!
	//
	return GameStateId::GameOver;
}

/** Called when the state has to be paused */
void GameoverState::pause()
{
	//
	// TODO : Pause state
	//
}

/** Called when the state has to be resumed (from pause) */
void GameoverState::resume()
{
	//
	// TODO : Resume state
	//
}

/** Buffered input - keyboard key clicked */
bool GameoverState::keyReleased(const OIS::KeyEvent& e)
{
	switch(e.key)
	{
	case OIS::KC_M:
		this->mNextGameStateId = GameStateId::MainMenu;
		break;
	case OIS::KC_ESCAPE:
		this->mNextGameStateId = GameStateId::Exit;
		break;
	}

	return true;
}
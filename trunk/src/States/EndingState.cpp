#include "..\include\States\EndingState.h"

using namespace WyvernsAssault;

EndingState::EndingState(GraphicsManager& graphicsManager, InputManager& inputManager, AudioManager& audioManager)
: BaseState(graphicsManager,inputManager,audioManager)
{
	//
	// TODO Constructor logic HERE
	//
}

EndingState::~EndingState()
{
	//
	// TODO Distructor logic HERE
	//
	exit();
}

/** Initialize current state */
void EndingState::initialize()
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

/** Load resources */
void EndingState::load()
{
	//
	// TODO Load 
	//
		// Create background material
	mBackgroundMaterial = MaterialManager::getSingleton().create("EndingBackground", "General");
	mBackgroundMaterial->getTechnique(0)->getPass(0)->createTextureUnitState("Ending.png");
	mBackgroundMaterial->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
	mBackgroundMaterial->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
	mBackgroundMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);

	// Create background rectangle covering the whole screen
	mRectangle = new Rectangle2D(true);
	mRectangle->setCorners(-1.0, 1.0, 1.0, -1.0);
	mRectangle->setMaterial("EndingBackground");

	// Render the background before everything else
	mRectangle->setRenderQueueGroup(RENDER_QUEUE_BACKGROUND);

	// Use infinite AAB to always stay visible
	AxisAlignedBox aabInf;
	aabInf.setInfinite();
	mRectangle->setBoundingBox(aabInf);

	// Attach background to the scene
	mBackgroundNode = mGraphicsManager->getSceneManager()->getRootSceneNode()->createChildSceneNode("EndingBackground");
	mBackgroundNode->attachObject(mRectangle);

	// Example of background scrolling
	//material->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setScrollAnimation(-0.25, 0.0);
}

/** Manage input */
void EndingState::input()
{
	//
	// TODO Read input
	//
	this->mInputManager->capture();
}

/** Update internal stuff */
void EndingState::update(const float elapsedSeconds)
{
	//
	// TODO Update
	//
}

/** Render */
void EndingState::render(const float elapsedSeconds)
{
	//
	// TODO Render
	//
}

/** Unload resources */
void EndingState::unload() 
{
	//
	// TODO Unload
	//
	mBackgroundNode->detachAllObjects();
}

/** Destroy the state */
void EndingState::finalize()
{
	//
	// TODO Destroy
	//
	mGraphicsManager->getSceneManager()->clearScene();

	mGraphicsManager->getSceneManager()->destroyAllCameras();

	mGraphicsManager->getRoot()->getAutoCreatedWindow()->removeAllViewports();
}

/** Get state Id */
GameStateId EndingState::getStateId()
{
	//
	// BEWARE! Make sure this is the correspondent state or we fuck up everything!
	//
	return GameStateId::Ending;
}

/** Called when the state has to be paused */
void EndingState::pause()
{
	//
	// TODO : Pause state
	//
}

/** Called when the state has to be resumed (from pause) */
void EndingState::resume()
{
	//
	// TODO : Resume state
	//
}

/** Buffered input - keyboard key clicked */
bool EndingState::keyReleased(const OIS::KeyEvent& e)
{
	switch(e.key)
	{
	case OIS::KC_M:
		this->mNextGameStateId = GameStateId::MainMenu;
		break;
	case OIS::KC_C:
		this->mNextGameStateId = GameStateId::Credits;
		break;
	}

	return true;
}
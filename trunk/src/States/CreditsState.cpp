#include "..\include\States\CreditsState.h"

using namespace WyvernsAssault;

CreditsState::CreditsState(GraphicsManager& graphicsManager, InputManager& inputManager, AudioManager& audioManager)
: BaseState(graphicsManager,inputManager,audioManager)
{
	//
	// TODO Constructor
	//
}

CreditsState::~CreditsState()
{
	//
	// TODO Destructor
	//
	exit();
}

/** Initialize current state */
void CreditsState::initialize()
{
	//
	// TODO Initialize
	//
	this->mNextGameStateId = this->getStateId();

	mCamera = mGraphicsManager->getSceneManager()->createCamera( "DefaultCamera" );

	mViewport = mGraphicsManager->getRenderWindow()->addViewport( mCamera );

	mViewport->setBackgroundColour( Ogre::ColourValue( 1, 1, 1 ) );
}

/** Load resources */
void CreditsState::load()
{
	//
	// TODO Load 
	//
		// Create background material
	mBackgroundMaterial = MaterialManager::getSingleton().create("CreditsBackground", "General");
	mBackgroundMaterial->getTechnique(0)->getPass(0)->createTextureUnitState("Credits.png");
	mBackgroundMaterial->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
	mBackgroundMaterial->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
	mBackgroundMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);

	// Create background rectangle covering the whole screen
	mRectangle = new Rectangle2D(true);
	mRectangle->setCorners(-1.0, 1.0, 1.0, -1.0);
	mRectangle->setMaterial("CreditsBackground");

	// Render the background before everything else
	mRectangle->setRenderQueueGroup(RENDER_QUEUE_BACKGROUND);

	// Use infinite AAB to always stay visible
	AxisAlignedBox aabInf;
	aabInf.setInfinite();
	mRectangle->setBoundingBox(aabInf);

	// Attach background to the scene
	mBackgroundNode = mGraphicsManager->getSceneManager()->getRootSceneNode()->createChildSceneNode("CreditsBackground");
	mBackgroundNode->attachObject(mRectangle);

	// Example of background scrolling
	//material->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setScrollAnimation(-0.25, 0.0);
}

/** Manage input */
void CreditsState::input()
{
	//
	// TODO Read input
	//
	this->mInputManager->capture();
}

/** Update internal stuff */
void CreditsState::update(const float elapsedSeconds)
{
	//
	// TODO Update
	//
}

/** Render */
void CreditsState::render(const float elapsedSeconds)
{
	//
	// TODO Render
	//
}

/** Unload resources */
void CreditsState::unload() 
{
	//
	// TODO Unload
	//
	mBackgroundNode->detachAllObjects();
}

/** Destroy the state */
void CreditsState::finalize()
{
	//
	// TODO Destroy
	//
	mGraphicsManager->getSceneManager()->clearScene();

	mGraphicsManager->getSceneManager()->destroyAllCameras();

	mGraphicsManager->getRoot()->getAutoCreatedWindow()->removeAllViewports();
}

/** Get state Id */
GameStateId CreditsState::getStateId()
{
	//
	// BEWARE! Make sure this is the correspondent state or we fuck up everything!
	//
	return GameStateId::Credits;
}

/** Called when the state has to be paused */
void CreditsState::pause()
{
	//
	// TODO : Pause state
	//
}

/** Called when the state has to be resumed (from pause) */
void CreditsState::resume()
{
	//
	// TODO : Resume state
	//
}

/** Buffered input - keyboard key clicked */
bool CreditsState::keyReleased(const OIS::KeyEvent& e)
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
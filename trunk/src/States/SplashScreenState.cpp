#include "..\include\States\SplashScreenState.h"

using namespace WyvernsAssault;

SplashScreenState::SplashScreenState(GraphicsManager& graphicsManager, InputManager& inputManager, AudioManager& audioManager)
: BaseState(graphicsManager,inputManager,audioManager)
{	
	//
	// TODO Constructor
	//
}

SplashScreenState::~SplashScreenState()
{
	//
	// TODO Destructor
	//
	finalize();
}

/** Initialize current state */
void SplashScreenState::initialize()
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
void SplashScreenState::load()
{
	//
	// TODO Load 
	//
	// Create background material
	mBackgroundMaterial = MaterialManager::getSingleton().create("SplashScreenBackground", "General");
	mBackgroundMaterial->getTechnique(0)->getPass(0)->createTextureUnitState("SplashScreen.png");
	mBackgroundMaterial->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
	mBackgroundMaterial->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
	mBackgroundMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);

	// Create background rectangle covering the whole screen
	mRectangle = new Rectangle2D(true);
	mRectangle->setCorners(-1.0, 1.0, 1.0, -1.0);
	mRectangle->setMaterial("SplashScreenBackground");

	// Render the background before everything else
	mRectangle->setRenderQueueGroup(RENDER_QUEUE_BACKGROUND);

	// Use infinite AAB to always stay visible
	AxisAlignedBox aabInf;
	aabInf.setInfinite();
	mRectangle->setBoundingBox(aabInf);

	// Attach background to the scene
	mBackgroundNode = mGraphicsManager->getSceneManager()->getRootSceneNode()->createChildSceneNode("SplashScreenBackground");
	mBackgroundNode->attachObject(mRectangle);

	// Example of background scrolling
	//material->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setScrollAnimation(-0.25, 0.0);
}

/** Manage input */
void SplashScreenState::input()
{
	//
	// TODO Read input
	//
	this->mInputManager->capture();
}

/** Update internal stuff */
void SplashScreenState::update(const float elapsedSeconds)
{
	//
	// TODO Update
	//
}

/** Render */
void SplashScreenState::render(const float elapsedSeconds)
{
	//
	// TODO Render
	//
}

/** Unload resources */
void SplashScreenState::unload() 
{
	//
	// TODO Unload
	//
	mBackgroundNode->detachAllObjects();
}

/** Destroy the state */
void SplashScreenState::finalize()
{
	//
	// TODO Destroy
	//
	mGraphicsManager->getSceneManager()->clearScene();

	mGraphicsManager->getSceneManager()->destroyAllCameras();

	mGraphicsManager->getRoot()->getAutoCreatedWindow()->removeAllViewports();
}

/** Get state Id */
GameStateId SplashScreenState::getStateId()
{
	//
	// BEWARE! Make sure this is the correspondent state or we fuck up everything!
	//
	return GameStateId::SplashScreen;
}

/** Called when the state has to be paused */
void SplashScreenState::pause()
{
	//
	// TODO : Pause state
	//
}

/** Called when the state has to be resumed (from pause) */
void SplashScreenState::resume()
{
	//
	// TODO : Resume state
	//
}

/** Buffered input - keyboard key clicked */
bool SplashScreenState::keyReleased(const OIS::KeyEvent& e)
{
	switch(e.key)
	{
	case OIS::KC_ESCAPE:
		this->mNextGameStateId = GameStateId::Exit;
		break;
	}

	return true;
}

bool SplashScreenState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID buttonId)
{
	switch(buttonId)
	{
		case OIS::MouseButtonID::MB_Left:
			this->mNextGameStateId = GameStateId::Intro;
			break;
	}

	return true;
}
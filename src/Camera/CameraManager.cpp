#include "..\include\Camera\CameraManager.h"

using namespace Ogre;
using namespace WyvernsAssault;

// BEGIN SINGLETON
template<> CameraManager* Ogre::Singleton<CameraManager>::ms_Singleton = 0;
CameraManager* CameraManager::getSingletonPtr(void)
{
    return ms_Singleton;
}
CameraManager& CameraManager::getSingleton(void)
{  
    assert( ms_Singleton );  return ( *ms_Singleton );  
}
// END SINGLETON

CameraManager::CameraManager()
: mGameCamera(0)
, mSceneManager(0)
, mGuiCamera(0)
, mCameraNode(0)
, mGameCameraNode(0)
, mGameCameraLookAtNode(0)
, mCameraMan(0)
, mAxes(0)
, mAxesNode(0)
, mCameraTransition(0)
, mLookAtTransition(0)
, mAxesTransition(0)
, mCameraEffect(0)
, mCameraEffectLook(0)
, mGameViewport(0)
, mGuiViewport(0)
{
}

CameraManager::~CameraManager()
{
	finalize();
}

/** Initialize the camera manager */
void CameraManager::initialize(SceneManager* sceneManager, RenderWindow* window)
{
	this->mSceneManager = sceneManager;
	this->mWindow = window;
	this->mCameraNode = mSceneManager->getRootSceneNode()->createChildSceneNode(CAMERA_NODE_NAME);

	// -----------------------
	// Gui Camera and Viewport
	// -----------------------
	if(mSceneManager->hasCamera( GUI_CAMERA ))
		mGuiCamera = mSceneManager->getCamera( GUI_CAMERA );
	else
		mGuiCamera = mSceneManager->createCamera( GUI_CAMERA );

	mGuiViewport = window->addViewport(mGuiCamera,0);
	mGuiViewport->setBackgroundColour( Ogre::ColourValue( 0, 0, 0 ) );

	mGuiCamera->setAspectRatio(Real(mGuiViewport->getActualWidth()) / Real(mGuiViewport->getActualHeight()));	

	// ------------------------
	// Game Camera and Viewport
	// ------------------------
	if(mSceneManager->hasCamera( GAME_CAMERA ))
		mGameCamera = mSceneManager->getCamera( GAME_CAMERA );
	else
		mGameCamera = mSceneManager->createCamera( GAME_CAMERA );

	mGameViewport = window->addViewport(mGameCamera,1);
	mGameViewport->setBackgroundColour( Ogre::ColourValue( 0.3, 0.6, 0.9 ) );

	/** Game camera cofiguration */
	mGameCamera->setNearClipDistance(50);
	mGameCamera->setFarClipDistance(20000);
	mGameCamera->setAspectRatio(Real(mGameViewport->getActualWidth()) / Real(mGameViewport->getActualHeight()));	

	/** Camera Node and Look At Node */
	mGameCameraNode = mCameraNode->createChildSceneNode("GameCameraNode");
	mGameCameraLookAtNode = mCameraNode->createChildSceneNode("LookAtNode");
	mGameCameraNode->attachObject(mGameCamera);
	mGameCamera->setAutoTracking(true, mGameCameraLookAtNode, Vector3::UNIT_X);

	// Initial Camera Position
	mGameCameraMode = CameraModes::Fixed;
	mGameCameraNode->setPosition(0, 0, 0);

	/** Debug axes node */
	mAxes = mSceneManager->createEntity("Axes", "axes.mesh");
	mAxesNode = mCameraNode->createChildSceneNode("AxesNode");
	mAxesNode->setScale(0.1, 0.1, 0.1);
	mAxesNode->setVisible(false);
	
	/** Other variables initialization */
	mGameArea = -1;
	mFixedCamera = 0;
	mMoving = false;

	mSceneManager->createAnimation("CameraTrack", 1);
	mSceneManager->createAnimation("LookAtTrack", 1);
	mSceneManager->createAnimation("AxesTrack", 1);
	mSceneManager->createAnimation("CameraEffect", 1);
	mSceneManager->createAnimation("CameraEffectLook", 1);
	mCameraTransition = mSceneManager->createAnimationState("CameraTrack");
	mCameraTransition->setEnabled(false);
	mLookAtTransition = mSceneManager->createAnimationState("LookAtTrack");
	mLookAtTransition->setEnabled(false);
	mAxesTransition = mSceneManager->createAnimationState("AxesTrack");
	mAxesTransition->setEnabled(false);
	mCameraEffect = mSceneManager->createAnimationState("CameraEffect");
	mCameraEffect->setEnabled(false);
	mCameraEffectLook = mSceneManager->createAnimationState("CameraEffectLook");
	mCameraEffectLook->setEnabled(false);

	/** Sdk Camera Manager for free look camera */
	mCameraMan = new OgreBites::SdkCameraMan(mGameCamera);
	mCameraMan->setStyle(OgreBites::CS_MANUAL);
}

/** Finalize the camera manager */
void CameraManager::finalize()
{	
	if(!mSceneManager)
		return;

	if(mSceneManager->hasCamera(GUI_CAMERA))
	{
		mSceneManager->destroyCamera( GUI_CAMERA );
		mGuiCamera = NULL;
	}

	if(mSceneManager->hasCamera(GAME_CAMERA))
	{
		mSceneManager->destroyCamera( GAME_CAMERA );
		mGameCamera = NULL;
	}

	if(mCameraMan)
	{
		delete mCameraMan;
		mCameraMan = 0;
	}

	if(mGameCameraNode)
	{
		mSceneManager->destroySceneNode(mGameCameraNode);
		mGameCameraNode = NULL;
	}

	if(mGameCameraLookAtNode)
	{
		mSceneManager->destroySceneNode(mGameCameraLookAtNode);
		mGameCameraLookAtNode = NULL;
	}

	if(mAxes)
	{
		mSceneManager->destroyEntity(mAxes);
		mAxes = NULL;
	}

	if(mAxesNode)
	{
		mSceneManager->destroySceneNode(mAxesNode);
		mAxesNode = NULL;
	}

	if(mCameraTransition)
	{
		mSceneManager->destroyAnimationState("CameraTrack");
		mCameraTransition = NULL;
	}

	if(mLookAtTransition)
	{
		mSceneManager->destroyAnimationState("LookAtTrack");
		mLookAtTransition = NULL;
	}

	if(mAxesTransition)
	{
		mSceneManager->destroyAnimationState("AxesTrack");
		mAxesTransition = NULL;
	}

	if( mCameraEffect)
	{
		mSceneManager->destroyAnimationState("CameraEffect");
		mCameraEffect = NULL;
	}

	if(mCameraEffectLook)
	{
		mSceneManager->destroyAnimationState("CameraEffectLook");
		mCameraEffectLook = NULL;
	}

	if(mSceneManager)
	{
		Utils::Destroy(mSceneManager, CAMERA_NODE_NAME);
		this->mCameraNode = NULL;
		mSceneManager->destroyAllCameras();
		mSceneManager = NULL;
	}

	if(mWindow)
	{
		mWindow->removeAllViewports();
	}
}

Vector3 CameraManager::getCameraPosition()
{ 
	if(mGameCameraMode == CameraModes::Free)
		return mGameCamera->getRealPosition();
	else
		return mGameCameraNode->getPosition();
}

Vector3 CameraManager::getCameraLookAt()
{ 
	if(mGameCameraMode == CameraModes::Free)
		return mGameCamera->getRealPosition() + mGameCamera->getDirection() * 50;
	else
		return mGameCameraLookAtNode->getPosition();
}

String CameraManager::getCameraModeString()
{
	switch( mGameCameraMode )
	{
		case CameraModes::Game:
			return "Game";
			break;
		case CameraModes::Free:
			return "Free";
			break;
		case CameraModes::Fixed:
			return "Fixed";
			break;
		case CameraModes::CutScene:
			return "Cut Scene";
			break;
		default:
			return "NoMode";
	}
}

int CameraManager::getGameArea(Vector3 position)
{
	// Look for player's current game area
	for(int areaId = 0; areaId < mGameAreas.size(); areaId++)
	{
		// Define a polygon (in correct order) for the game area
		Ogre::Polygon area;
		area.insertVertex( mGameAreas[areaId].mBeginNear);
		area.insertVertex( mGameAreas[areaId].mEndNear);
		area.insertVertex( mGameAreas[areaId].mEndFar);
		area.insertVertex( mGameAreas[areaId].mBeginFar);

		// Check if player position (without height) is inside this game area
		if(area.isPointInside(position * Vector3(1,0,1)))
		{	
			return areaId;
		}
	}

	// No game area - Control ERROR
	return 0;
}

/** Camera moving direction */
Vector3 CameraManager::getDirection(Vector3 direction)
{
	Vector3 finalDirection;

	if(direction == Vector3(1,0,0))
	{
		finalDirection = Quaternion(Degree(270), Vector3::UNIT_Y) * mGameCamera->getDirection();
	}
	else if(direction == Vector3(-1,0,0))
	{
		finalDirection = Quaternion(Degree(90), Vector3::UNIT_Y) * mGameCamera->getDirection();
	}
	else if(direction == Vector3(0,0,1))
	{
		finalDirection = Quaternion(Degree(180), Vector3::UNIT_Y) * mGameCamera->getDirection();
	}
	else if(direction == Vector3(0,0,-1))
	{
		finalDirection = mGameCamera->getDirection();
	}
	else if(direction == Vector3(1,0,1))
	{
		finalDirection = Quaternion(Degree(225), Vector3::UNIT_Y) * mGameCamera->getDirection();
	}
	else if(direction == Vector3(1,0,-1))
	{
		finalDirection = Quaternion(Degree(315), Vector3::UNIT_Y) * mGameCamera->getDirection();
	}
	else if(direction == Vector3(-1,0,1))
	{
		finalDirection = Quaternion(Degree(135), Vector3::UNIT_Y) * mGameCamera->getDirection();
	}		
	else if(direction == Vector3(-1,0,-1))
	{
		finalDirection = Quaternion(Degree(45), Vector3::UNIT_Y) * mGameCamera->getDirection();
	}

	return finalDirection * Vector3(1,0,1);
}

/** Camera functions **/

void CameraManager::updateCamera(Vector3 player, Real elapsedSeconds)
{
	// Update camera camera
	if( mGameCameraMode == CameraModes::Game )
	{
		// Game Area changed, raise event
		if( mGameArea != getGameArea(player) )
		{
			GameAreaChangedEventPtr evt = GameAreaChangedEventPtr(new GameAreaChangedEvent(mGameArea, getGameArea(player)));
			EVENTS_FIRE(evt);
		}
		mGameArea = getGameArea(player);

		Vector3 begin = mCameraSegments[mGameArea].mPositionBegin;
		Vector3 end = mCameraSegments[mGameArea].mPositionEnd;
		Vector3 lbegin = mCameraSegments[mGameArea].mLookAtBegin;
		Vector3 lend = mCameraSegments[mGameArea].mLookAtEnd;

		Vector3 position;
		Vector3 lookAt;

		Real u = ((player.x - begin.x) * (end.x - begin.x)) + ((player.y - begin.y) * (end.y - begin.y)) + ((player.z - begin.z) * (end.z - begin.z));
		Real dist = begin.distance(end);
		u = u / (dist * dist);

		position.x = begin.x + u * (end.x - begin.x);
		position.y = begin.y + u * (end.y - begin.y);
		position.z = begin.z + u * (end.z - begin.z);

		// Movement in XZ (side scroll in X)
		if((end.x - begin.x) > (begin.z - end.z))
		{
			// If player position not in camera segment, camera position = begin/end of current segment
			if(player.x < begin.midPoint(lbegin).x)
			{
				position.x = begin.x;
			}
			else if(player.x > end.midPoint(lend).x)
			{
				position.x = end.x;
			}

			Real gap = (position.x - begin.x) / (end.x - begin.x);
			position.z = begin.z - (gap * (begin.z - end.z));
			position.y = begin.y + (gap * (end.y - begin.y));
		}
		// Movement in ZX (side scroll in Z). Be carefull with Z index!
		else
		{
			// If player position not in camera segment, camera position = begin/end of current segment
			if(player.z > begin.midPoint(lbegin).z)
			{
				position.z = begin.z;
			}
			else if(player.z < end.midPoint(lend).z)			
			{
				position.z = end.z;
			}

			Real gap = (begin.z - position.z) / (begin.z - end.z);
			position.x = begin.x + (gap * (end.x - begin.x));
			position.y = begin.y + (gap * (end.y - begin.y));
		}		
		
		// Get current look at position with distances
		Real percent = begin.distance(position) / begin.distance(end);
		
		lookAt.x = lbegin.x + ((lend.x - lbegin.x) * percent);
		lookAt.y = lbegin.y + ((lend.y - lbegin.y) * percent);
		lookAt.z = lbegin.z + ((lend.z - lbegin.z) * percent);

		// Move camera to ZERO
		mGameCamera->setPosition(Vector3::ZERO);
		// Translate animation to camera scene node and look at node to current position
		createTransition(getCameraPosition(), position, getCameraLookAt(), lookAt, 0.2);
	}
	// Cut scene camera update
	else if( mGameCameraMode == CameraModes::CutScene )
	{
		if ( mCameraTransition->hasEnded() && mLookAtTransition->hasEnded() )
			mMoving = false;
		else
			mMoving = true;
	}
	// Other camera modes
	else
	{
		// NO update needed in other modes

	}

	// Add elapsed seconds to animations
	mCameraEffect->addTime(elapsedSeconds);
	mCameraEffectLook->addTime(elapsedSeconds);
	mCameraTransition->addTime(elapsedSeconds);
	mLookAtTransition->addTime(elapsedSeconds);
	mAxesTransition->addTime(elapsedSeconds);
}

void CameraManager::createTransition(Vector3 begin, Vector3 end, Vector3 lbegin, Vector3 lend, float duration)
{
	// Set up spline animation of node
	if(mSceneManager->hasAnimation("CameraTrack"))
		mSceneManager->destroyAnimation("CameraTrack");
	Animation* anim = mSceneManager->createAnimation("CameraTrack", duration);
    // Spline it for nice curves
    anim->setInterpolationMode(Animation::IM_SPLINE);
    // Create a track to animate the camera's node
	NodeAnimationTrack* track = anim->createNodeTrack(0, mGameCameraNode);
    // Setup keyframes
	// Start position
    TransformKeyFrame* key = track->createNodeKeyFrame(0);
    key->setTranslate(begin);
	// Middle position for smooth translation
	key = track->createNodeKeyFrame(duration/2);	
	key->setTranslate(begin + ((end - begin)/2));
	// Final position
    key = track->createNodeKeyFrame(duration);
	key->setTranslate(end);
    // Create a new animation state to track this
	if(mSceneManager->hasAnimationState("CameraTrack"))
		mSceneManager->destroyAnimationState("CameraTrack");
	mCameraTransition = mSceneManager->createAnimationState("CameraTrack");
    mCameraTransition->setEnabled(true);
	mCameraTransition->setWeight(1);
	mCameraTransition->setLoop(false);

	/** Camera look at positioning translation animation */
	if(mSceneManager->hasAnimation("LookAtTrack"))
		mSceneManager->destroyAnimation("LookAtTrack");
	anim = mSceneManager->createAnimation("LookAtTrack", duration);
    anim->setInterpolationMode(Animation::IM_SPLINE);
	track = anim->createNodeTrack(0, mGameCameraLookAtNode);
    key = track->createNodeKeyFrame(0);
    key->setTranslate(lbegin);
	key = track->createNodeKeyFrame(duration/2);	
	key->setTranslate(lbegin + ((lend - lbegin)/2));
    key = track->createNodeKeyFrame(duration);
	key->setTranslate(lend);
	if(mSceneManager->hasAnimationState("LookAtTrack"))
		mSceneManager->destroyAnimationState("LookAtTrack");
	mLookAtTransition = mSceneManager->createAnimationState("LookAtTrack");
    mLookAtTransition->setEnabled(true);
	mLookAtTransition->setWeight(1);
	mLookAtTransition->setLoop(false);

	/** Debug axes positioning translation animation */
	Real x = end.x - ((end.x - lend.x)/10);
	Real y = end.y - ((end.y - lend.y)/10);
	Real z = end.z - ((end.z - lend.z)/10);
	Vector3 axesBegin = mAxesNode->getPosition();
	Vector3 axesEnd = Vector3(x, y, z);

	if(mSceneManager->hasAnimation("AxesTrack"))
		mSceneManager->destroyAnimation("AxesTrack");
	anim = mSceneManager->createAnimation("AxesTrack", duration);
    anim->setInterpolationMode(Animation::IM_SPLINE);
	track = anim->createNodeTrack(0, mAxesNode);
    key = track->createNodeKeyFrame(0);
    key->setTranslate(axesBegin);
	key = track->createNodeKeyFrame(duration/2);	
	key->setTranslate(axesBegin + ((axesEnd - axesBegin)/2));
    key = track->createNodeKeyFrame(duration);
	key->setTranslate(axesEnd);
	if(mSceneManager->hasAnimationState("AxesTrack"))
		mSceneManager->destroyAnimationState("AxesTrack");
	mAxesTransition = mSceneManager->createAnimationState("AxesTrack");
    mAxesTransition->setEnabled(true);
	mAxesTransition->setWeight(1);
	mAxesTransition->setLoop(false);
}

/** Add game area to vector */
void CameraManager::addGameArea(Vector3 beginNear, Vector3 endNear, Vector3 beginFar, Vector3 endFar)
{
	GameArea area;
	area.mBeginNear = beginNear;
	area.mEndNear = endNear;
	area.mBeginFar = beginFar;
	area.mEndFar = endFar;
	mGameAreas.push_back(area);
}

/** Add camera segment to vector */
void CameraManager::addCameraSegment(Vector3 pbegin, Vector3 pend, Vector3 lbegin, Vector3 lend)
{
	CameraSegment camSeg;
	camSeg.mPositionBegin = pbegin;
	camSeg.mPositionEnd = pend;
	camSeg.mLookAtBegin = lbegin;	
	camSeg.mLookAtEnd = lend;
	mCameraSegments.push_back(camSeg);
}

/** Set a fixed camera */
void CameraManager::setFixedCamera(int camera, Vector3 position, Vector3 lookAt)
{
	FixedCamera fixedCamera;
	fixedCamera.mId = camera;
	fixedCamera.mPosition = position;
	fixedCamera.mLookAt = lookAt;

	mFixedCameras.push_back(fixedCamera);
}

/** Camera effects */
void CameraManager::zoom(Real duration, Real amount)
{
	Vector3 begin = this->getCameraPosition();
	Vector3 zoomIn = this->getCameraDirection() * amount;
	// Camera node
	if(mSceneManager->hasAnimation("CameraEffect"))
		mSceneManager->destroyAnimation("CameraEffect");
	Animation* anim = mSceneManager->createAnimation("CameraEffect", duration);
    // Spline it for nice curves
	anim->setInterpolationMode(Animation::IM_LINEAR);
    // Create a track to animate the camera's node
	NodeAnimationTrack* track = anim->createNodeTrack(0, mGameCameraNode);
    // Setup keyframes
	TransformKeyFrame* key = track->createNodeKeyFrame(0);
	//key->setTranslate( begin );
	key = track->createNodeKeyFrame( duration - (duration - 0.1) );
	key->setTranslate( zoomIn );
	key = track->createNodeKeyFrame( duration - 0.1 );
	key->setTranslate( zoomIn );
	key = track->createNodeKeyFrame( duration );
	//key->setTranslate( begin );
    // Create a new animation state to track this
	if(mSceneManager->hasAnimationState("CameraEffect"))
		mSceneManager->destroyAnimationState("CameraEffect");
	mCameraEffect = mSceneManager->createAnimationState("CameraEffect");
    mCameraEffect->setEnabled(true);
	mCameraEffect->setWeight(1);
	mCameraEffect->setLoop(false);	
}

void CameraManager::rumble(Real duration, Real amount)
{
	// Camera node
	if(mSceneManager->hasAnimation("CameraEffect"))
		mSceneManager->destroyAnimation("CameraEffect");
	Animation* anim = mSceneManager->createAnimation("CameraEffect", duration);
    // Spline it for nice curves
    anim->setInterpolationMode(Animation::IM_SPLINE);
    // Create a track to animate the camera's node
	NodeAnimationTrack* track = anim->createNodeTrack(0, mGameCameraNode);
    // Setup keyframes
	TransformKeyFrame* key = track->createNodeKeyFrame(0);
	key->setTranslate(Vector3::ZERO);
	key = track->createNodeKeyFrame(duration/5);
	key->setTranslate(Vector3(-amount, -amount, 0));
	key = track->createNodeKeyFrame(duration*2/5);
	key->setTranslate(Vector3(amount, amount, 0));
	key = track->createNodeKeyFrame(duration*3/5);
	key->setTranslate(Vector3(-amount/2, -amount/2, 0));
	key = track->createNodeKeyFrame(duration*4/5);
	key->setTranslate(Vector3(amount/2, amount/2, 0));
	key = track->createNodeKeyFrame(duration);
	key->setTranslate(Vector3::ZERO);

    // Create a new animation state to track this
	if(mSceneManager->hasAnimationState("CameraEffect"))
		mSceneManager->destroyAnimationState("CameraEffect");
	mCameraEffect = mSceneManager->createAnimationState("CameraEffect");
    mCameraEffect->setEnabled(true);
	mCameraEffect->setWeight(1);
	mCameraEffect->setLoop(false);

	// Camera look at node
	if(mSceneManager->hasAnimation("CameraEffectLook"))
		mSceneManager->destroyAnimation("CameraEffectLook");
	anim = mSceneManager->createAnimation("CameraEffectLook", duration);
    // Spline it for nice curves
    anim->setInterpolationMode(Animation::IM_SPLINE);
    // Create a track to animate the camera's node
	track = anim->createNodeTrack(0, mGameCameraLookAtNode);
    // Setup keyframes
	key = track->createNodeKeyFrame(0);
	key->setTranslate(Vector3::ZERO);
	key = track->createNodeKeyFrame(duration/5);
	key->setTranslate(Vector3(-amount, -amount, 0));
	key = track->createNodeKeyFrame(duration*2/5);
	key->setTranslate(Vector3(amount, amount, 0));
	key = track->createNodeKeyFrame(duration*3/5);
	key->setTranslate(Vector3(-amount/2, -amount/2, 0));
	key = track->createNodeKeyFrame(duration*4/5);
	key->setTranslate(Vector3(amount/2, amount/2, 0));
	key = track->createNodeKeyFrame(duration);
	key->setTranslate(Vector3::ZERO);
    // Create a new animation state to track this
	if(mSceneManager->hasAnimationState("CameraEffectLook"))
		mSceneManager->destroyAnimationState("CameraEffectLook");
	mCameraEffectLook = mSceneManager->createAnimationState("CameraEffectLook");
    mCameraEffectLook->setEnabled(true);
	mCameraEffectLook->setWeight(1);
	mCameraEffectLook->setLoop(false);
}

void CameraManager::tremor(Real duration, Real amount)
{
	// Camera node
	if(mSceneManager->hasAnimation("CameraEffect"))
		mSceneManager->destroyAnimation("CameraEffect");
	Animation* anim = mSceneManager->createAnimation("CameraEffect", duration);
    // Spline it for nice curves
    anim->setInterpolationMode(Animation::IM_SPLINE);
    // Create a track to animate the camera's node
	NodeAnimationTrack* track = anim->createNodeTrack(0, mGameCameraNode);
    // Setup keyframes
	TransformKeyFrame* key = track->createNodeKeyFrame(0);
	key->setTranslate(Vector3::ZERO);
	key = track->createNodeKeyFrame(duration/10);
	key->setTranslate(Vector3(0, -amount, 0));
	key = track->createNodeKeyFrame(duration/5);
	key->setTranslate(Vector3(0, amount, 0));
	key = track->createNodeKeyFrame(duration*3/10);
	key->setTranslate(Vector3(0, -amount*2/3, 0));
	key = track->createNodeKeyFrame(duration/4);
	key->setTranslate(Vector3(0, amount*2/3, 0));
	key = track->createNodeKeyFrame(duration/2);
	key->setTranslate(Vector3(0, -amount/2, 0));
	key = track->createNodeKeyFrame(duration*3/5);
	key->setTranslate(Vector3(0, amount/2, 0));
	key = track->createNodeKeyFrame(duration*7/10);
	key->setTranslate(Vector3(0, -amount/3, 0));
	key = track->createNodeKeyFrame(duration*4/5);
	key->setTranslate(Vector3(0, amount/3, 0));
	key = track->createNodeKeyFrame(duration*9/10);
	key->setTranslate(Vector3(0, -amount/3, 0));
	key = track->createNodeKeyFrame(duration);
	key->setTranslate(Vector3::ZERO);
    // Create a new animation state to track this
	if(mSceneManager->hasAnimationState("CameraEffect"))
		mSceneManager->destroyAnimationState("CameraEffect");
	mCameraEffect = mSceneManager->createAnimationState("CameraEffect");
    mCameraEffect->setEnabled(true);
	mCameraEffect->setWeight(1);
	mCameraEffect->setLoop(false);

	// Camera look at node
	if(mSceneManager->hasAnimation("CameraEffectLook"))
		mSceneManager->destroyAnimation("CameraEffectLook");
	anim = mSceneManager->createAnimation("CameraEffectLook", duration);
    // Spline it for nice curves
    anim->setInterpolationMode(Animation::IM_SPLINE);
    // Create a track to animate the camera's node
	track = anim->createNodeTrack(0, mGameCameraLookAtNode);
    // Setup keyframes
	key = track->createNodeKeyFrame(0);
	key->setTranslate(Vector3::ZERO);
	key = track->createNodeKeyFrame(duration/10);
	key->setTranslate(Vector3(0, -amount, 0));
	key = track->createNodeKeyFrame(duration/5);
	key->setTranslate(Vector3(0, amount, 0));
	key = track->createNodeKeyFrame(duration*3/10);
	key->setTranslate(Vector3(0, -amount*2/3, 0));
	key = track->createNodeKeyFrame(duration/4);
	key->setTranslate(Vector3(0, amount*2/3, 0));
	key = track->createNodeKeyFrame(duration/2);
	key->setTranslate(Vector3(0, -amount/2, 0));
	key = track->createNodeKeyFrame(duration*3/5);
	key->setTranslate(Vector3(0, amount/2, 0));
	key = track->createNodeKeyFrame(duration*7/10);
	key->setTranslate(Vector3(0, -amount/3, 0));
	key = track->createNodeKeyFrame(duration*4/5);
	key->setTranslate(Vector3(0, amount/3, 0));
	key = track->createNodeKeyFrame(duration*9/10);
	key->setTranslate(Vector3(0, -amount/3, 0));
	key = track->createNodeKeyFrame(duration);
	key->setTranslate(Vector3::ZERO);
    // Create a new animation state to track this
	if(mSceneManager->hasAnimationState("CameraEffectLook"))
		mSceneManager->destroyAnimationState("CameraEffectLook");
	mCameraEffectLook = mSceneManager->createAnimationState("CameraEffectLook");
    mCameraEffectLook->setEnabled(true);
	mCameraEffectLook->setWeight(1);
	mCameraEffectLook->setLoop(false);
}

void CameraManager::shake(Real duration, Real amount)
{
	// Camera look at node translation animation
	if(mSceneManager->hasAnimation("CameraEffectLook"))
		mSceneManager->destroyAnimation("CameraEffectLook");
	Animation* anim = mSceneManager->createAnimation("CameraEffectLook", duration);
    // Spline it for nice curves
    anim->setInterpolationMode(Animation::IM_SPLINE);
    // Create a track to animate the camera's node
	NodeAnimationTrack* track = anim->createNodeTrack(0, mGameCameraLookAtNode);
    // Setup keyframes
	TransformKeyFrame* key = track->createNodeKeyFrame(0);
	key->setTranslate(Vector3::ZERO);
	key = track->createNodeKeyFrame(duration/3);
	key->setTranslate(Vector3(0, amount, 0));
	key = track->createNodeKeyFrame(duration*2/3);
	key->setTranslate(Vector3(0, -amount*2, 0));
	key = track->createNodeKeyFrame(duration);
	key->setTranslate(Vector3::ZERO);
    // Create a new animation state to track this
	if(mSceneManager->hasAnimationState("CameraEffectLook"))
		mSceneManager->destroyAnimationState("CameraEffectLook");
	mCameraEffectLook = mSceneManager->createAnimationState("CameraEffectLook");
    mCameraEffectLook->setEnabled(true);
	mCameraEffectLook->setWeight(1);
	mCameraEffectLook->setLoop(false);
}

void CameraManager::drunk(Real duration, Real amount)
{
	// Camera look at node
	if(mSceneManager->hasAnimation("CameraEffectLook"))
		mSceneManager->destroyAnimation("CameraEffectLook");
	Animation* anim = mSceneManager->createAnimation("CameraEffectLook", duration);
    // Spline it for nice curves
    anim->setInterpolationMode(Animation::IM_SPLINE);
    // Create a track to animate the camera's node
	NodeAnimationTrack* track = anim->createNodeTrack(0, mGameCameraLookAtNode);
    // Setup keyframes
	TransformKeyFrame* key = track->createNodeKeyFrame(0);
	key->setTranslate(Vector3::ZERO);

	key = track->createNodeKeyFrame(duration/5);
	key->setTranslate(Vector3(-amount, -amount, 0));

	key = track->createNodeKeyFrame(duration*2/5);
	key->setTranslate(Vector3(amount, amount, 0));

	key = track->createNodeKeyFrame(duration*3/5);
	key->setTranslate(Vector3(-amount/2, -amount/2, 0));

	key = track->createNodeKeyFrame(duration*4/5);
	key->setTranslate(Vector3(amount/2, amount/2, 0));
	key = track->createNodeKeyFrame(duration);
	key->setTranslate(Vector3::ZERO);
    // Create a new animation state to track this
	if(mSceneManager->hasAnimationState("CameraEffectLook"))
		mSceneManager->destroyAnimationState("CameraEffectLook");
	mCameraEffectLook = mSceneManager->createAnimationState("CameraEffectLook");
    mCameraEffectLook->setEnabled(true);
	mCameraEffectLook->setWeight(1);
	mCameraEffectLook->setLoop(false);
}

/** Camera modes */
void CameraManager::gameCamera()
{
	mGameCameraMode = CameraModes::Game;
	mCameraMan->setStyle(OgreBites::CS_MANUAL);
	mGameCamera->setAutoTracking(true, mGameCameraLookAtNode, Vector3::UNIT_X);
	mGameCamera->setPosition(Vector3::ZERO);
	mGameViewport->setCamera(mGameCamera);
}
void CameraManager::freeCamera()
{
	mGameCameraMode = CameraModes::Free;
	mCameraMan->setStyle(OgreBites::CS_FREELOOK);
	mGameCamera->setAutoTracking(false);
	mGameViewport->setCamera(mGameCamera);
}
void CameraManager::fixedCamera(int camera)
{	
	char camName[20];
	sprintf(camName, "Fixed %d", camera);
	mGameCameraMode = CameraModes::Fixed;
	mCameraMan->setStyle(OgreBites::CS_MANUAL);
	mGameCamera->setAutoTracking(true, mGameCameraLookAtNode, Vector3::UNIT_X);
	mGameCamera->setPosition(Vector3::ZERO);
	mGameViewport->setCamera(mGameCamera);

	// Translate animation to camera scene node and look at node to current position
	createTransition(getCameraPosition(), mFixedCameras[camera].mPosition, getCameraLookAt(), mFixedCameras[camera].mLookAt, 1.5);

	// Save fixed camera index
	mFixedCamera = camera;
}

void CameraManager::cutSceneCamera()
{
	mGameCameraMode = CameraModes::CutScene;
	mCameraMan->setStyle(OgreBites::CS_MANUAL);
	mGameCamera->setAutoTracking(true, mGameCameraLookAtNode, Vector3::UNIT_X);
	mGameCamera->setPosition(Vector3::ZERO);
	mGameViewport->setCamera(mGameCamera);
}

void CameraManager::resumeCamera()
{
	if( mGameCameraMode == CameraModes::Game )
	{
		freeCamera();
		gameCamera();
	}
	else if ( mGameCameraMode == CameraModes::Free )
		freeCamera();
	else if ( mGameCameraMode == CameraModes::CutScene )
		cutSceneCamera();
	else
		fixedCamera(mFixedCamera);
}

/** Free camera functions */
void CameraManager::freeCameraMouse(OIS::MouseEvent evt)
{
	mCameraMan->injectMouseMove(evt);
}
void CameraManager::freeCameraKeyboardDown(OIS::KeyEvent evt)
{
	mCameraMan->injectKeyDown(evt);
}
void CameraManager::freeCameraKeyboardUp(OIS::KeyEvent evt)
{
	mCameraMan->injectKeyUp(evt);
}

/**  Rendering queue */
bool CameraManager::frameRenderingQueued(FrameEvent evt)
{
	return mCameraMan->frameRenderingQueued(evt);
}

/** Debug axes visibility */
void CameraManager::showAxes()
{
	mAxesNode->attachObject(mSceneManager->getEntity("Axes"));
	mAxesNode->getAttachedObject("Axes")->setVisible(true);
}
void CameraManager::hideAxes()
{
	mAxesNode->getAttachedObject("Axes")->setVisible(false);
	mAxesNode->detachObject("Axes");	
}

// --------------
// Event handlers
// --------------
EVENTS_BEGIN_REGISTER_HANDLERS(CameraManager)
	EVENTS_REGISTER_HANDLER(CameraManager,EnemyKilled)
	EVENTS_REGISTER_HANDLER(CameraManager,ItemCatch)
EVENTS_END_REGISTER_HANDLERS()

EVENTS_BEGIN_UNREGISTER_HANDLERS(CameraManager)
	EVENTS_UNREGISTER_HANDLER(CameraManager,EnemyKilled)
	EVENTS_UNREGISTER_HANDLER(CameraManager,ItemCatch)
EVENTS_END_UNREGISTER_HANDLERS()

EVENTS_DEFINE_HANDLER(CameraManager,EnemyKilled)
{
	EnemyPtr enemy = evt->getEnemy();
	PlayerPtr player = evt->getPlayer();

	if( enemy->hasDieAnimation() )
	{
		this->shake(0.5, (rand()%5));
	}
	else
	{
		this->rumble(0.5, (rand()%4));
	}

	if( player->isSpecial() )
	{
		this->zoom(player->getSpecialLength(), 3);
	}
}

EVENTS_DEFINE_HANDLER(CameraManager, ItemCatch)
{
	ItemPtr item = evt->getItem();

	if ( item->getDrunkTime() > 0 )
		this->drunk(item->getDrunkTime() / 2 , item->getDrunkTime());
}

// --------------------------------
// Lua Camera Lib
// --------------------------------
LUA_BEGIN_BINDING(CameraManager, cameralib)
LUA_BIND(CameraManager, setCurrent)
LUA_BIND(CameraManager, getCurrent)
LUA_BIND(CameraManager, translate)
LUA_BIND(CameraManager, moveTo)
LUA_BIND(CameraManager, flyTo)
LUA_BIND(CameraManager, hasArrived)
LUA_BIND(CameraManager, strife)
LUA_BIND(CameraManager, shake)
LUA_BIND(CameraManager, rumble)
LUA_BIND(CameraManager, tremor)
LUA_BIND(CameraManager, zoom)
LUA_END_BINDING()

//
// Load lua scripts that will be used by this manager
//
LUA_BEGIN_LOAD_SCRIPTS(CameraManager)
// 
// TODO : Load scripts if needed
//
LUA_END_LOAD_SCRIPTS()

LUA_DEFINE_FUNCTION(CameraManager, setCurrent)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	//
	// Retrieve the CameraManager
	//
	CameraManager* cameraManager = CameraManager::getSingletonPtr();

	int cameraMode = lua_tonumber(L, 1);

	//
	// Set current camera
	//
	switch( cameraMode )
	{
		case CameraModes::CutScene:
			cameraManager->cutSceneCamera();
			break;
		case CameraModes::Free:
			cameraManager->freeCamera();
			break;
		case CameraModes::Game:
			cameraManager->gameCamera();
		default:
			cameraManager->gameCamera();
	}

	/* return the number of results */
	return 0;
}

LUA_DEFINE_FUNCTION(CameraManager, getCurrent)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	//
	// Retrieve the CameraManager
	//
	CameraManager* cameraManager = CameraManager::getSingletonPtr();

	//
	// Get current camera name, and return it
	//
	int cameraMode = cameraManager->getCameraMode();

	lua_pushnumber(L, cameraMode);

	/* return the number of results */
	return 1;
}

LUA_DEFINE_FUNCTION(CameraManager, translate)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	Ogre::Vector3 position = Ogre::Vector3::ZERO;
	Ogre::Vector3 lookAt = Ogre::Vector3::ZERO;

	position.x = lua_tonumber(L, 1);
	position.y = lua_tonumber(L, 2);
	position.z = lua_tonumber(L, 3);
	lookAt.x = lua_tonumber(L, 4);
	lookAt.y = lua_tonumber(L, 5);
	lookAt.z = lua_tonumber(L, 6);

	//
	// Retrieve the CameraManager
	//
	CameraManager* cameraManager = CameraManager::getSingletonPtr();

	cameraManager->translate(position, lookAt);

	/* return the number of results */
	return 0;
}


LUA_DEFINE_FUNCTION(CameraManager, moveTo)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	Ogre::Vector3 position = Ogre::Vector3::ZERO;
	Ogre::Vector3 lookAt = Ogre::Vector3::ZERO;

	position.x = lua_tonumber(L, 1);
	position.y = lua_tonumber(L, 2);
	position.z = lua_tonumber(L, 3);
	lookAt.x = lua_tonumber(L, 4);
	lookAt.y = lua_tonumber(L, 5);
	lookAt.z = lua_tonumber(L, 6);

	//
	// Retrieve the CameraManager
	//
	CameraManager* cameraManager = CameraManager::getSingletonPtr();

	cameraManager->moveTo(position, lookAt);

	/* return the number of results */
	return 0;
}

LUA_DEFINE_FUNCTION(CameraManager, flyTo)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	Ogre::Vector3 cameraPosition = Ogre::Vector3::ZERO;
	Ogre::Vector3 lookAtPosition = Ogre::Vector3::ZERO;
	float time = 0.0f;

	cameraPosition.x = lua_tonumber(L, 1);
	cameraPosition.y = lua_tonumber(L, 2);
	cameraPosition.z = lua_tonumber(L, 3);
	lookAtPosition.x = lua_tonumber(L, 4);
	lookAtPosition.y = lua_tonumber(L, 5);
	lookAtPosition.z = lua_tonumber(L, 6);
	time = lua_tonumber(L, 7);

	//
	// Retrieve the CameraManager
	//
	CameraManager* cameraManager = CameraManager::getSingletonPtr();

	cameraManager->createTransition(cameraManager->getCameraPosition(), cameraPosition, cameraManager->getCameraLookAt(), lookAtPosition, time);

	/* return the number of results */
	return 0;
}

LUA_DEFINE_FUNCTION(CameraManager, hasArrived)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	bool arrived = false;

	//
	// Retrieve the CameraManager
	//
	CameraManager* cameraManager = CameraManager::getSingletonPtr();

	// If camera is moving, hasn't arrived yet!
	arrived = !cameraManager->isMoving();

	lua_pushboolean(L, arrived);

	/* return the number of results */
	return 1;
}

LUA_DEFINE_FUNCTION(CameraManager, strife)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	float direction = lua_tonumber(L, 1);
	float amount = lua_tonumber(L, 2);

	//
	// Retrieve the CameraManager
	//
	CameraManager* cameraManager = CameraManager::getSingletonPtr();

	Ogre::Vector3 dir = Ogre::Vector3::ZERO;

	// UP
	if( direction == 0 )
		dir = Ogre::Vector3(0, 1, 0);
	// DOWN
	else if( direction == 1 )
		dir = Ogre::Vector3(0, -1, 0);
	// LEFT
	else if( direction == 2 )
		dir = Ogre::Vector3(1, 0, 0);
	// RIGHT
	else if( direction == 3 )
		dir = Ogre::Vector3(-1, 0, 0);

	dir = dir * amount;

	cameraManager->translate(dir, dir);

	/* return the number of results */
	return 0;
}

LUA_DEFINE_FUNCTION(CameraManager, shake)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	float duration = lua_tonumber(L, 1);
	float amount = lua_tonumber(L, 2);

	//
	// Retrieve the CameraManager
	//
	CameraManager* cameraManager = CameraManager::getSingletonPtr();
	
	cameraManager->shake(duration, amount);

	/* return the number of results */
	return 0;
}

LUA_DEFINE_FUNCTION(CameraManager, rumble)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	float duration = lua_tonumber(L, 1);
	float amount = lua_tonumber(L, 2);

	//
	// Retrieve the CameraManager
	//
	CameraManager* cameraManager = CameraManager::getSingletonPtr();

	cameraManager->rumble(duration, amount);

	/* return the number of results */
	return 0;
}

LUA_DEFINE_FUNCTION(CameraManager, tremor)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	float duration = lua_tonumber(L, 1);
	float amount = lua_tonumber(L, 2);

	//
	// Retrieve the CameraManager
	//
	CameraManager* cameraManager = CameraManager::getSingletonPtr();

	cameraManager->tremor(duration, amount);

	/* return the number of results */
	return 0;
}

LUA_DEFINE_FUNCTION(CameraManager, zoom)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	float duration = lua_tonumber(L, 1);
	float amount = lua_tonumber(L, 2);

	//
	// Retrieve the CameraManager
	//
	CameraManager* cameraManager = CameraManager::getSingletonPtr();

	cameraManager->zoom(duration, amount);

	/* return the number of results */
	return 0;
}
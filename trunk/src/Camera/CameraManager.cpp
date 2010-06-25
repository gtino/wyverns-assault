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
	mGameCameraMode = "Start";
	mGameCameraNode->setPosition(2000, 1500, -2000);

	/** Debug axes node */
	mAxes = mSceneManager->createEntity("Axes", "axes.mesh");
	mAxesNode = mCameraNode->createChildSceneNode("AxesNode");
	mAxesNode->setScale(0.1, 0.1, 0.1);
	mAxesNode->setVisible(false);
	
	/** Other variables initialization */
	mGameArea = 0;
	mFixedCamera = 0;

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
	if(mGameCameraMode == "Free")
		return mGameCamera->getRealPosition();
	else
		return mGameCameraNode->getPosition();
}

Vector3 CameraManager::getCameraLookAt()
{ 
	if(mGameCameraMode == "Free")
		return mGameCamera->getRealPosition() + mGameCamera->getDirection() * 50;
	else
		return mGameCameraLookAtNode->getPosition();
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
	if(mGameCameraMode == "Game")
	{
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
		createTransition(getCameraPosition(), position, getCameraLookAt(), lookAt);
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

void CameraManager::createTransition(Vector3 begin, Vector3 end, Vector3 lbegin, Vector3 lend)
{
	/** Camera positioning translation animation */
	Real dist = 0.2;
	if(mGameCameraMode != "Game")
		dist = 1;

	// Set up spline animation of node
	if(mSceneManager->hasAnimation("CameraTrack"))
		mSceneManager->destroyAnimation("CameraTrack");
	Animation* anim = mSceneManager->createAnimation("CameraTrack", dist);
    // Spline it for nice curves
    anim->setInterpolationMode(Animation::IM_SPLINE);
    // Create a track to animate the camera's node
	NodeAnimationTrack* track = anim->createNodeTrack(0, mGameCameraNode);
    // Setup keyframes
	// Start position
    TransformKeyFrame* key = track->createNodeKeyFrame(0);
    key->setTranslate(begin);
	// Middle position for smooth translation
	key = track->createNodeKeyFrame(dist/2);	
	key->setTranslate(begin + ((end - begin)/2));
	// Final position
    key = track->createNodeKeyFrame(dist);
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
	anim = mSceneManager->createAnimation("LookAtTrack", dist);
    anim->setInterpolationMode(Animation::IM_SPLINE);
	track = anim->createNodeTrack(0, mGameCameraLookAtNode);
    key = track->createNodeKeyFrame(0);
    key->setTranslate(lbegin);
	key = track->createNodeKeyFrame(dist/2);	
	key->setTranslate(lbegin + ((lend - lbegin)/2));
    key = track->createNodeKeyFrame(dist);
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
	anim = mSceneManager->createAnimation("AxesTrack", dist);
    anim->setInterpolationMode(Animation::IM_SPLINE);
	track = anim->createNodeTrack(0, mAxesNode);
    key = track->createNodeKeyFrame(0);
    key->setTranslate(axesBegin);
	key = track->createNodeKeyFrame(dist/2);	
	key->setTranslate(axesBegin + ((axesEnd - axesBegin)/2));
    key = track->createNodeKeyFrame(dist);
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
void CameraManager::zoom(Real duration)
{
	Vector3 begin = this->getCameraPosition();
	Vector3 zoomIn = this->getCameraDirection() * 2;
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

void CameraManager::rumble(Real scale)
{
	// Camera node
	if(mSceneManager->hasAnimation("CameraEffect"))
		mSceneManager->destroyAnimation("CameraEffect");
	Animation* anim = mSceneManager->createAnimation("CameraEffect", 0.50);
    // Spline it for nice curves
    anim->setInterpolationMode(Animation::IM_SPLINE);
    // Create a track to animate the camera's node
	NodeAnimationTrack* track = anim->createNodeTrack(0, mGameCameraNode);
    // Setup keyframes
	TransformKeyFrame* key = track->createNodeKeyFrame(0);
	key->setTranslate(Vector3::ZERO);
	key = track->createNodeKeyFrame(0.10);
	key->setTranslate(Vector3(-scale, -scale, 0));
	key = track->createNodeKeyFrame(0.20);
	key->setTranslate(Vector3(scale, scale, 0));
	key = track->createNodeKeyFrame(0.30);
	key->setTranslate(Vector3(-scale/2, -scale/2, 0));
	key = track->createNodeKeyFrame(0.40);
	key->setTranslate(Vector3(scale/2, scale/2, 0));
	key = track->createNodeKeyFrame(0.50);
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
	anim = mSceneManager->createAnimation("CameraEffectLook", 0.50);
    // Spline it for nice curves
    anim->setInterpolationMode(Animation::IM_SPLINE);
    // Create a track to animate the camera's node
	track = anim->createNodeTrack(0, mGameCameraLookAtNode);
    // Setup keyframes
	key = track->createNodeKeyFrame(0);
	key->setTranslate(Vector3::ZERO);
	key = track->createNodeKeyFrame(0.10);
	key->setTranslate(Vector3(-scale, -scale, 0));
	key = track->createNodeKeyFrame(0.20);
	key->setTranslate(Vector3(scale, scale, 0));
	key = track->createNodeKeyFrame(0.30);
	key->setTranslate(Vector3(-scale/2, -scale/2, 0));
	key = track->createNodeKeyFrame(0.40);
	key->setTranslate(Vector3(scale/2, scale/2, 0));
	key = track->createNodeKeyFrame(0.50);
	key->setTranslate(Vector3::ZERO);
    // Create a new animation state to track this
	if(mSceneManager->hasAnimationState("CameraEffectLook"))
		mSceneManager->destroyAnimationState("CameraEffectLook");
	mCameraEffectLook = mSceneManager->createAnimationState("CameraEffectLook");
    mCameraEffectLook->setEnabled(true);
	mCameraEffectLook->setWeight(1);
	mCameraEffectLook->setLoop(false);
}

void CameraManager::tremor(Real scale)
{
	// Camera node
	if(mSceneManager->hasAnimation("CameraEffect"))
		mSceneManager->destroyAnimation("CameraEffect");
	Animation* anim = mSceneManager->createAnimation("CameraEffect", 1);
    // Spline it for nice curves
    anim->setInterpolationMode(Animation::IM_SPLINE);
    // Create a track to animate the camera's node
	NodeAnimationTrack* track = anim->createNodeTrack(0, mGameCameraNode);
    // Setup keyframes
	TransformKeyFrame* key = track->createNodeKeyFrame(0);
	key->setTranslate(Vector3::ZERO);
	key = track->createNodeKeyFrame(0.1);
	key->setTranslate(Vector3(0, -scale, 0));
	key = track->createNodeKeyFrame(0.2);
	key->setTranslate(Vector3(0, scale, 0));
	key = track->createNodeKeyFrame(0.3);
	key->setTranslate(Vector3(0, -scale*2/3, 0));
	key = track->createNodeKeyFrame(0.4);
	key->setTranslate(Vector3(0, scale*2/3, 0));
	key = track->createNodeKeyFrame(0.5);
	key->setTranslate(Vector3(0, -scale/2, 0));
	key = track->createNodeKeyFrame(0.6);
	key->setTranslate(Vector3(0, scale/2, 0));
	key = track->createNodeKeyFrame(0.7);
	key->setTranslate(Vector3(0, -scale/3, 0));
	key = track->createNodeKeyFrame(0.8);
	key->setTranslate(Vector3(0, scale/3, 0));
	key = track->createNodeKeyFrame(0.9);
	key->setTranslate(Vector3(0, -scale/3, 0));
	key = track->createNodeKeyFrame(1);
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
	anim = mSceneManager->createAnimation("CameraEffectLook", 1);
    // Spline it for nice curves
    anim->setInterpolationMode(Animation::IM_SPLINE);
    // Create a track to animate the camera's node
	track = anim->createNodeTrack(0, mGameCameraLookAtNode);
    // Setup keyframes
	key = track->createNodeKeyFrame(0);
	key->setTranslate(Vector3::ZERO);
	key = track->createNodeKeyFrame(0.1);
	key->setTranslate(Vector3(0, -scale, 0));
	key = track->createNodeKeyFrame(0.2);
	key->setTranslate(Vector3(0, scale, 0));
	key = track->createNodeKeyFrame(0.3);
	key->setTranslate(Vector3(0, -scale*2/3, 0));
	key = track->createNodeKeyFrame(0.4);
	key->setTranslate(Vector3(0, scale*2/3, 0));
	key = track->createNodeKeyFrame(0.5);
	key->setTranslate(Vector3(0, -scale/2, 0));
	key = track->createNodeKeyFrame(0.6);
	key->setTranslate(Vector3(0, scale/2, 0));
	key = track->createNodeKeyFrame(0.7);
	key->setTranslate(Vector3(0, -scale/3, 0));
	key = track->createNodeKeyFrame(0.8);
	key->setTranslate(Vector3(0, scale/3, 0));
	key = track->createNodeKeyFrame(0.9);
	key->setTranslate(Vector3(0, -scale/3, 0));
	key = track->createNodeKeyFrame(1);
	key->setTranslate(Vector3::ZERO);
    // Create a new animation state to track this
	if(mSceneManager->hasAnimationState("CameraEffectLook"))
		mSceneManager->destroyAnimationState("CameraEffectLook");
	mCameraEffectLook = mSceneManager->createAnimationState("CameraEffectLook");
    mCameraEffectLook->setEnabled(true);
	mCameraEffectLook->setWeight(1);
	mCameraEffectLook->setLoop(false);
}

void CameraManager::shake(Real scale)
{
	// Camera node translation animation
	if(mSceneManager->hasAnimation("CameraEffect"))
		mSceneManager->destroyAnimation("CameraEffect");
	Animation* anim = mSceneManager->createAnimation("CameraEffect", 0.3);
    // Spline it for nice curves
    anim->setInterpolationMode(Animation::IM_SPLINE);
    // Create a track to animate the camera's node
	NodeAnimationTrack* track = anim->createNodeTrack(0, mGameCameraNode);
    // Setup keyframes
	TransformKeyFrame* key = track->createNodeKeyFrame(0);
	key->setTranslate(Vector3::ZERO);
	key = track->createNodeKeyFrame(0.1);
	key->setTranslate(Vector3(0, -scale, 0));
	key = track->createNodeKeyFrame(0.2);
	key->setTranslate(Vector3(0, scale, 0));
	key = track->createNodeKeyFrame(0.3);
	key->setTranslate(Vector3::ZERO);
    // Create a new animation state to track this
	if(mSceneManager->hasAnimationState("CameraEffect"))
		mSceneManager->destroyAnimationState("CameraEffect");
	mCameraEffect = mSceneManager->createAnimationState("CameraEffect");
    mCameraEffect->setEnabled(true);
	mCameraEffect->setWeight(1);
	mCameraEffect->setLoop(false);

	// Camera look at node translation animation
	if(mSceneManager->hasAnimation("CameraEffectLook"))
		mSceneManager->destroyAnimation("CameraEffectLook");
	anim = mSceneManager->createAnimation("CameraEffectLook", 0.3);
    // Spline it for nice curves
    anim->setInterpolationMode(Animation::IM_SPLINE);
    // Create a track to animate the camera's node
	track = anim->createNodeTrack(0, mGameCameraLookAtNode);
    // Setup keyframes
	key = track->createNodeKeyFrame(0);
	key->setTranslate(Vector3::ZERO);
	key = track->createNodeKeyFrame(0.1);
	key->setTranslate(Vector3(0, scale, 0));
	key = track->createNodeKeyFrame(0.2);
	key->setTranslate(Vector3(0, -scale, 0));
	key = track->createNodeKeyFrame(0.3);
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
	mGameCameraMode = "Game";
	mCameraMan->setStyle(OgreBites::CS_MANUAL);
	mGameCamera->setAutoTracking(true, mGameCameraLookAtNode, Vector3::UNIT_X);
}
void CameraManager::freeCamera()
{
	mGameCameraMode = "Free";
	mCameraMan->setStyle(OgreBites::CS_FREELOOK);
	mGameCamera->setAutoTracking(false);
	mGameViewport->setCamera(mGameCamera);
}
void CameraManager::fixedCamera(int camera)
{	
	char camName[20];
	sprintf(camName, "Fixed %d", camera);
	mGameCameraMode = camName;
	mCameraMan->setStyle(OgreBites::CS_MANUAL);
	mGameCamera->setAutoTracking(true, mGameCameraLookAtNode, Vector3::UNIT_X);
	mGameCamera->setPosition(Vector3::ZERO);
	mGameViewport->setCamera(mGameCamera);

	// Translate animation to camera scene node and look at node to current position
	createTransition(getCameraPosition(), mFixedCameras[camera].mPosition, getCameraLookAt(), mFixedCameras[camera].mLookAt);

	// Save fixed camera index
	mFixedCamera = camera;
}

void CameraManager::resumeCamera()
{
	if( mGameCameraMode == "Game" )
	{
		freeCamera();
		gameCamera();
	}
	else if ( mGameCameraMode == "Free" )
		freeCamera();
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
EVENTS_END_REGISTER_HANDLERS()

EVENTS_BEGIN_UNREGISTER_HANDLERS(CameraManager)
	EVENTS_UNREGISTER_HANDLER(CameraManager,EnemyKilled)
EVENTS_END_UNREGISTER_HANDLERS()

EVENTS_DEFINE_HANDLER(CameraManager,EnemyKilled)
{
	EnemyPtr enemy = evt->getEnemy();
	PlayerPtr player = evt->getPlayer();

	if( enemy->hasDieAnimation() )
	{
		this->shake((rand()%4)*2);
	}
	else
	{
		this->rumble((rand()%4));
	}

	if( player->isSpecial() )
	{
		this->zoom(player->getSpecialLength());
	}
}

// --------------------------------
// Lua Camera Lib
// --------------------------------
LUA_BEGIN_BINDING(CameraManager, cameralib)
LUA_BIND(CameraManager, setCurrent)
LUA_BIND(CameraManager, getCurrent)
LUA_BIND(CameraManager, rotate)
LUA_BIND(CameraManager, translate)
LUA_BIND(CameraManager, lookAt)
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

	//
	// Set current camera
	//

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

	/* return the number of results */
	return 1;
}

LUA_DEFINE_FUNCTION(CameraManager, translate)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	//
	// Retrieve the CameraManager
	//
	CameraManager* cameraManager = CameraManager::getSingletonPtr();

	/* return the number of results */
	return 0;
}

LUA_DEFINE_FUNCTION(CameraManager, rotate)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	//
	// Retrieve the CameraManager
	//
	CameraManager* cameraManager = CameraManager::getSingletonPtr();

	/* return the number of results */
	return 0;
}

LUA_DEFINE_FUNCTION(CameraManager, lookAt)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	//
	// Retrieve the CameraManager
	//
	CameraManager* cameraManager = CameraManager::getSingletonPtr();

	/* return the number of results */
	return 0;
}

LUA_DEFINE_FUNCTION(CameraManager, moveTo)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	//
	// Retrieve the CameraManager
	//
	CameraManager* cameraManager = CameraManager::getSingletonPtr();

	/* return the number of results */
	return 0;
}

LUA_DEFINE_FUNCTION(CameraManager, flyTo)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	//
	// Retrieve the CameraManager
	//
	CameraManager* cameraManager = CameraManager::getSingletonPtr();

	/* return the number of results */
	return 0;
}

LUA_DEFINE_FUNCTION(CameraManager, hasArrived)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	//
	// Retrieve the CameraManager
	//
	CameraManager* cameraManager = CameraManager::getSingletonPtr();

	/* return the number of results */
	return 1;
}

LUA_DEFINE_FUNCTION(CameraManager, strife)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	//
	// Retrieve the CameraManager
	//
	CameraManager* cameraManager = CameraManager::getSingletonPtr();

	/* return the number of results */
	return 0;
}

LUA_DEFINE_FUNCTION(CameraManager, shake)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	//
	// Retrieve the CameraManager
	//
	CameraManager* cameraManager = CameraManager::getSingletonPtr();

	/* return the number of results */
	return 0;
}

LUA_DEFINE_FUNCTION(CameraManager, rumble)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	//
	// Retrieve the CameraManager
	//
	CameraManager* cameraManager = CameraManager::getSingletonPtr();

	/* return the number of results */
	return 0;
}

LUA_DEFINE_FUNCTION(CameraManager, tremor)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	//
	// Retrieve the CameraManager
	//
	CameraManager* cameraManager = CameraManager::getSingletonPtr();

	/* return the number of results */
	return 0;
}

LUA_DEFINE_FUNCTION(CameraManager, zoom)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	//
	// Retrieve the CameraManager
	//
	CameraManager* cameraManager = CameraManager::getSingletonPtr();

	/* return the number of results */
	return 0;
}
#include "..\include\Camera\CameraManager.h"

using namespace Ogre;
using namespace WyvernsAssault;

CameraManager::CameraManager(SceneManager* sceneManager, RenderWindow* window, Viewport* viewport)
{
	this->mSceneManager = sceneManager;
	this->mWindow = window;
	this->mViewport = viewport;

	mCamera = NULL;
}

CameraManager::~CameraManager()
{
	finalize();
}

/** Initialize the camera manager */
void CameraManager::initialize()
{
	if(mSceneManager->hasCamera( "GameCamera" ))
		mCamera = mSceneManager->getCamera( "GameCamera" );
	else
		mCamera = mSceneManager->createCamera( "GameCamera" );

	/** Game camera cofiguration */
	mCamera->setNearClipDistance(1);
	mCamera->setFarClipDistance(20000);
	mCamera->setAspectRatio(Real(mViewport->getActualWidth()) / Real(mViewport->getActualHeight()));	
	mViewport->setCamera(mCamera);

	/** Camera Node and Look At Node */
	mCameraNode = mSceneManager->getRootSceneNode()->createChildSceneNode("CameraNode");
	mCameraLookAtNode = mSceneManager->getRootSceneNode()->createChildSceneNode("LookAtNode");
	mCameraNode->attachObject(mCamera);
	mCamera->setAutoTracking(true, mCameraLookAtNode, Vector3::UNIT_X);

	/** Debug axes node */
	mSceneManager->createEntity("Axes", "axes.mesh");
	mAxesNode = mSceneManager->getRootSceneNode()->createChildSceneNode("AxesNode");
	mAxesNode->setScale(0.1, 0.1, 0.1);
	mAxesNode->setVisible(false);
	
	/** Other variables initialization */
	mCameraMode = "NoMode";
	mGameArea = 0;
	mCameraZoom = 0;

	mSceneManager->createAnimation("CameraTrack", 1);
	mSceneManager->createAnimation("LookAtTrack", 1);
	mSceneManager->createAnimation("AxesTrack", 1);
	mSceneManager->createAnimation("CameraEffect", 1);	
	mCameraTransition = mSceneManager->createAnimationState("CameraTrack");
	mCameraTransition->setEnabled(false);
	mLookAtTransition = mSceneManager->createAnimationState("LookAtTrack");
	mLookAtTransition->setEnabled(false);
	mAxesTransition = mSceneManager->createAnimationState("AxesTrack");
	mAxesTransition->setEnabled(false);
	mCameraEffect = mSceneManager->createAnimationState("CameraEffect");
	mCameraEffect->setEnabled(false);

	/** Sdk Camera Manager for free look camera */
	mCameraMan = new OgreBites::SdkCameraMan(mCamera);
	mCameraMan->setStyle(OgreBites::CS_FREELOOK);
}

/** Finalize the camera manager */
void CameraManager::finalize()
{	
	if(mCamera)
	{
		mSceneManager->destroyCamera( "GameCamera" );
		mCamera = NULL;
	}
	if(mCameraMan)
	{
		delete mCameraMan;
		mCameraMan = 0;
	}
}

Vector3 CameraManager::getCameraPosition()
{ 
	if(mCameraMode == "Free")
		return mCamera->getRealPosition();
	else
		return mCameraNode->getPosition();
}

Vector3 CameraManager::getCameraLookAt()
{ 
	if(mCameraMode == "Free")
		return mCamera->getDirection();
	else
		return mCameraLookAtNode->getPosition();
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

	// Checking game area for x direction (begin-end camera segments problem). NEED IMPROVING!

	if(direction == Vector3(1,0,0))
	{
		if(mGameArea < 7)
			finalDirection = mCameraSegments[mGameArea].mPositionEnd - mCameraSegments[mGameArea].mPositionBegin;
		else
			finalDirection = mCameraSegments[mGameArea].mPositionBegin - mCameraSegments[mGameArea].mPositionEnd;
	}
	else if(direction == Vector3(-1,0,0))
	{
		if(mGameArea < 7)
			finalDirection = mCameraSegments[mGameArea].mPositionBegin - mCameraSegments[mGameArea].mPositionEnd;
		else
			finalDirection = mCameraSegments[mGameArea].mPositionEnd - mCameraSegments[mGameArea].mPositionBegin;
	}
	else if(direction == Vector3(0,0,1))
	{
		finalDirection = getCameraPosition() - getCameraLookAt();
	}
	else if(direction == Vector3(0,0,-1))
	{
		finalDirection = getCameraLookAt() - getCameraPosition();
	}
	else if(direction == Vector3(1,0,1))
	{
		if(mGameArea < 7)
			finalDirection = mCameraSegments[mGameArea].mPositionEnd - mCameraSegments[mGameArea].mPositionBegin;
		else
			finalDirection = mCameraSegments[mGameArea].mPositionBegin - mCameraSegments[mGameArea].mPositionEnd;

		finalDirection = finalDirection + (getCameraPosition() - getCameraLookAt());
	}
	else if(direction == Vector3(1,0,-1))
	{
		if(mGameArea < 7)
			finalDirection = mCameraSegments[mGameArea].mPositionEnd - mCameraSegments[mGameArea].mPositionBegin;
		else
			finalDirection = mCameraSegments[mGameArea].mPositionBegin - mCameraSegments[mGameArea].mPositionEnd;

		finalDirection = finalDirection + (getCameraLookAt() - getCameraPosition());
	}
	else if(direction == Vector3(-1,0,1))
	{
		if(mGameArea < 7)
			finalDirection = mCameraSegments[mGameArea].mPositionBegin - mCameraSegments[mGameArea].mPositionEnd;
		else
			finalDirection = mCameraSegments[mGameArea].mPositionEnd - mCameraSegments[mGameArea].mPositionBegin;

		finalDirection = finalDirection + (getCameraPosition() - getCameraLookAt());
	}		
	else if(direction == Vector3(-1,0,-1))
	{
		if(mGameArea < 7)
			finalDirection = mCameraSegments[mGameArea].mPositionBegin - mCameraSegments[mGameArea].mPositionEnd;
		else
			finalDirection = mCameraSegments[mGameArea].mPositionEnd - mCameraSegments[mGameArea].mPositionBegin;

		finalDirection = finalDirection + (getCameraLookAt() - getCameraPosition());
	}

	// Don't use y component
	finalDirection = finalDirection * Vector3(1,0,1);
	// Normalize vector for constant speed
	finalDirection.normalise();
	return finalDirection;
}

/** Camera functions **/

void CameraManager::updateCamera(Vector3 player, Real elapsedSeconds)
{
	// Update camera camera
	if(mCameraMode == "Game")
	{
		mGameArea = getGameArea(player);

		Vector3 begin = mCameraSegments[mGameArea].mPositionBegin;
		Vector3 end = mCameraSegments[mGameArea].mPositionEnd;
		Vector3 lbegin = mCameraSegments[mGameArea].mLookAtBegin;
		Vector3 lend = mCameraSegments[mGameArea].mLookAtEnd;

		Vector3 position;
		Vector3 lookAt;

		// Movement in XZ (side scroll in X)
		if((end.x - begin.x) > (begin.z - end.z))
		{
			// If player position not in camera segment, camera position = begin/end of current segment
			if(player.x < begin.x)
			{
				position.x = begin.x;
			}
			else if(player.x > end.x)
			{
				position.x = end.x;
			}
			else
			{
				position.x = player.x;
			}

			Real gap = (position.x - begin.x) / (end.x - begin.x);
			position.z = begin.z - (gap * (begin.z - end.z))  + mCameraZoom;
			position.y = begin.y + (gap * (end.y - begin.y));
		}
		// Movement in ZX (side scroll in Z). Be carefull with Z index!
		else
		{
			// If player position not in camera segment, camera position = begin/end of current segment
			if(player.z > begin.z)
			{
				position.z = begin.z;
			}
			else if(player.z < end.z)
			{
				position.z = end.z;
			}
			else
			{
				position.z = player.z;
			}

			Real gap = (begin.z - position.z) / (begin.z - end.z);
			position.x = begin.x + (gap * (end.x - begin.x)) + mCameraZoom;
			position.y = begin.y + (gap * (end.y - begin.y));
		}		
		
		// Get current look at position with distances
		Real percent = begin.distance(position) / begin.distance(end);
		
		lookAt.x = lbegin.x + ((lend.x - lbegin.x) * percent);
		lookAt.y = lbegin.y + ((lend.y - lbegin.y) * percent);
		lookAt.z = lbegin.z + ((lend.z - lbegin.z) * percent);

		// Move camera to ZERO
		mCamera->setPosition(Vector3::ZERO);
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
	mCameraTransition->addTime(elapsedSeconds);
	mLookAtTransition->addTime(elapsedSeconds);
	mAxesTransition->addTime(elapsedSeconds);
}

void CameraManager::createTransition(Vector3 begin, Vector3 end, Vector3 lbegin, Vector3 lend)
{
	/** Camera positioning translation animation */
	Real dist = begin.distance(end) / 100;
	if(dist > 0.25) 
		dist = 0.25;
	
	// Set up spline animation of node
	if(mSceneManager->hasAnimation("CameraTrack"))
		mSceneManager->destroyAnimation("CameraTrack");
	Animation* anim = mSceneManager->createAnimation("CameraTrack", dist);
    // Spline it for nice curves
    anim->setInterpolationMode(Animation::IM_SPLINE);
    // Create a track to animate the camera's node
	NodeAnimationTrack* track = anim->createNodeTrack(0, mCameraNode);
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
	track = anim->createNodeTrack(0, mCameraLookAtNode);
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
void CameraManager::zoom(Real zoom)
{
	mCameraZoom += zoom;
}

void CameraManager::rumble()
{
	if(mSceneManager->hasAnimation("CameraEffect"))
		mSceneManager->destroyAnimation("CameraEffect");
	Animation* anim = mSceneManager->createAnimation("CameraEffect", 0.50);
    // Spline it for nice curves
    anim->setInterpolationMode(Animation::IM_SPLINE);
    // Create a track to animate the camera's node
	NodeAnimationTrack* track = anim->createNodeTrack(0, mCameraNode);
    // Setup keyframes
	TransformKeyFrame* key = track->createNodeKeyFrame(0);
	key->setTranslate(Vector3::ZERO);
	key = track->createNodeKeyFrame(0.10);
	key->setTranslate(Vector3(-10, -10, 0));
	key = track->createNodeKeyFrame(0.20);
	key->setTranslate(Vector3(10, 10, 0));
	key = track->createNodeKeyFrame(0.30);
	key->setTranslate(Vector3(-6, -6, 0));
	key = track->createNodeKeyFrame(0.40);
	key->setTranslate(Vector3(6, 6, 0));
	key = track->createNodeKeyFrame(0.50);
	key->setTranslate(Vector3::ZERO);
    // Create a new animation state to track this
	if(mSceneManager->hasAnimationState("CameraEffect"))
		mSceneManager->destroyAnimationState("CameraEffect");
	mCameraEffect = mSceneManager->createAnimationState("CameraEffect");
    mCameraEffect->setEnabled(true);
	mCameraEffect->setWeight(1);
	mCameraEffect->setLoop(false);
}

void CameraManager::tremor()
{
	if(mSceneManager->hasAnimation("CameraEffect"))
		mSceneManager->destroyAnimation("CameraEffect");
	Animation* anim = mSceneManager->createAnimation("CameraEffect", 1);
    // Spline it for nice curves
    anim->setInterpolationMode(Animation::IM_SPLINE);
    // Create a track to animate the camera's node
	NodeAnimationTrack* track = anim->createNodeTrack(0, mCameraNode);
    // Setup keyframes
	TransformKeyFrame* key = track->createNodeKeyFrame(0);
	key->setTranslate(Vector3::ZERO);
	key = track->createNodeKeyFrame(0.1);
	key->setTranslate(Vector3(0, -30, 0));
	key = track->createNodeKeyFrame(0.2);
	key->setTranslate(Vector3(0, 30, 0));
	key = track->createNodeKeyFrame(0.3);
	key->setTranslate(Vector3(0, -20, 0));
	key = track->createNodeKeyFrame(0.4);
	key->setTranslate(Vector3(0, 20, 0));
	key = track->createNodeKeyFrame(0.5);
	key->setTranslate(Vector3(0, -15, 0));
	key = track->createNodeKeyFrame(0.6);
	key->setTranslate(Vector3(0, 15, 0));
	key = track->createNodeKeyFrame(0.7);
	key->setTranslate(Vector3(0, -10, 0));
	key = track->createNodeKeyFrame(0.8);
	key->setTranslate(Vector3(0, 10, 0));
	key = track->createNodeKeyFrame(0.9);
	key->setTranslate(Vector3(0, -5, 0));
	key = track->createNodeKeyFrame(1);
	key->setTranslate(Vector3::ZERO);
    // Create a new animation state to track this
	if(mSceneManager->hasAnimationState("CameraEffect"))
		mSceneManager->destroyAnimationState("CameraEffect");
	mCameraEffect = mSceneManager->createAnimationState("CameraEffect");
    mCameraEffect->setEnabled(true);
	mCameraEffect->setWeight(1);
	mCameraEffect->setLoop(false);
}

/** Camera modes */
void CameraManager::gameCamera(Vector3 player)
{
	mCameraMode = "Game";
	mCameraMan->setStyle(OgreBites::CS_MANUAL);
	mCamera->setAutoTracking(true, mCameraLookAtNode, Vector3::UNIT_X);
}
void CameraManager::freeCamera()
{
	mCameraMode = "Free";
	mCameraMan->setStyle(OgreBites::CS_FREELOOK);
	mCamera->setAutoTracking(false);
}
void CameraManager::fixedCamera(int camera)
{
	char camName[20];
	sprintf(camName, "Fixed %d", camera);
	mCameraMode = camName;
	mCameraMan->setStyle(OgreBites::CS_MANUAL);
	mCamera->setAutoTracking(true, mCameraLookAtNode, Vector3::UNIT_X);
	mCamera->setPosition(Vector3::ZERO);

	// Translate animation to camera scene node and look at node to current position
	createTransition(getCameraPosition(), mFixedCameras[camera].mPosition, getCameraLookAt(), mFixedCameras[camera].mLookAt);
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
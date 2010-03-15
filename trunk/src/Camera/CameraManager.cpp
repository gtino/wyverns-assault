#include "..\include\Camera\CameraManager.h"

using namespace Ogre;
using namespace WyvernsAssault;

CameraManager::CameraManager(SceneManager* sceneManager, RenderWindow* renderWindow, Viewport* viewport)
{
	this->mSceneManager = sceneManager;
	this->mRenderWindow = renderWindow;
	this->mViewport = viewport;
}

CameraManager::~CameraManager()
{
	finalize();
}

/** Initialize the camera manager */
void CameraManager::initialize()
{
	mCameraType = FIXEDCAMERA;
	mCamera = mSceneManager->createCamera( "Camera" );
	
	mSceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(mCamera);
	
	mCamera->setNearClipDistance(5);
	mCamera->setFarClipDistance(5000);
	mCamera->setAspectRatio(Real(mViewport->getActualWidth()) / Real(mViewport->getActualHeight()));
	
	mViewport->setCamera(mCamera);
}

/** Finalize the camera manager */
void CameraManager::finalize()
{
	mSceneManager->destroyAllCameras();
}

/** Camera functions **/

void CameraManager::positionCamera(Vector3 position)
{
	mCamera->setPosition(position);
}

void CameraManager::rotateCamera(Radian x, Radian y, Radian z)
{
	if (x.valueRadians() != 0) mCamera->roll(x);
	if (y.valueRadians() != 0) mCamera->yaw(y);
	if (z.valueRadians() != 0) mCamera->pitch(z);
}

void CameraManager::lookAtCamera(Vector3 lookAt)
{
	mCamera->lookAt(lookAt);
}

void CameraManager::moveCamera(Vector3 move)
{
	mCamera->move(move);
}

void CameraManager::followNode(SceneNode* node, Vector3 offset)
{
	mCamera->setAutoTracking(true, node, offset);
}

void CameraManager::updateCamera(SceneNode* node)
{
	float cameraHeight = 10;
	float cameraZ = 10;

	switch(mCameraType)
	{
		case GAMECAMERA:
			// Camera positioning	-	NEED IMPROVING
			if (node->getPosition().z > -45)
			{
				cameraHeight	=	(55 - ((45 + node->getPosition().z)*2)) + node->getPosition().z;
				cameraZ			=	(55 - ((45 + node->getPosition().z)*2)) + node->getPosition().z;
				cameraZ			=	(cosf(cameraHeight * PI/180) * 10);
			}
			positionCamera(Vector3(node->getPosition().x, cameraHeight, cameraZ));			
			break;

		case FPSCAMERA:
			positionCamera(node->getPosition() + Vector3(0,PLAYERHEIGHT,-PLAYERWIDTH));
			lookAtCamera(node->getPosition() + Vector3(25,-PLAYERHEIGHT,-PLAYERWIDTH));
			break;

		case FIXEDCAMERA:

			break;

		case TRAVELCAMERA:

			break;
	}
}

/** Camera types **/

void CameraManager::gameCamera(SceneNode* node)
{
	mCameraType = GAMECAMERA;
	followNode(node);
	positionCamera(node->getPosition() + Vector3(5,30,50));
}

void CameraManager::fpsCamera(SceneNode* node)
{
	mCameraType = FPSCAMERA;
	mCamera->setAutoTracking(false);
	positionCamera(node->getPosition() + Vector3(0,PLAYERHEIGHT,-PLAYERWIDTH));
	lookAtCamera(node->getPosition() + Vector3(25,-PLAYERHEIGHT,-PLAYERWIDTH));
}

void CameraManager::fixedCamera(int id)
{
	mCameraType = FIXEDCAMERA;
	mCamera->setAutoTracking(false);
	positionCamera(mFixedCameras[id][0]);
	lookAtCamera(mFixedCameras[id][1]);
}

void CameraManager::travelCamera(int id)
{
	mCameraType = TRAVELCAMERA;
	mCamera->setAutoTracking(false);

	// Position camera at first travel point
	moveCamera(mCamera->getPosition() - mTravelCameras[id][0][0]);
	lookAtCamera(mTravelCameras[id][0][1]);

	// Move the camer through travel point
	/*for(int point = 1; point < TRAVELPOINTS; point++)
	{
		if (mTravelCameras[id][point][0] == Vector3::ZERO || mTravelCameras[id][point][1] == Vector3::ZERO) break;
		moveCamera(mTravelCameras[id][point][0] - mTravelCameras[id][point-1][0]);
		lookAtCamera(mTravelCameras[id][point][1]);
	}*/
}

/** Fixed cameras functions **/
void CameraManager::setFixedCamera(int camera, Vector3 position, Vector3 lookAt)
{
	mFixedCameras[camera][0] = position;
	mFixedCameras[camera][1] = lookAt;
}

/** Travel cameras functions **/
void CameraManager::setTravelCamera(int camera, int point, Vector3 position, Vector3 lookAt)
{
	mTravelCameras[camera][point][0] = position;
	mTravelCameras[camera][point][1] = lookAt;
}

/** Debug camera functions **/
void CameraManager::switchtPolygonMode()
{
	switch(mCamera->getPolygonMode())
	{	
		case PM_POINTS:
			mCamera->setPolygonMode(PM_WIREFRAME);
			break;
		case PM_WIREFRAME:
			mCamera->setPolygonMode(PM_SOLID);
			break;
		case PM_SOLID:
			mCamera->setPolygonMode(PM_POINTS);
			break;
	}
}

String CameraManager::getPolygonMode()
{
	if(mCamera->getPolygonMode() == PM_POINTS) return "point";
	else if(mCamera->getPolygonMode() == PM_WIREFRAME) return "wireframe";
	else if(mCamera->getPolygonMode() == PM_SOLID) return "solid";
	else return "unknown";
};

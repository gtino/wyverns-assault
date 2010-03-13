#include "..\include\Camera\CameraManager.h"

using namespace Ogre;
using namespace WyvernsAssault;

CameraManager::CameraManager(SceneManager* sceneManager, RenderWindow* renderWindow)
{
	this->mSceneManager = sceneManager;
	this->mRenderWindow = renderWindow;
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
	mCamera->setNearClipDistance(1);
	mCamera->setFarClipDistance(1000);
	mViewport = mRenderWindow->addViewport( mCamera );
	mFixedCameras[0][0] = Vector3(100,50,100);
	mFixedCameras[0][1] = Vector3(10,0,10);
	mFixedCameras[1][0] = Vector3(300,50,150);
	mFixedCameras[1][1] = Vector3(100,-5,-10);
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
	switch(mCameraType)
	{
		case GAMECAMERA:
			positionCamera(node->getPosition() + Vector3(-5,30,80));
			break;

		case FPSCAMERA:
			positionCamera(node->getPosition());
			break;

		case FIXEDCAMERA:

			break;

		case TRAVELLCAMERA:

			break;
	}
}

/** Camera types **/

void CameraManager::gameCamera(SceneNode* node)
{
	mCameraType = GAMECAMERA;
	followNode(node);
	positionCamera(node->getPosition() + Vector3(-5,30,80));
}

void CameraManager::fpsCamera(SceneNode* node)
{
	mCameraType = FPSCAMERA;
	mCamera->setAutoTracking(false);
	//followNode(node);
	positionCamera(node->getPosition());
	lookAtCamera(node->getPosition() + Vector3(100,PLAYERHEIGHT,0));
}

void CameraManager::fixedCamera(int id)
{
	mCameraType = FIXEDCAMERA;
	mCamera->setAutoTracking(false);
	positionCamera(mFixedCameras[id][0]);
	lookAtCamera(mFixedCameras[id][1]);
}

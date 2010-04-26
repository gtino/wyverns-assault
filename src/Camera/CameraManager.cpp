#include "..\include\Camera\CameraManager.h"

using namespace Ogre;
using namespace WyvernsAssault;

CameraManager::CameraManager(SceneManager* sceneManager, RenderWindow* renderWindow, Viewport* viewport)
{
	this->mSceneManager = sceneManager;
	this->mRenderWindow = renderWindow;
	this->mViewport = viewport;

	mCamera = NULL;
	mCameraCS = NULL;
	mCamFixedDirMode = NULL;
	mCamFixedMode = NULL;
}

CameraManager::~CameraManager()
{
	finalize();
}

/** Initialize the camera manager */
void CameraManager::initialize(SceneNode* player)
{
	mCamera = mSceneManager->createCamera( "Camera" );
	mCamera->setNearClipDistance(10);
	mCamera->setFarClipDistance(15000);
	mCamera->setAspectRatio(Real(mViewport->getActualWidth()) / Real(mViewport->getActualHeight()));
	
	mViewport->setCamera(mCamera);

	// Camera control system
	mCameraCS = new CCS::CameraControlSystem(mSceneManager, "CCS", mCamera);
	mCameraCS->setCameraTarget(player);

	/** Define camera modes **/

	// Trough target
	SceneNode* centerNode = mSceneManager->getRootSceneNode()->createChildSceneNode( "Through" );
	centerNode->setPosition(400, -50, 300);
	mCamThroughMode = new CCS::ThroughTargetCameraMode(mCameraCS, 300);
	mCamThroughMode->setCameraFocusPosition(centerNode->_getDerivedPosition() - Ogre::Vector3(0, 250, 0));
    mCameraCS->registerCameraMode("Through Target", mCamThroughMode);

	// Free mode
	mCamFreeMode = new CCS::FreeCameraMode(mCameraCS);
	mCamFreeMode->setMoveFactor(20);
	mCameraCS->registerCameraMode("Free", mCamFreeMode);

	// Fixed direction
	mCamFixedDirMode = new CCS::FixedDirectionCameraMode(mCameraCS, Ogre::Vector3(0.0,-0.75,-1), 300);
	mCamFixedDirMode->setCameraTightness(0.05);
	mCameraCS->registerCameraMode("Fixed direction", mCamFixedDirMode);

	// Chase 
	mCamChaseMode = new CCS::ChaseCameraMode(mCameraCS, Ogre::Vector3(-500,150,0));    
    mCamChaseMode->setCameraTightness(0.05);
	mCameraCS->registerCameraMode("Chase", mCamChaseMode);

}

/** Finalize the camera manager */
void CameraManager::finalize()
{
	mSceneManager->destroyAllCameras();
	
	mCamera = NULL;
	mCameraCS = NULL;
	mCamFixedDirMode = NULL;
	mCamFixedMode = NULL;
}

/** Camera functions **/

void CameraManager::updateCamera(Real timeSinceLastFrame)
{
	mCameraCS->update(timeSinceLastFrame);
}

/** Camera types **/

void CameraManager::gameCamera()
{
	mCameraCS->setCurrentCameraMode(mCamThroughMode);
	mCamera->setNearClipDistance(10);
	mCamera->setFarClipDistance(15000);
}

void CameraManager::freeCamera()
{
	mCameraCS->setCurrentCameraMode(mCamFreeMode);
	mCamera->setNearClipDistance(10);
	mCamera->setFarClipDistance(15000);
}

void CameraManager::fixedCamera(int id)
{
	char cameraName [30];
	sprintf (cameraName, "Fixed %d", id);
	mCameraCS->setCurrentCameraMode(mCameraCS->getCameraMode(cameraName));
	mCamera->setNearClipDistance(100);
	mCamera->setFarClipDistance(15000);
}

/** Fixed cameras functions **/
void CameraManager::setFixedCamera(int camera, Vector3 position, Real roll, Real yaw, Real pitch)
{
	mCamFixedMode = new CCS::FixedCameraMode(mCameraCS);    
    mCamFixedMode->setCameraPosition(position);
    mCamFixedMode->setCameraOrientation( Quaternion(Radian(Degree(roll)),Vector3::UNIT_Z)
        * Quaternion(Radian(Degree(yaw)),Vector3::UNIT_Y)
        * Quaternion(Radian(Degree(pitch)),Vector3::UNIT_X));

	// Register in camera control system
	char cameraName [30];
	sprintf (cameraName, "Fixed %d", camera);
	mCameraCS->registerCameraMode(cameraName, mCamFixedMode);
}

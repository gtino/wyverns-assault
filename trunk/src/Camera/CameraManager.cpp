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
	mCamPlaneMode = NULL;
	mCamFixedDirMode = NULL;
	mCamFixedMode = NULL;

	distance = 500;
}

CameraManager::~CameraManager()
{
	finalize();
}

/** Initialize the camera manager */
void CameraManager::initialize(SceneNode* player)
{
	mCamera = mSceneManager->createCamera( "Camera" );

	mCamera->setNearClipDistance(1);
	mCamera->setFarClipDistance(20000);
	mCamera->setAspectRatio(Real(mViewport->getActualWidth()) / Real(mViewport->getActualHeight()));
	
	mViewport->setCamera(mCamera);

	// Camera control system
	mCameraCS = new CCS::CameraControlSystem(mSceneManager, "CCS", mCamera);

	mCameraCS->setCameraTarget(player);

	/** Define camera modes **/

	// Plane binded
	Plane* mPlane = new Plane(Vector3(0, -1, -1), Vector3(0,0,4000));
    mCamPlaneMode = new CCS::PlaneBindedCameraMode(mCameraCS, *mPlane);
	mCameraCS->registerCameraMode("Plane Binded", mCamPlaneMode);

	// Fixed direction
	mCamFixedDirMode = new CCS::FixedDirectionCameraMode(mCameraCS, Ogre::Vector3(0.0,-0.75,-1), distance);
	mCamFixedDirMode->setCameraTightness(0.05);
	mCameraCS->registerCameraMode("Fixed direction", mCamFixedDirMode);


	// First person
	/*mCamFirstPersonMode = new CCS::FirstPersonCameraMode(mCameraCS,Ogre::Vector3(0,17,-16)
            , Ogre::Radian(0),Ogre::Radian(Ogre::Degree(180)),Ogre::Radian(0));
	mCamFirstPersonMode->setCharacterVisible(false);
	mCameraCS->registerCameraMode("First Person", mCamFirstPersonMode);*/

	// Chase 
	mCamChaseMode = new CCS::ChaseCameraMode(mCameraCS, Ogre::Vector3(-400,70,20));    
    mCamChaseMode->setCameraTightness(0.05);
	mCameraCS->registerCameraMode("Chase", mCamChaseMode);

	// Chase free yaw axis
	/*mCamChaseFreeMode = new CCS::ChaseFreeYawAxisCameraMode(mCameraCS,Ogre::Vector3(-600,50,0)
            , Ogre::Radian(0),Ogre::Radian(Ogre::Degree(270)),Ogre::Radian(0));
	mCamChaseFreeMode->setCameraTightness(0.1);
    mCameraCS->registerCameraMode("Chase Free Yaw Axis",mCamChaseFreeMode);    

	// Attached lateral
	mCamAttachedMode = new CCS::AttachedCameraMode(mCameraCS,Ogre::Vector3(-200,50,0)
            , Ogre::Radian(0),Ogre::Radian(Ogre::Degree(270)),Ogre::Radian(0));
    mCameraCS->registerCameraMode("Attached (lateral)",mCamAttachedMode);*/


	// Trough target
	SceneNode* centerNode = mSceneManager->getSceneNode("Center");
	mCamThroughMode = new CCS::ThroughTargetCameraMode(mCameraCS, distance);
	mCamThroughMode->setCameraFocusPosition(centerNode->_getDerivedPosition() - Ogre::Vector3(0, 1000, 0));
    mCameraCS->registerCameraMode("Through Target", mCamThroughMode);

	// Free mode
	mCamFreeMode = new CCS::FreeCameraMode(mCameraCS);
	mCamFreeMode ->setMoveFactor(50);
	mCameraCS->registerCameraMode("Free", mCamFreeMode);
}

/** Finalize the camera manager */
void CameraManager::finalize()
{
	mSceneManager->destroyAllCameras();
	
	mCamera = NULL;
	mCameraCS = NULL;
	mCamPlaneMode = NULL;
	mCamFixedDirMode = NULL;
	mCamFixedMode = NULL;
}

/** Camera functions **/

void CameraManager::updateCamera(Real timeSinceLastFrame)
{
	mCameraCS->update(timeSinceLastFrame);
}
void CameraManager::zoom(Real zoom)
{ 
	distance += zoom * 20; 
	mCamFixedDirMode->setDistance(distance);
}


/** Camera types **/

void CameraManager::gameCamera()
{
	mCameraCS->setCurrentCameraMode(mCamThroughMode);
	mCamera->setNearClipDistance(1);
	mCamera->setFarClipDistance(5000);
}

void CameraManager::fpsCamera()
{
	mCameraCS->setCurrentCameraMode(mCamFreeMode);
	mCamera->setNearClipDistance(1);
	mCamera->setFarClipDistance(10000);
}

void CameraManager::fixedCamera(int id)
{
	char cameraName [30];
	sprintf (cameraName, "Fixed %d", id);
	mCameraCS->setCurrentCameraMode(mCameraCS->getCameraMode(cameraName));
	mCamera->setNearClipDistance(500);
	mCamera->setFarClipDistance(20000);
}

void CameraManager::travelCamera(int id)
{
	mCameraCS->setCurrentCameraMode(mCamChaseMode);
	mCamera->setNearClipDistance(1);
	mCamera->setFarClipDistance(5000);
}

void CameraManager::scenarioCamera()
{
	mCameraCS->setCurrentCameraMode(mCamFixedDirMode);
	mCamera->setNearClipDistance(1);
	mCamera->setFarClipDistance(10000);	
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


/** Debug camera functions **/
void CameraManager::switchPolygonMode()
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
			mCamera->setPolygonMode(PM_WIREFRAME);
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

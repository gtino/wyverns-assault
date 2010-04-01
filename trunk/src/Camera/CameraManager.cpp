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
}

CameraManager::~CameraManager()
{
	finalize();
}

/** Initialize the camera manager */
void CameraManager::initialize(SceneNode* player)
{
	mCamera = mSceneManager->createCamera( "Camera" );

	mCamera->setNearClipDistance(0.1);
	mCamera->setFarClipDistance(50000);
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
	mCamFixedDirMode = new CCS::FixedDirectionCameraMode(mCameraCS, Ogre::Vector3(0,-0.5,-1), 1000);
	mCameraCS->registerCameraMode("Fixed direction", mCamFixedDirMode);

	// First person
	mCamFirstPersonMode = new CCS::FirstPersonCameraMode(mCameraCS,Ogre::Vector3(0,17,-16)
            , Ogre::Radian(0),Ogre::Radian(Ogre::Degree(180)),Ogre::Radian(0));
	mCamFirstPersonMode->setCharacterVisible(false);
	mCameraCS->registerCameraMode("First Person", mCamFirstPersonMode);

	// Chase 
	mCamChaseMode = new CCS::ChaseCameraMode(mCameraCS, Ogre::Vector3(-200,70,20));    
    mCamChaseMode->setCameraTightness(0.2);
	mCameraCS->registerCameraMode("Chase", mCamChaseMode);

	// Chase free yaw axis
	mCamChaseFreeMode = new CCS::ChaseFreeYawAxisCameraMode(mCameraCS,Ogre::Vector3(-600,50,0)
            , Ogre::Radian(0),Ogre::Radian(Ogre::Degree(270)),Ogre::Radian(0));
	mCamChaseFreeMode->setCameraTightness(0.1);
    mCameraCS->registerCameraMode("ChaseFreeYawAxis",mCamChaseFreeMode);    

	// Attached lateral
	mCamAttachedMode = new CCS::AttachedCameraMode(mCameraCS,Ogre::Vector3(-200,50,0)
            , Ogre::Radian(0),Ogre::Radian(Ogre::Degree(270)),Ogre::Radian(0));
    mCameraCS->registerCameraMode("Attached (lateral)",mCamAttachedMode);

	// Fixed scenario camera
	mCamFixedMode = new CCS::FixedCameraMode(mCameraCS);    
    mCamFixedMode->setCameraPosition(Vector3(1000, 6500, 4500));
    mCamFixedMode->setCameraOrientation( Quaternion(Radian(Degree(0)),Vector3::UNIT_Z)
        * Quaternion(Radian(Degree(10)),Vector3::UNIT_Y)
        * Quaternion(Radian(Degree(-50)),Vector3::UNIT_X));

	// Register in camera control system
	mCameraCS->registerCameraMode("Scenario", mCamFixedMode);
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

void CameraManager::updateCamera(SceneNode* node)
{

}

/** Camera types **/

void CameraManager::gameCamera()
{
	mCameraType = GAMECAMERA;
	mCameraCS->setCurrentCameraMode(mCamPlaneMode);
}

void CameraManager::fpsCamera()
{
	mCameraType = FPSCAMERA;
	mCameraCS->setCurrentCameraMode(mCamFirstPersonMode);
}

void CameraManager::fixedCamera(int id)
{
	mCameraType = FIXEDCAMERA;
	mCameraCS->setCurrentCameraMode(mCameraCS->getCameraMode("Fixed_" + id));
}

void CameraManager::travelCamera(int id)
{
	mCameraType = TRAVELCAMERA;
}

void CameraManager::scenarioCamera()
{
	mCameraType = FIXEDCAMERA;
	mCameraCS->setCurrentCameraMode(mCameraCS->getCameraMode("Scenario"));
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

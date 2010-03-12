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
	//
	// TODO Destructor
	//
	finalize();
}

/** Initialize the audio manager */
void CameraManager::initialize()
{
	setDefaultCamera();
	setCamera("Camera1", Vector3(1.0,1.0,1.0), Vector3(0.0,0.0,0.0));
	setCamera("Camera2", Vector3(10.0,10.0,10.0), Vector3(0.0,0.0,0.0));
}

/** Finalize the audio manager */
void CameraManager::finalize()
{
	mSceneManager->destroyAllCameras();
}

void CameraManager::loadResources()
{
	//
	// TODO Load resources
	//
}

void CameraManager::unloadResources()
{
	//
	// TODO Unload resources
	//
}

void CameraManager::setCamera(String name, Vector3 position, Vector3 lookAt)
{
	mCamera = mSceneManager->createCamera(name);
	mCamera->setPosition(position);
	mCamera->lookAt(lookAt);
}

Camera* CameraManager::getCamera(String name)
{
	return this->mSceneManager->getCamera(name);
}

void CameraManager::enableCamera(String name)
{

}

void CameraManager::setDefaultCamera()
{
	mDefaultCamera = mSceneManager->createCamera( "Default" );
}

Camera* CameraManager::getDefaultCamera()
{
	return mDefaultCamera;
}

void CameraManager::enableDetaultCamera()
{

}

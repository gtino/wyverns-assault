#include "..\include\Camera\CameraManager.h"

using namespace Ogre;
using namespace WyvernsAssault;

CameraManager::CameraManager(SceneManager* sceneManager, Root* root)
{
	this->mRoot = root;
	this->mSceneManager = sceneManager;
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
	mDefaultCamera = mSceneManager->createCamera( "Default" );
}

/** Finalize the audio manager */
void CameraManager::finalize()
{
	//
	// TODO Finalize
	//
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

/*****************/

void CameraManager::setCamera(String name, Vector3 position, Vector3 lookAt)
{
	Camera* mCamera = this->mSceneManager->createCamera(name);
	mCamera->setPosition(position);
	mCamera->lookAt(lookAt);
}

Camera* CameraManager::getCamera(String name)
{
	return this->mCameras[0];
}

void CameraManager::enableCamera(String name)
{
	this->mSceneManager->getSceneNode(name);
}

void CameraManager::setDefaultCamera()
{

}

String CameraManager::getDefaultCamera()
{
	return mDefaultCamera->getName();
}

void CameraManager::enableDetaultCamera()
{

}
/*****************/
#include "..\include\Graphics\LightsManager.h"

using namespace WyvernsAssault;

LightsManager::LightsManager(SceneManager* sceneManager)
{
	this->mSceneManager = sceneManager;
	mLight = NULL;
}

LightsManager::~LightsManager()
{
	finalize();
}

/** Initialize the lights manager */
void LightsManager::initialize()
{
	// Setting default ambient light value
	setAmbientLight(ColourValue(0.5, 0.5, 0.5));
	mLight = new Light();

}
/** Finalize the lights manager */
void LightsManager::finalize()
{
	mLight = NULL;
	mSceneManager->destroyAllLights();
}

/** Create lights functions **/		
void LightsManager::createLight(String name, Light::LightTypes type, ColourValue diffuse, ColourValue specular, Vector3 position, Vector3 direction)
{
	mLight = mSceneManager->createLight(name);
	mLight->setType(type);
	mLight->setDiffuseColour(diffuse);
	mLight->setSpecularColour(specular);
	if(position != Vector3::ZERO) mLight->setPosition(position);
	if(direction != Vector3::ZERO) mLight->setDirection(direction);
}

void LightsManager::setAmbientLight(ColourValue color)
{
	mSceneManager->setAmbientLight(color);
}
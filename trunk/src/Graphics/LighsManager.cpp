#include "..\include\Graphics\LightsManager.h"

using namespace WyvernsAssault;

LightsManager::LightsManager(SceneManager* sceneManager)
{
	this->mSceneManager = sceneManager;
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

	/** Temporal initialization **/

	/*createLight("Point1",		Light::LT_POINT,		ColourValue(0.5,0.5,0.2),	ColourValue(0.5,0.5,0.5),	Vector3(0, 300, 0));
	createLight("Directional1",	Light::LT_DIRECTIONAL,	ColourValue(0.5,0.1,0.1),	ColourValue(0.5,0.0,0.0),	Vector3::ZERO,			Vector3(1,0.5,0));
	createLight("Spot1",		Light::LT_SPOTLIGHT,	ColourValue(0.1,0.1,0.8),	ColourValue(0.0,0.0,0.8),	Vector3(200, 50, -20),		Vector3(0.5,-1,0));*/

	/*****************************/
}
/** Finalize the lights manager */
void LightsManager::finalize()
{
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
#include "..\include\Level\Level.h"

using namespace WyvernsAssault;

Level::Level(Ogre::String sceneFile, Ogre::String camerasFile, Ogre::String gameAreasFile)
{
	this->mSceneFile = sceneFile;
	this->mCamerasFile = camerasFile;
	this->mGameAreasFile = gameAreasFile;
}

Level::~Level()
{
	//
	// TODO Destructor
	//
}
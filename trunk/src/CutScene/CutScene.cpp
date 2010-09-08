#include "..\include\CutScene\CutScene.h"

using namespace WyvernsAssault;

CutScene::CutScene(int id, Ogre::String script)
: mPlayed(false)
{
	this->mId = id;
	this->mScript = script;
}

CutScene::~CutScene()
{
	//
	// TODO Destructor
	//
}
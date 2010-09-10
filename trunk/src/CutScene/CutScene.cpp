#include "..\include\CutScene\CutScene.h"

using namespace WyvernsAssault;

CutScene::CutScene(int level, int id, Ogre::String script)
: mPlayed(false)
{
	this->mLevel = level;
	this->mId = id;
	this->mScript = script;
}

CutScene::~CutScene()
{
	//
	// TODO Destructor
	//
}
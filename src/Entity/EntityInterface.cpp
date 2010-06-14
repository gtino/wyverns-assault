#include "..\..\..\include\Entity\EntityInterface.h"

using namespace WyvernsAssault;

EntityInterface::EntityInterface(Ogre::String name)
: mEntity(0)
, mSceneNode(0)
, mSceneManager(0)
, mAnimationState(0)
, mDirection(Ogre::Vector3::ZERO)
{
	mName = name;
}

EntityInterface::~EntityInterface()
{}

void EntityInterface::initializeEntity(Ogre::Entity* entity, Ogre::SceneNode* sceneNode, Ogre::SceneManager* sceneManager)
{
	// Make sure parameters are not null!
	assert(entity);
	assert(sceneManager);
	assert(sceneNode);

	// Make sure internal parameters are NULL, otherwise it means that we are calling 
	// initializeEntity on an entity that has already been initialized, and this should never happen!
	// If it happens, make sure you are not doing it by mistake. If done on purpouse, make sure you
	// call finalizeEntity before calling initializeEntity again!
	assert(!mEntity);
	assert(!mSceneManager);
	assert(!mSceneNode);

	// assign them to the internal 
	mEntity = entity;
	mSceneNode = sceneNode;
	mSceneManager = sceneManager;
}

void EntityInterface::finalizeEntity()
{
	// Assert if internal pointers are NOT null! This means that the 
	assert(mEntity);
	assert(mSceneManager);
	assert(mSceneNode);

	//
	// Here we recursively iterate thorug this sub branch and destroy EVERYTHING!
	//
	_destroy(mSceneNode);

	// Set internal pointers to NULL
	mEntity = NULL;
	mSceneNode = NULL;
	mSceneManager = NULL;
}

void EntityInterface::_destroy(SceneNode* sceneNode)
{
	Utils::Destroy(mSceneManager, sceneNode->getName());
}
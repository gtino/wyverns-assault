#include "..\..\include\Scenario\Object.h"

using namespace WyvernsAssault;

Object::Object(Ogre::String name, ObjectTypes type, Ogre::Vector3 physicBox, int objLife) 
: EntityInterface(name)
, PhysicsInterface()
, mOBBoxRenderable(0)
, mIsDebugEnabled(false)
, mMaxLife(100)
, mDieMesh(NULL)
, mDieAnimation(NULL)
{
	mType = type;
	life = objLife;
	mPhysicSize = physicBox; 

}


Object::~Object()
{
	finalizeEntity();
}

void Object::initializeEntity(Ogre::Entity* entity, Ogre::SceneNode* sceneNode, Ogre::SceneManager* sceneManager)
{
	// Always call base method before!
	EntityInterface::initializeEntity(entity, sceneNode, sceneManager);

	burning = false;

	if(mType == WyvernsAssault::ObjectTypes::DynamicObject && mPhysicSize > Vector3::ZERO)
	{
		initializePhysics(entity->getName(),mPhysicSize,"OBBoxManualMaterial_Object");
		sceneNode->attachObject(getGeometry(PhysicBoxType::body)->getMovableObject());
	}
}

void Object::finalizeEntity()
{
	// Always call base method before!
	EntityInterface::finalizeEntity();
}

void Object::updateEntity(const float elapsedSeconds)
{
	if( isDying() && hasDieAnimation() )
		mDieAnimation->addTime(elapsedSeconds);
}

bool Object::isHurt()
{
	return (life / mMaxLife * 100.0f) < 15.0f;
}

bool Object::isDying()
{
	return (life <= 0.0f);
}

void Object::hit(float damage)
{
	life -= damage;
}

// Die function, change visible meshes
void Object::dieSwitch()
{
	// Main mesh visibility to false
	setVisible(false);

	// Die mesh visible
	mDieMesh->setVisible(true);
}

// Set die mesh and set invisible
void Object::setDieMesh(Ogre::Entity* entity)
{ 
	mDieMesh = entity;
	mDieMesh->setVisible(false);
}

// Set die animation from skeleton
void Object::setDieAnimation(Ogre::AnimationState* dieAnimation)
{
	mDieAnimation = dieAnimation;
	mDieAnimation->setEnabled(true);
	mDieAnimation->setWeight(1);
	mDieAnimation->setLoop(false);
}

ObjectTypes Object::StringToType (Ogre::String string)
{
	const char* str = string.c_str();

	if(strcmp ( "Default", str ) == 0) return ObjectTypes::Default;

	return ObjectTypes::Default;
}

void Object::setDebugEnabled(bool isDebugEnabled)
{
	if(mIsDebugEnabled != isDebugEnabled)
	{
		mIsDebugEnabled = isDebugEnabled;
		if(mType == WyvernsAssault::ObjectTypes::DynamicObject)
			getGeometry(PhysicBoxType::body)->getMovableObject()->setVisible(mIsDebugEnabled);
	}
}

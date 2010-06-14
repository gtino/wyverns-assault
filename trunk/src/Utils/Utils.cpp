#include "..\include\Utils\Utils.h"

using namespace WyvernsAssault;

void Utils::Destroy(SceneManager* sceneManager, Ogre::String sceneNodeName)
{
	if(!sceneManager->hasSceneNode(sceneNodeName))
		return;

	SceneNode* sceneNode = sceneManager->getSceneNode(sceneNodeName);

	SceneNode::ChildNodeIterator itn = sceneNode->getChildIterator();

	while(itn.hasMoreElements())
	{
		SceneNode* node = static_cast<SceneNode*>(itn.getNext());

		Destroy(sceneManager,node->getName());
	}

	SceneNode::ObjectIterator ite = sceneNode->getAttachedObjectIterator();
	while (ite.hasMoreElements())
	{
		Entity* entity = static_cast<Entity*>(ite.getNext());

		sceneNode->detachObject(entity->getName());

		if(sceneManager->hasEntity(entity->getName()))
			sceneManager->destroyEntity(entity->getName());
	}	  	

	SceneNode* parentNode = sceneNode->getParentSceneNode();
	
	if(parentNode)
		parentNode->removeAndDestroyChild(sceneNode->getName());
}
#include "..\..\include\Debug\Debug.h"

using namespace WyvernsAssault;
using namespace Ogre;

void Debug::Out(Ogre::String text)
{
#ifdef OGRE_LOG_DEBUG_OUT
	LogManager::getSingleton().logMessage(text);
#else
	::OutputDebugString(text.c_str());
#endif
}

void Debug::Out(const char *text)
{
#ifdef OGRE_LOG_DEBUG_OUT
	LogManager::getSingleton().logMessage(text);
#else
	::OutputDebugString(text);
#endif
}

void Debug::Out(Ogre::SceneNode* sceneNode)
{
	Debug::Out(sceneNode,0);
}

void Debug::Out(Ogre::SceneNode* sceneNode, int level)
{
	if(!sceneNode)
		return;

	Ogre::String nodeStr;

	for(int i=0;i<level;i++)
		nodeStr += "\t";

	Debug::Out( nodeStr + "[N] " + sceneNode->getName());

	SceneNode::ChildNodeIterator itn = sceneNode->getChildIterator();

	while(itn.hasMoreElements())
	{
		SceneNode* node = static_cast<SceneNode*>(itn.getNext());

		Debug::Out(node,level+1);
	}

	Ogre::String baseEntityStr;

	for(int i=0;i<level+1;i++)
		baseEntityStr += "\t";

	SceneNode::ObjectIterator ite = sceneNode->getAttachedObjectIterator();
	while (ite.hasMoreElements())
	{
		Entity* entity = static_cast<Entity*>(ite.getNext());
		// do whatever there is to do with the entity...
		Debug::Out(baseEntityStr + "[E] " + entity->getName());
	}	  	
}
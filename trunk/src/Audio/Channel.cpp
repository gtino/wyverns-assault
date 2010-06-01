#include "..\..\include\Audio\Channel.h"

using namespace WyvernsAssault;

void ChannelInstance::clear(void)
{
	sceneNode = NULL;
	prevPosition = Ogre::Vector3(0, 0, 0);
}
#include "..\..\include\Audio\Channel.h"

using namespace WyvernsAssault;

void ChannelInstance::clear(void)
{
	position = Ogre::Vector3::ZERO;
	prevPosition = Ogre::Vector3::ZERO;
}
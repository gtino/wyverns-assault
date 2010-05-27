/*
TecnoFreak Animation System
http://sourceforge.net/projects/tecnofreakanima/

Copyright (c) 2008 Pau Novau Lebrato

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

    3. This notice may not be removed or altered from any source
    distribution.
*/

#include "Ogre/AnimationSystem.h"
#include "Ogre/Animation.h"

#include "Property/Xml/PropertyBuilder.h"
#include "Property/Property.h"

#include <Ogre.h>

#include <cassert>

using namespace tecnofreak;
using namespace tecnofreak::ogre;


AnimationSystem::AnimationSystem( Ogre::Entity* entity )
: ExampleAnimationSystem()
, m_animations( NULL )
{
	assert( entity );

	m_animations = entity->getAllAnimationStates();

	bool entityIsAnimated = ( entity->getAllAnimationStates() != NULL );
	assert( entityIsAnimated );

	Ogre::Skeleton* skeleton = entity->getSkeleton();
	skeleton->setBlendMode( Ogre::ANIMBLEND_CUMULATIVE );
}

AnimationSystem::AnimationSystem( Ogre::AnimationStateSet* animationStateSet )
: ExampleAnimationSystem()
, m_animations( animationStateSet )
{
	assert( getAnimationStateSet() );
}


AnimationSystem::~AnimationSystem()
{
}


void AnimationSystem::loadAnimationTree( const std::string& animationTreeOgreResourceName, const std::string& ogreResourceResourceGroupName )
{
	using namespace tecnofreak::property;

	Ogre::DataStreamPtr dataStream = Ogre::ResourceGroupManager::getSingleton().openResource( animationTreeOgreResourceName, ogreResourceResourceGroupName );

	std::string animationTreeData( dataStream->getAsString() );

	Property animationTree;
	xml::PropertyBuilder::initialiseFromData( &animationTree, animationTreeData );

	loadAnimationTree( &animationTree );
}


void AnimationSystem::loadAnimationInfo( const std::string& animationInfoOgreResourceName, const std::string& ogreResourceResourceGroupName )
{
	using namespace tecnofreak::property;

	Ogre::DataStreamPtr dataStream = Ogre::ResourceGroupManager::getSingleton().openResource( animationInfoOgreResourceName, ogreResourceResourceGroupName );
	
	std::string animationInfoData( dataStream->getAsString() );

	Property animationInfo;
	xml::PropertyBuilder::initialiseFromData( &animationInfo, animationInfoData );

	loadAnimationInfo( &animationInfo );
}


void AnimationSystem::createAnimations()
{
	Ogre::AnimationStateSet* animationStates = getAnimationStateSet();

	// Although there is an assert for this condition in the constructor, added check for 
	// safety in a release build.
	if ( animationStates == NULL )
	{
		return;
	}

	Ogre::AnimationStateIterator it = animationStates->getAnimationStateIterator();
	while ( it.hasMoreElements() )
	{
		Ogre::AnimationState* animationState = it.getNext();

		if ( 0 < animationState->getLength() )
		{
			Animation* animation = new Animation( animationState );
			addAnimation( animation );
		}
	}
}


Ogre::AnimationStateSet* AnimationSystem::getAnimationStateSet()
{
	return m_animations;
}

const Ogre::AnimationStateSet* AnimationSystem::getAnimationStateSet() const
{
	return m_animations;
}
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

#include "Ogre/Animation.h"

#include <cassert>

#include <Ogre.h>

using namespace tecnofreak;
using namespace tecnofreak::ogre;

Animation::Animation( Ogre::AnimationState* animationState )
: ExampleAnimation()
, m_animationState( animationState )
, m_weight( 1 )
, m_speed( 1 )
{
	assert( getAnimationState() );
	getAnimationState()->setWeight( getWeight() );
}

Animation::~Animation()
{
}

const std::string& Animation::getName() const
{
	return getAnimationState()->getAnimationName();
}

bool Animation::getEnabled() const
{
	return getAnimationState()->getEnabled();
}

void Animation::setEnabled( const bool enabled )
{
	getAnimationState()->setEnabled( enabled );
}

float Animation::getWeight() const
{
	return m_weight;
}

void Animation::setWeight( const float weight )
{
	m_weight = weight;

	getAnimationState()->setWeight( weight );
	
	// Enable or disable animation based on weight.
	bool validWeight = ( 0.0001 < weight );
	setEnabled( validWeight );
}

float Animation::getLength() const
{
	return getAnimationState()->getLength();
}

bool Animation::getLoop() const
{
	return getAnimationState()->getLoop();
}

void Animation::setLoop( const bool loop )
{
	getAnimationState()->setLoop( loop );
}


float Animation::getSpeed() const
{
	return m_speed;
}

void Animation::setSpeed( const float speed )
{
	m_speed = speed;
}

void Animation::reset()
{
	getAnimationState()->setTimePosition( 0 );
}

void Animation::update( const float elapsedSeconds )
{
	float currentRelativeTime = getRelativeTimePosition();

	getAnimationState()->addTime( elapsedSeconds * getSpeed() );

	float nextRelativeTime = getRelativeTimePosition();

	updateTriggers( currentRelativeTime, nextRelativeTime );
}

float Animation::getRelativeTimePosition() const
{
	return ( getTimePosition() / getLength() );
}

float Animation::getTimePosition() const
{
	return getAnimationState()->getTimePosition();
}

Ogre::AnimationState* Animation::getAnimationState()
{
	return m_animationState;
}

const Ogre::AnimationState* Animation::getAnimationState() const
{
	return m_animationState;
}

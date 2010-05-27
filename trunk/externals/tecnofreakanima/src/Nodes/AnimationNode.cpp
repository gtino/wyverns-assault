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

#include "Nodes/AnimationNode.h"

#include "Property/Property.h"

#include <cassert>

using namespace tecnofreak;

const int AnimationNode::INVALID_ANIMATION_ID( -1 );

AnimationNode::AnimationNode()
: Node()
, m_animationId( INVALID_ANIMATION_ID )
, m_speedPort( NULL )
, m_animation( NULL )
{
	m_speedPort = createPort( "speed" );
	getSpeedPort()->setDefaultValue( 1 );
}

AnimationNode::~AnimationNode()
{
}

void AnimationNode::evaluateAnimations( AnimationNodeList& reachedAnimationNodes )
{
	if ( ! hasValidAnimation() )
	{
		return;
	}

	reachedAnimationNodes.push_back( this );
}

void AnimationNode::setAnimationName( const std::string& animationName )
{
	m_animationName = animationName;
}

const std::string& AnimationNode::getAnimationName() const
{
	return m_animationName;
}

void AnimationNode::setAnimationId( const int animationId )
{
	m_animationId = animationId;
}

int AnimationNode::getAnimationId() const
{
	return m_animationId;
}

bool AnimationNode::hasValidAnimation() const
{
	return ( getAnimationId() != INVALID_ANIMATION_ID );
}

float AnimationNode::evaluateAnimationSpeed()
{
	float animationSpeed = getSpeedPort()->evaluateValue();
	return animationSpeed;
}

IPort* AnimationNode::getSpeedPort()
{
	return m_speedPort;
}

const IPort* AnimationNode::getSpeedPort() const
{
	return m_speedPort;
}

IAnimation* AnimationNode::getAnimation()
{
	return m_animation;
}

const IAnimation* AnimationNode::getAnimation() const
{
	return m_animation;
}

void AnimationNode::setAnimation( IAnimation* animation )
{
	assert( animation != NULL );

	m_animation = animation;
}

void AnimationNode::init( const tecnofreak::property::Property* description )
{
	if ( description == NULL )
	{
		return;
	}

	if ( ! description->hasChild( "animation" ) )
	{
		return;
	}

	std::string animationName = description->getChild( "animation" )->getValue();
	setAnimationName( animationName );
}
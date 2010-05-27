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

#include "Nodes/TransitionSelectNode.h"
#include "Nodes/AnimationNode.h"
#include "Helper/Clamp.h"

#include <cassert>
#include <sstream>

#include "Property/Property.h"

using namespace tecnofreak;

TransitionSelectNode::TransitionSelectNode()
: Node()
, m_selectedAnimationIdPort( NULL )
, m_timeInPort( NULL )
, m_timeOutPort( NULL )
, m_selectedAnimationId( -1 )
{
	m_selectedAnimationIdPort = createPort( "select" );
	m_timeInPort = createPort( "time in" );
	m_timeOutPort = createPort( "time out" );
}

TransitionSelectNode::~TransitionSelectNode()
{
}

int TransitionSelectNode::getSelectedAnimationId() const
{
	return m_selectedAnimationId;
}

void TransitionSelectNode::setSelectedAnimationId( const int selectedAnimationId, const float minTransitionSeconds )
{
	assert( 0 <= selectedAnimationId );
	assert( selectedAnimationId < getAnimationPortCount() );

	if ( selectedAnimationId == getSelectedAnimationId() )
	{
		return;
	}

	float timeIn = std::max< float >( getTimeInPort()->evaluateValue(), minTransitionSeconds );
	float timeOut = std::max< float >( getTimeOutPort()->evaluateValue(), minTransitionSeconds );

	// Mark all current active animations to transition out.
	for ( int i = 0; i < getAnimationPortCount(); i++ )
	{
		TransitionInfo& info = m_transtionInfoList[ i ];

		info.transitionIn = false;
		info.transitionOut = false;
		info.remainingSeconds = 0;

		bool animationIsActive = ( 0.0001f < info.weight );
		if ( animationIsActive )
		{
			info.transitionOut = true;
			info.remainingSeconds = timeOut;
		}
	}

	// Mark new selected animation to transition in.
	m_transtionInfoList[ selectedAnimationId ].transitionIn = true;
	m_transtionInfoList[ selectedAnimationId ].remainingSeconds = timeIn;

	m_selectedAnimationId = selectedAnimationId;
}

void TransitionSelectNode::updateAnimations( const float elapsedSeconds, AnimationNodeList& reachedAnimationNodes, bool selectedAnimationIdChanged )
{
	for ( int i = 0; i < getAnimationPortCount(); i++ )
	{
		TransitionInfo& info = m_transtionInfoList[ i ];

		bool animationInTransition = ( info.transitionIn || info.transitionOut );
		if ( animationInTransition )
		{
			
			float desiredWeight = ( info.transitionIn )? 1.f : 0.f;

			if ( info.remainingSeconds - elapsedSeconds <= 0 )
			{
				// The transition has finished.
				info.remainingSeconds = 0;
				info.weight = desiredWeight;
				info.transitionIn = false;
				info.transitionOut = false;
			}
			else
			{
				
				float remainingWeight = desiredWeight - info.weight;
				float weightIncreasePerSecond = remainingWeight / info.remainingSeconds;

				info.weight += weightIncreasePerSecond * elapsedSeconds;
				info.remainingSeconds -= elapsedSeconds;
			}

			float weight = info.weight * getWeight();
			if ( selectedAnimationIdChanged && i == getSelectedAnimationId() )
			{
				// The animations entering the transition must be reset so they start at
				// time position zero.

				AnimationNodeList animationsToReset;
				getAnimationPort( i )->evaluateAnimations( weight, animationsToReset );
				
				for ( AnimationNodeList::iterator it = animationsToReset.begin(); it != animationsToReset.end(); it++ )
				{
					AnimationNode* animationNode = (*it);
					bool animationEnabled = animationNode->getAnimation()->getEnabled();
					if ( ! animationEnabled )
					{
						animationNode->getAnimation()->reset();
					}
					reachedAnimationNodes.push_back( animationNode );
				}
			}
			else
			{
				getAnimationPort( i )->evaluateAnimations( weight, reachedAnimationNodes );
			}
		}
		else
		{
			// Animation that has finished a transition:
			// it will be either at full weight or at zero weight.

			bool animationHasWeight = ( 0.1f < info.weight ); // only checking if not zero here.
			if ( animationHasWeight )
			{
				float weight = getWeight() * info.weight;
				getAnimationPort( i )->evaluateAnimations( weight, reachedAnimationNodes );
			}
		}
	}
}

void TransitionSelectNode::evaluateAnimations( AnimationNodeList& reachedAnimationNodes )
{
	const float elapsedSeconds = getElapsedSeconds();

	int selectedAnimationId = int( getSelectedAnimationIdPort()->evaluateValue() );
	selectedAnimationId = helper::clamp( selectedAnimationId, 0, getAnimationPortCount() - 1 );

	bool selectedAnimationIdChanged = ( selectedAnimationId != getSelectedAnimationId() );
	if ( selectedAnimationIdChanged )
	{
		float minTransitionSeconds = elapsedSeconds * 0.1f;
		setSelectedAnimationId( selectedAnimationId, minTransitionSeconds );
	}

	updateAnimations( elapsedSeconds, reachedAnimationNodes, selectedAnimationIdChanged );
}

IPort* TransitionSelectNode::getSelectedAnimationIdPort()
{
	return m_selectedAnimationIdPort;
}

const IPort* TransitionSelectNode::getSelectedAnimationIdPort() const
{
	return m_selectedAnimationIdPort;
}

IPort* TransitionSelectNode::getTimeInPort()
{
	return m_timeInPort;
}

const IPort* TransitionSelectNode::getTimeInPort() const
{
	return m_timeInPort;
}

IPort* TransitionSelectNode::getTimeOutPort()
{
	return m_timeOutPort;
}

const IPort* TransitionSelectNode::getTimeOutPort() const
{
	return m_timeOutPort;
}

IPort* TransitionSelectNode::getAnimationPort( const int animationPortId )
{
	assert( 0 <= animationPortId );
	assert( animationPortId < getAnimationPortCount() );

	return m_animationPortList[ animationPortId ];
}

const IPort* TransitionSelectNode::getAnimationPort( const int animationPortId ) const
{
	assert( 0 <= animationPortId );
	assert( animationPortId < getAnimationPortCount() );

	return m_animationPortList[ animationPortId ];
}

int TransitionSelectNode::getAnimationPortCount() const
{
	return int( m_animationPortList.size() );
}

void TransitionSelectNode::createAnimationPorts( const int animationPortCount )
{
	assert( 0 < animationPortCount );
	if ( animationPortCount <= 0 )
	{
		// TODO: throw exception?
		return;
	}

	assert( m_animationPortList.size() == 0 );
	if ( m_animationPortList.size() != 0 )
	{
		// TODO: throw exception?
		return;
	}

	m_animationPortList.resize( animationPortCount );
	m_transtionInfoList.resize( animationPortCount );
	for ( unsigned int i = 0; i < m_animationPortList.size(); i++ )
	{
		std::stringstream portName;
		portName << "animation " << i;
		m_animationPortList[ i ] = createPort( portName.str() );

		m_transtionInfoList[ i ].transitionIn = false;
		m_transtionInfoList[ i ].transitionOut = false;
		m_transtionInfoList[ i ].weight = 0;
		m_transtionInfoList[ i ].remainingSeconds = 0;
	}

	m_transtionInfoList[ 0 ].weight = 1;
}

void TransitionSelectNode::init( const tecnofreak::property::Property* description )
{
	if ( description == NULL )
	{
		return;
	}

	if ( m_animationPortList.size() != 0 )
	{
		return;
	}

	if ( ! description->hasChild( "size" ) )
	{
		return;
	}

	int nodeSize = description->getChild( "size" )->getValueAsInteger();

	createAnimationPorts( nodeSize );
}
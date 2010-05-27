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

#include "Nodes/MixNode.h"

#include <sstream>
#include <cassert>

#include "Property/Property.h"

using namespace tecnofreak;

MixNode::MixNode()
: Node()
{
}

MixNode::~MixNode()
{
}

void MixNode::evaluateAnimations( AnimationNodeList& reachedAnimationNodes )
{
	float portWeight = getWeight();
	for ( unsigned int i = 0; i < m_animationPortList.size(); i++ )
	{
		IPort* currentPort = m_animationPortList[ i ];
		currentPort->evaluateAnimations( portWeight, reachedAnimationNodes );
	}
}

void MixNode::createAnimationPorts( const int animationPortCount )
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
	for ( unsigned int i = 0; i < m_animationPortList.size(); i++ )
	{
		std::stringstream portName;
		portName << "in" << i;
		m_animationPortList[ i ] = createPort( portName.str() );
	}
}


void MixNode::init( const property::Property* description )
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
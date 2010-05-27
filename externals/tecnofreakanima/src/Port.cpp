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

#include "Port.h"
#include "INode.h"

using namespace tecnofreak;

Port::Port( INode* parentNode, int id, const std::string& name )
: m_parentNode( parentNode )
, m_id( id )
, m_name( name )
, m_defaultValue( 0 )
, m_connectedNode( NULL )
{
}

Port::~Port()
{
}

INode* Port::getConnectedNode()
{
	return m_connectedNode;
}

const INode* Port::getConnectedNode() const
{
	return m_connectedNode;
}

void Port::setConnectedNode( INode* connectedNode )
{
	m_connectedNode = connectedNode;
}
	
bool Port::isConnected() const
{
	return ( getConnectedNode() != NULL );
}

INode* Port::getParentNode()
{
	return m_parentNode;
}

const INode* Port::getParentNode() const
{
	return m_parentNode;
}

int Port::getId() const
{
	return m_id;
}

const std::string& Port::getName() const
{
	return m_name;
}

void Port::evaluateAnimations( const float weight, AnimationNodeList& reachedAnimationNodes )
{
	if ( ! isConnected() )
	{
		return;
	}

	getConnectedNode()->setWeight( weight );
	getConnectedNode()->evaluateAnimations( reachedAnimationNodes );
}

float Port::evaluateValue()
{
	if ( ! isConnected() )
	{
		return getDefaultValue();
	}

	return getConnectedNode()->evaluateValue();
}

void Port::setDefaultValue( const float defaultValue )
{
	m_defaultValue = defaultValue;
}

float Port::getDefaultValue() const
{
	return m_defaultValue;
}
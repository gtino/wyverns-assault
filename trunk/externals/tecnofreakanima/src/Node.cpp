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

#include "Node.h"
#include "Port.h"
#include "SharedData.h"

#include <cassert>

using namespace tecnofreak;

const int Node::INVALID_PORT_ID( -1 );

Node::Node()
: m_weight( 0 )
, m_sharedData( NULL )
{
}

Node::~Node()
{
	for ( unsigned int i = 0; i < m_ports.size(); i++ )
	{
		assert( m_ports[ i ] != NULL );

		delete m_ports[ i ];
		m_ports[ i ] = NULL;
	}
}

float Node::getWeight() const
{
	return m_weight;
}

void Node::setWeight( const float weight )
{
	m_weight = weight;
}


void Node::evaluateAnimations( AnimationNodeList& reachedAnimationNodes )
{
	// Default action -> Do nothing.
	// This function should be overriden by subclasses.
}

float Node::evaluateValue()
{
	// Default action -> Return a default value.
	// This function should be overriden by subclasses.

	return 0;
}

float Node::getElapsedSeconds() const
{
	return getSharedData()->getElapsedSeconds();
}

int Node::getPortCount() const
{
	return int( m_ports.size() );
}

int Node::getPortId( const std::string& name ) const
{
	PortIdDictionary::const_iterator cit = m_portId.find( name );
	bool portIdFound = ( cit != m_portId.end() );

	if ( ! portIdFound )
	{
		return INVALID_PORT_ID;
	}

	int portId = cit->second;
	return portId;
}

bool Node::hasPort( const std::string& name ) const
{
	int portId = getPortId( name );
	bool portExists = isValidPortId( portId );

	return portExists;
}

bool Node::isValidPortId( const int id ) const
{
	return ( 0 <= id && size_t( id ) < m_ports.size() );
}

IPort* Node::getPort( const int id )
{
	assert( isValidPortId( id ) );
	// TODO: Throw exception if id isn't valid?

	return m_ports[ id ];
}

const IPort* Node::getPort( const int id ) const
{
	assert( isValidPortId( id ) );
	// TODO: Throw exception if id isn't valid?

	return m_ports[ id ];
}

IPort* Node::getPort( const std::string& name )
{
	int portId = getPortId( name );
	return getPort( portId );
}

const IPort* Node::getPort( const std::string& name ) const
{
	int portId = getPortId( name );
	return getPort( portId );
}

IPort* Node::createPort( const std::string& name )
{
	bool portWithThisNameExists = hasPort( name );
	assert( ! portWithThisNameExists );
	if ( portWithThisNameExists )
	{
		// throw exception?
		return NULL;
	}

	int portId = int( m_ports.size() );
	Port* port = new Port( this, portId, name );
	m_ports.push_back( port );

	m_portId[ name ] = portId;

	// TODO: move this to a unit test?
	assert( getPort( port->getId() ) == port );

	return port;
}

void Node::init( const property::Property* description )
{
}

void Node::setSharedData( SharedData* sharedData )
{
	m_sharedData = sharedData;
}

SharedData* Node::getSharedData()
{
	return m_sharedData;
}

const SharedData* Node::getSharedData() const
{
	return m_sharedData;
}

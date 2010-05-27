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

#include "Nodes/ParameterNode.h"

#include "Property/Property.h"

#include <cassert>

using namespace tecnofreak;

ParameterNode::ParameterNode()
: Node()
, m_parameter( NULL )
{
}

ParameterNode::~ParameterNode()
{
}

void ParameterNode::setParameterName( const std::string& parameterName )
{
	m_parameterName = parameterName;
}

const std::string& ParameterNode::getParameterName() const
{
	return m_parameterName;
}

void ParameterNode::setParameter( IParameter* parameter )
{
	assert( parameter );

	m_parameter = parameter;
}

IParameter* ParameterNode::getParameter()
{
	return m_parameter;
}

const IParameter* ParameterNode::getParameter() const
{
	return m_parameter;
}

float ParameterNode::evaluateValue()
{
	assert( getParameter() );
	if ( ! getParameter() )
	{
		// should cause exception..? probably it should...
		return 0;
	}

	return getParameter()->getFloatValue();
}

void ParameterNode::init( const tecnofreak::property::Property* description )
{
	if ( description == NULL )
	{
		return;
	}

	if ( ! description->hasChild( "parameter" ) )
	{
		return;
	}

	std::string parameterName = description->getChild( "parameter" )->getValue();
	setParameterName( parameterName );
}

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

#include "Nodes/SmoothValueNode.h"
#include "Property/Property.h"

#include "Helper/SmoothValue.h"

using namespace tecnofreak;

SmoothValueNode::SmoothValueNode()
: m_desiredValuePort( NULL )
, m_smoothTimePort( NULL )
, m_currentValue( 0 )
, m_velocity( 0 )
{
	m_desiredValuePort = createPort( "value" );
	m_smoothTimePort = createPort( "smooth_time" );
}

SmoothValueNode::~SmoothValueNode()
{

}

float SmoothValueNode::evaluateValue()
{
	float desiredValue = getDesiredValue();
	float smoothTime = getSmoothTime();
	float elapsedSeconds = getElapsedSeconds();

	m_currentValue = tecnofreak::helper::smoothValue( m_currentValue, desiredValue, m_velocity, smoothTime, elapsedSeconds );

	return m_currentValue;
}

IPort* SmoothValueNode::getDesiredValuePort()
{
	return m_desiredValuePort;
}

const IPort* SmoothValueNode::getDesiredValuePort() const
{
	return m_desiredValuePort;
}

IPort* SmoothValueNode::getSmoothTimePort()
{
	return m_smoothTimePort;
}
const IPort* SmoothValueNode::getSmoothTimePort() const
{
	return m_smoothTimePort;
}

float SmoothValueNode::getDesiredValue() const
{
	return m_desiredValuePort->evaluateValue();
}

float SmoothValueNode::getCurrentValue() const
{
	return m_currentValue;
}

float SmoothValueNode::getSmoothTime() const
{
	return m_smoothTimePort->evaluateValue();
}

void SmoothValueNode::init( const tecnofreak::property::Property* description )
{
	if ( description == NULL )
	{
		return;
	}

	if ( ! description->hasChild( "initial_value" ) )
	{
		return;
	}

	m_currentValue = description->getChild( "initial_value" )->getValueAsFloat();
}


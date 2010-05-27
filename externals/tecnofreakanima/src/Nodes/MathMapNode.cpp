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

#include "Nodes/MathMapNode.h"
#include "Helper/Clamp.h"

#include <cmath>

using namespace tecnofreak;

MathMapNode::MathMapNode()
: Node()
, m_minValueInPort( NULL )
, m_maxValueInPort( NULL )
, m_minValueOutPort( NULL )
, m_maxValueOutPort( NULL )
, m_valueInPort( NULL )
{
	m_minValueInPort = createPort( "min_in" );
	m_maxValueInPort = createPort( "max_in" );
	m_minValueOutPort = createPort( "min_out" );
	m_maxValueOutPort = createPort( "max_out" );
	m_valueInPort = createPort( "value" );
}

MathMapNode::~MathMapNode()
{
}

IPort* MathMapNode::getMinValueInPort()
{
	return m_minValueInPort;
}

const IPort* MathMapNode::getMinValueInPort() const
{
	return m_minValueInPort;
}

IPort* MathMapNode::getMaxValueInPort()
{
	return m_maxValueInPort;
}

const IPort* MathMapNode::getMaxValueInPort() const
{
	return m_maxValueInPort;
}

IPort* MathMapNode::getMinValueOutPort()
{
	return m_minValueOutPort;
}

const IPort* MathMapNode::getMinValueOutPort() const
{
	return m_minValueOutPort;
}

IPort* MathMapNode::getMaxValueOutPort()
{
	return m_maxValueOutPort;
}

const IPort* MathMapNode::getMaxValueOutPort() const
{
	return m_maxValueOutPort;
}

IPort* MathMapNode::getValueInPort()
{
	return m_valueInPort;
}

const IPort* MathMapNode::getValueInPort() const
{
	return m_valueInPort;
}

float MathMapNode::evaluateValue()
{
	float minValueIn = getMinValueInPort()->evaluateValue();
	float maxValueIn = getMaxValueInPort()->evaluateValue();
	float inRange = std::abs( maxValueIn - minValueIn );

	if ( inRange == 0 )
	{
		return 0;
	}

	float minValueOut = getMinValueOutPort()->evaluateValue();
	float maxValueOut = getMaxValueOutPort()->evaluateValue();
	float outRange = std::abs( maxValueOut - minValueOut );

	float valueIn = getValueInPort()->evaluateValue();
	valueIn = helper::clamp( valueIn, minValueIn, maxValueIn );

	
	float normalizedValueIn = ( ( valueIn - minValueIn ) / inRange );
	float valueOut = minValueOut + ( normalizedValueIn * outRange );

	return valueOut;
}
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

#include "Nodes/MathClampNode.h"
#include "Helper/Clamp.h"

using namespace tecnofreak;

MathClampNode::MathClampNode()
: Node()
, m_minValuePort( NULL )
, m_maxValuePort( NULL )
, m_valuePort( NULL )
{
	m_minValuePort = createPort( "min" );
	m_maxValuePort = createPort( "max" );
	m_valuePort = createPort( "value" );
}

MathClampNode::~MathClampNode()
{
}

IPort* MathClampNode::getMinValuePort()
{
	return m_minValuePort;
}

const IPort* MathClampNode::getMinValuePort() const
{
	return m_minValuePort;
}

IPort* MathClampNode::getMaxValuePort()
{
	return m_maxValuePort;
}

const IPort* MathClampNode::getMaxValuePort() const
{
	return m_maxValuePort;
}

IPort* MathClampNode::getValuePort()
{
	return m_valuePort;
}

const IPort* MathClampNode::getValuePort() const
{
	return m_valuePort;
}

float MathClampNode::evaluateValue()
{
	float minValue = getMinValuePort()->evaluateValue();
	float maxValue = getMaxValuePort()->evaluateValue();
	float value = getValuePort()->evaluateValue();
	
	return helper::clamp( value, minValue, maxValue );
}
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

#include "Nodes/MathDivNode.h"

#include <cassert>

using namespace tecnofreak;

MathDivNode::MathDivNode()
: Node()
, m_dividendPort( NULL )
, m_divisorPort( NULL )
{
	m_dividendPort = createPort( "in0" );
	m_divisorPort = createPort( "in1" );
}

MathDivNode::~MathDivNode()
{
}

IPort* MathDivNode::getDividendPort()
{
	return m_dividendPort;
}

const IPort* MathDivNode::getDividendPort() const
{
	return m_dividendPort;
}

IPort* MathDivNode::getDivisorPort()
{
	return m_divisorPort;
}

const IPort* MathDivNode::getDivisorPort() const
{
	return m_divisorPort;
}

float MathDivNode::evaluateValue()
{
	float dividend = getDividendPort()->evaluateValue();
	float divisor = getDivisorPort()->evaluateValue();

	assert( divisor != 0 );
	if ( divisor == 0 )
	{
		// Should probably throw an exception here...
		return 0;
	}

	float quotient = dividend / divisor;
	return quotient;
}


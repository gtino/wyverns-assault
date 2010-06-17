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


#ifndef __TECNOFREAK__MATH_MINUS_NODE__H__
#define __TECNOFREAK__MATH_MINUS_NODE__H__

#include "Node.h"
#include "IPort.h"

namespace tecnofreak
{

class TecnoFreakDllPrivate MathMinusNode : public Node
{
public:
	MathMinusNode();
	virtual ~MathMinusNode();

	IPort* getAPort();
	const IPort* getAPort() const;

	IPort* getBPort();
	const IPort* getBPort() const;

	virtual float evaluateValue();
private:
	IPort* m_aPort;
	IPort* m_bPort;
};

}

#endif
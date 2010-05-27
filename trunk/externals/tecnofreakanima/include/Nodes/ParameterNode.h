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

#ifndef __TECNOFREAK__PARAMETER_NODE__H__
#define __TECNOFREAK__PARAMETER_NODE__H__

#include "Node.h"
#include "IParameter.h"

#include <string>


namespace tecnofreak
{

class TecnoFreakDllPrivate ParameterNode
	: public Node
{
public:

	ParameterNode();
	virtual ~ParameterNode();

	void setParameterName( const std::string& parameterName );
	const std::string& getParameterName() const;

	void setParameter( IParameter* parameter );
	IParameter* getParameter();
	const IParameter* getParameter() const;

	virtual float evaluateValue();

	virtual void init( const tecnofreak::property::Property* description );
private:
	std::string m_parameterName;
	IParameter* m_parameter;
};

}

#endif

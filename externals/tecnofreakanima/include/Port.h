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

#ifndef __TECNOFREAK__PORT__H__
#define __TECNOFREAK__PORT__H__

#include "Common.h"

#include "IPort.h"

namespace tecnofreak
{

class TecnoFreakDllExport Port
	: public IPort
{
public:
	Port( INode* parentNode, const int id, const std::string& name );
	virtual ~Port();

	virtual INode* getConnectedNode();
	virtual const INode* getConnectedNode() const;
	
	virtual bool isConnected() const;

	virtual INode* getParentNode();
	virtual const INode* getParentNode() const;

	virtual int getId() const;

	virtual const std::string& getName() const;

	virtual void evaluateAnimations( const float weight, AnimationNodeList& reachedAnimationNodes );
	virtual float evaluateValue();

	virtual void setConnectedNode( INode* connectedNode );

	virtual void setDefaultValue( const float defaultValue );
	virtual float getDefaultValue() const;

private:
	INode* m_parentNode;
	int m_id;
	std::string m_name;

	float m_defaultValue;

	INode* m_connectedNode;
};

}

#endif
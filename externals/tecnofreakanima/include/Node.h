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

#ifndef __TECNOFREAK__NODE__H__
#define __TECNOFREAK__NODE__H__ 

#include "Common.h"
#include "INode.h"
#include "SharedData.h"

#include <vector>
#include <map>

namespace tecnofreak
{


class TecnoFreakDllExport Node
	: public INode
{
public:

	static const int INVALID_PORT_ID;

	Node();
	virtual ~Node();

	virtual float getWeight() const;
	virtual void setWeight( const float weight );

	virtual void evaluateAnimations( AnimationNodeList& reachedAnimationNodes );
	virtual float evaluateValue();

	virtual float getElapsedSeconds() const;

	virtual int getPortCount() const;

	virtual int getPortId( const std::string& name ) const;
	virtual bool hasPort( const std::string& name ) const;
	virtual bool isValidPortId( const int id ) const;
	virtual IPort* getPort( const int id );
	virtual const IPort* getPort( const int id ) const;
	virtual IPort* getPort( const std::string& name );
	virtual const IPort* getPort( const std::string& name ) const;

	virtual void init( const tecnofreak::property::Property* description );

	virtual void setSharedData( SharedData* sharedData );
	virtual SharedData* getSharedData();
	virtual const SharedData* getSharedData() const;

//protected:
	IPort* createPort( const std::string& name );

private:
	float m_weight;

	typedef std::vector< IPort* > PortVector;
	PortVector m_ports;

	typedef std::map< std::string, int > PortIdDictionary;
	PortIdDictionary m_portId;

	SharedData* m_sharedData;
};

}

#endif

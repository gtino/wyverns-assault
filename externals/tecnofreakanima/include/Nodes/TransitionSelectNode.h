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

#ifndef __TECNOFREAK__TRANSITION_SELECT_NODE__H__
#define __TECNOFREAK__TRANSITION_SELECT_NODE__H__

#include "Node.h"
#include "IPort.h"

#include <vector>


namespace tecnofreak
{

class TecnoFreakDllPrivate TransitionSelectNode
	: public Node
{
public:

	TransitionSelectNode();
	virtual ~TransitionSelectNode();

	virtual void evaluateAnimations( AnimationNodeList& reachedAnimationNodes );

	virtual void init( const tecnofreak::property::Property* description );
protected:

	IPort* getSelectedAnimationIdPort();
	const IPort* getSelectedAnimationIdPort() const;

	IPort* getTimeInPort();
	const IPort* getTimeInPort() const;

	IPort* getTimeOutPort();
	const IPort* getTimeOutPort() const;

	IPort* getAnimationPort( const int animationPortId );
	const IPort* getAnimationPort( const int animationPortId ) const;

	int getAnimationPortCount() const;

	void createAnimationPorts( const int animationPortCount );

	void updateAnimations( const float elapsedSeconds, AnimationNodeList& reachedAnimationNodes, bool selectedAnimationIdChanged );

	int getSelectedAnimationId() const;
	void setSelectedAnimationId( const int selectedAnimationId, const float minTransitionSeconds );

private:
	IPort* m_selectedAnimationIdPort;
	IPort* m_timeInPort;
	IPort* m_timeOutPort;
	std::vector< IPort* > m_animationPortList;

	int m_selectedAnimationId;

	typedef struct
	{
		bool transitionIn;
		bool transitionOut;
		float weight;
		float remainingSeconds;
	} TransitionInfo;
	std::vector< TransitionInfo > m_transtionInfoList;
};

}

#endif

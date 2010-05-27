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

#ifndef __TECNOFREAK__ANIMATION_NODE__H__
#define __TECNOFREAK__ANIMATION_NODE__H__

#include "Node.h"
#include "IPort.h"
#include "IAnimation.h"
#include <string>

namespace tecnofreak
{

class TecnoFreakDllPrivate AnimationNode
	: public Node
{
public:

	static const int INVALID_ANIMATION_ID;

	AnimationNode();
	virtual ~AnimationNode();

	virtual void evaluateAnimations( AnimationNodeList& reachedAnimationNodes );

	void setAnimationName( const std::string& animationName );
	const std::string& getAnimationName() const;

	void setAnimationId( const int animationId );
	int getAnimationId() const;

	IAnimation* getAnimation();
	const IAnimation* getAnimation() const;
	void setAnimation( IAnimation* animation );

	bool hasValidAnimation() const;

	float evaluateAnimationSpeed();

	IPort* getSpeedPort();
	const IPort* getSpeedPort() const;	

	virtual void init( const tecnofreak::property::Property* description );
private:
	int m_animationId;
	std::string m_animationName;

	IPort* m_speedPort;

	IAnimation* m_animation;
};

}

#endif

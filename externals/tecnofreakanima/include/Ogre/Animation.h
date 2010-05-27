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

#ifndef __TECNOFREAK__OGRE_ANIMATION__H__
#define __TECNOFREAK__OGRE_ANIMATION__H__

#include "Common.h"
#include "ExampleAnimation.h"

namespace Ogre
{
	class AnimationState;
}

namespace tecnofreak
{

namespace ogre
{

class TecnoFreakDllPrivate Animation
	: public ExampleAnimation
{
public:
	Animation( Ogre::AnimationState* animationState );
	virtual ~Animation();

	virtual const std::string& getName() const;

	virtual bool getEnabled() const;
	virtual void setEnabled( const bool enabled );

	virtual float getWeight() const;
	virtual void setWeight( const float weight );

	virtual float getLength() const;

	virtual bool getLoop() const;
	virtual void setLoop( const bool loop );

	virtual float getSpeed() const;
	virtual void setSpeed( const float speed );

	virtual void reset();

	virtual void update( const float elapsedSeconds );

	virtual float getRelativeTimePosition() const;
	virtual float getTimePosition() const;

	Ogre::AnimationState* getAnimationState();
	const Ogre::AnimationState* getAnimationState() const;

private:
	Ogre::AnimationState* m_animationState;
	
	// weight must be stored because many references to a single animation state may exist.
	float m_weight;
	float m_speed;

};

}

}

#endif
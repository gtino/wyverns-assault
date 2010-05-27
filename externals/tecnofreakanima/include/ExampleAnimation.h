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

#ifndef __TECNOFREAK__ABSTRACT_ANIMATION__H__
#define __TECNOFREAK__ABSTRACT_ANIMATION__H__

#include "IAnimation.h"

#include "AnimationTriggerSystem.h"

#include <list>
#include <vector>

namespace tecnofreak
{

class TecnoFreakDllPrivate ExampleAnimation
	: public IAnimation
{
public:
	ExampleAnimation();
	virtual ~ExampleAnimation();

	int getActiveReferencingNodesCount() const;
	void resetActiveReferencingNodesCount();
	void increaseActiveReferencingNodeCount();

	float getAccumulatedWeight() const;
	void resetAccumulatedWeight();
	void increaseAccumulatedWeight( const float weightIncrease );

	float getAccumulatedSpeed() const;
	void resetAccumulatedSpeed();
	void increaseAccumulatedSpeed( const float speedIncrease );

	float calculateRealWeight() const;
	float calculateRealSpeed() const;

	void updateTriggers( const float currentRelativeTime, const float nextRelativeTime );
	void clearTriggers();
	void addTrigger( Trigger* trigger );

	void addTriggerSubscriber( ITriggerSubscriber* subscriber );
	void addTriggerSubscriber( const std::string& triggerName, ITriggerSubscriber* subscriber );

	void removeTriggerSubscriber( ITriggerSubscriber* subscriber );
	void removeTriggerSubscriber( const std::string& triggerName );
	void removeTriggerSubscriber( const std::string& triggerName, ITriggerSubscriber* subscriber );

	void clearTriggerSubscribers();

private:

	int m_activeReferencingNodesCount;
	float m_accumulatedWeight;
	float m_accumulatedSpeed;

	AnimationTriggerSystem* m_triggerSystem;


};

typedef std::list< ExampleAnimation* > ExampleAnimationList;
typedef std::vector< ExampleAnimation* > ExampleAnimationVector;

}

#endif

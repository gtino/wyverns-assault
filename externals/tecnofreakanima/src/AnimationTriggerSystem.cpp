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

#include "AnimationTriggerSystem.h"

using namespace tecnofreak;

AnimationTriggerSystem::AnimationTriggerSystem()
{

}

AnimationTriggerSystem::~AnimationTriggerSystem()
{
	typedef TriggerContainer::iterator TriggerIterator;
	for ( TriggerIterator it = m_triggers.begin(); it != m_triggers.end(); it++ )
	{
		delete it->second;
	}
	m_triggers.clear();
}

void AnimationTriggerSystem::addTrigger( Trigger* trigger )
{
	m_triggers.insert( std::pair< float, Trigger* >( trigger->getRelativeTime(), trigger ) );
}

void AnimationTriggerSystem::addSubscriber( ITriggerSubscriber* subscriber )
{
	for ( TriggerContainer::iterator it = m_triggers.begin(); it != m_triggers.end(); it++ )
	{
		Trigger* trigger = it->second;
		trigger->addSubscriber( subscriber );
	}
}


void AnimationTriggerSystem::addSubscriber( const std::string& triggerName, ITriggerSubscriber* subscriber )
{
	for ( TriggerContainer::iterator it = m_triggers.begin(); it != m_triggers.end(); it++ )
	{
		Trigger* trigger = it->second;
		if ( trigger->getName() == triggerName )
		{
			trigger->addSubscriber( subscriber );
		}
	}
}


void AnimationTriggerSystem::removeSubscriber( ITriggerSubscriber* subscriber )
{
	for ( TriggerContainer::iterator it = m_triggers.begin(); it != m_triggers.end(); it++ )
	{
		Trigger* trigger = it->second;
		trigger->removeSubscriber( subscriber );
	}
}

void AnimationTriggerSystem::removeSubscriber( const std::string& triggerName )
{
	for ( TriggerContainer::iterator it = m_triggers.begin(); it != m_triggers.end(); it++ )
	{
		Trigger* trigger = it->second;
		if ( trigger->getName() == triggerName )
		{
			trigger->clearSubscribers();
		}
	}
}

void AnimationTriggerSystem::removeSubscriber( const std::string& triggerName, ITriggerSubscriber* subscriber )
{
	for ( TriggerContainer::iterator it = m_triggers.begin(); it != m_triggers.end(); it++ )
	{
		Trigger* trigger = it->second;
		if ( trigger->getName() == triggerName )
		{
			trigger->removeSubscriber( subscriber );
		}
	}
}

void AnimationTriggerSystem::removeAllSubscribers()
{
	for ( TriggerContainer::iterator it = m_triggers.begin(); it != m_triggers.end(); it++ )
	{
		Trigger* trigger = it->second;
		trigger->clearSubscribers();
	}
}

void AnimationTriggerSystem::update( const float currentRelativeTime, const float nextRelativeTime, const float currentWeight )
{
	if ( currentRelativeTime <= nextRelativeTime )
	{
		// normal case
		typedef TriggerContainer::const_iterator I;
		std::pair< I, I > triggers = m_triggers.equal_range( currentRelativeTime );
		for ( I it = triggers.first; it != m_triggers.end(); it++ )
		{
			const Trigger* currentTrigger = it->second;

			if ( nextRelativeTime < currentTrigger->getRelativeTime() )
			{
				return;
			}

			currentTrigger->notify( currentWeight );
		}
	}
	else
	{
		// the animation has looped.
		update( currentRelativeTime, 1, currentWeight );
		update( 0, nextRelativeTime, currentWeight );
	}
	
}
/*
-----------------------------------------------------------------------------
Wyverns' Assault 3D Videgame.
Copyright (C) 2010  Giorgio Tino, Javier Soto Huesa, Jordi Carreras Ribot, 
					Marc Serena, Elm Oliver Torres

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

-----------------------------------------------------------------------------
*/
#ifndef __EVENT_H_
#define __EVENT_H_

#include <boost/shared_ptr.hpp>

#include "..\Entity\Enemy\Enemy.h"
#include "..\Entity\Player\Player.h"
#include "..\Entity\Item\Item.h"

namespace WyvernsAssault
{
	/** Event types */
	enum EventTypes
	{
		PlayerHit = 0,
		PlayerKilled,
		PlayerRemove,
		PlayerAttack,
		PlayerAttackSpecial,
		EnemyAttack,
		EnemyHit,
		EnemyKilled,
		EnemyRemove,
		EnemyCustom,
		ObjectHit,
		Collision,		
		ItemCatch,
		ItemRemove
	};

	/** Event priority, used to put the event in the correct queue */
	enum EventPriorities
	{
		Highest = 0,
		High,
		Normal,
		Low,
		Lower
	};

	typedef float EventTimer;
	typedef void* EventData;

	class Event
	{
	public:
		Event(EventTypes type, EventPriorities priority);
		virtual ~Event() = 0;

	public:
		/** Returns the event type */
		EventTypes getType();
		/** Returns the event priority */
		EventPriorities getPriority();
		/** Returens the event data */
		EventData getData();
		/** Sets the event data */
		void setData(EventData data);

		// Set the internal timer
		void setTimer(const float timoutInSeconds);
		// Update internal timer. Internal timer will be decremented 
		// of given amount of time in seconds 
		void updateTimer(const float elapsedSeconds);
		// Return if timer has expired (<= 0)
		bool hasExpired();

	private:
		EventTypes mType;
		EventPriorities mPriority;
		EventData mData;
		EventTimer mTimer;
	};

	typedef boost::shared_ptr<Event> EventPtr;

	/** Functor class used to compare two event pointers */
	class EventComparator
	{
	public:
		/** Compare two events checking their priorities:
		    The event with the lower value will have higher priority */
		bool operator()(EventPtr& evt1, EventPtr& evt2);
	};

	// --------------------------------
	class CollisionEvent: public Event
	{
	public:
		CollisionEvent();
		~CollisionEvent(){};
	};

	typedef boost::shared_ptr<CollisionEvent> CollisionEventPtr;

	// --------------------------------	
	class EnemyAttackEvent : public Event
	{
	public:
		EnemyAttackEvent(EnemyPtr e);
		~EnemyAttackEvent(){};
		
		EnemyPtr getEnemy(){return mEnemy;}

	private:		
		EnemyPtr mEnemy;
	};

	typedef boost::shared_ptr<EnemyAttackEvent> EnemyAttackEventPtr;

	// --------------------------------
	class EnemyHitEvent : public Event
	{
	public:
		EnemyHitEvent(EnemyPtr e, PlayerPtr p);
		~EnemyHitEvent(){};
		
		EnemyPtr getEnemy(){return mEnemy;}
		PlayerPtr getPlayer(){return mPlayer;}
		
		void setDamage(Ogre::Real damage){mDamage = damage;}
		Ogre::Real getDamage(){return mDamage;}

	private:
		EnemyPtr mEnemy;
		PlayerPtr mPlayer;

		Ogre::Real mDamage;
	};

	typedef boost::shared_ptr<EnemyHitEvent> EnemyHitEventPtr;

	// --------------------------------
	class EnemyKilledEvent : public Event
	{
	public:
		EnemyKilledEvent(EnemyPtr e, PlayerPtr p);
		~EnemyKilledEvent(){};
		
		EnemyPtr getEnemy(){return mEnemy;}
		PlayerPtr getPlayer(){return mPlayer;}

	private:
		EnemyPtr mEnemy;
		PlayerPtr mPlayer;
	};

	typedef boost::shared_ptr<EnemyKilledEvent> EnemyKilledEventPtr;

	// --------------------------------
	class EnemyRemoveEvent : public Event
	{
	public:
		EnemyRemoveEvent(EnemyPtr e);
		~EnemyRemoveEvent(){};
		
		EnemyPtr getEnemy(){return mEnemy;}

	private:
		EnemyPtr mEnemy;
	};

	typedef boost::shared_ptr<EnemyRemoveEvent> EnemyRemoveEventPtr;

	// --------------------------------	
	class EnemyCustomEvent : public Event
	{
	public:
		EnemyCustomEvent(EnemyPtr e);
		~EnemyCustomEvent(){};
		
		EnemyPtr getEnemy(){return mEnemy;}

	private:		
		EnemyPtr mEnemy;
	};

	typedef boost::shared_ptr<EnemyCustomEvent> EnemyCustomEventPtr;

	// --------------------------------
	class PlayerHitEvent : public Event
	{
	public:
		PlayerHitEvent(EnemyPtr e, PlayerPtr p);
		~PlayerHitEvent(){};
		
		PlayerPtr getPlayer(){return mPlayer;}
		EnemyPtr getEnemy(){return mEnemy;}		

	private:		
		PlayerPtr mPlayer;
		EnemyPtr mEnemy;

		Ogre::Real damage;
	};

	typedef boost::shared_ptr<PlayerHitEvent> PlayerHitEventPtr;

	// --------------------------------

	class PlayerKilledEvent : public Event
	{
	public:
		PlayerKilledEvent(PlayerPtr p);
		~PlayerKilledEvent(){};
		
		PlayerPtr getPlayer(){return mPlayer;}

	private:		
		PlayerPtr mPlayer;

		Ogre::Real damage;
	};

	typedef boost::shared_ptr<PlayerKilledEvent> PlayerKilledEventPtr;

	// --------------------------------
	class PlayerAttackEvent : public Event
	{
	public:
		PlayerAttackEvent(PlayerPtr p);
		
		PlayerPtr getPlayer(){return mPlayer;}

	private:		
		PlayerPtr mPlayer;
	};

	typedef boost::shared_ptr<PlayerAttackEvent> PlayerAttackEventPtr;

	// --------------------------------
	class PlayerAttackSpecialEvent : public Event
	{
	public:
		PlayerAttackSpecialEvent(PlayerPtr p);
		~PlayerAttackSpecialEvent(){};
		
		PlayerPtr getPlayer(){return mPlayer;}

	private:		
		PlayerPtr mPlayer;
	};

	typedef boost::shared_ptr<PlayerAttackSpecialEvent> PlayerAttackSpecialEventPtr;

	// --------------------------------
	class ItemCatchEvent : public Event
	{
	public:
		ItemCatchEvent(PlayerPtr p, ItemPtr item);
		~ItemCatchEvent(){};
		
		PlayerPtr getPlayer(){return mPlayer;}
		ItemPtr getItem(){return mItem;}

	private:		
		PlayerPtr mPlayer;
		ItemPtr mItem;
	};

	typedef boost::shared_ptr<ItemCatchEvent> ItemCatchEventPtr;

	// --------------------------------
	class ItemRemoveEvent : public Event
	{
	public:
		ItemRemoveEvent(/*PlayerPtr p,*/ ItemPtr item);
		~ItemRemoveEvent(){};
		
		/*PlayerPtr getPlayer(){return mPlayer;}*/
		ItemPtr getItem(){return mItem;}

	private:		
		/*PlayerPtr mPlayer;*/
		ItemPtr mItem;
	};

	typedef boost::shared_ptr<ItemRemoveEvent> ItemRemoveEventPtr;
}

#endif // __EVENT_H_
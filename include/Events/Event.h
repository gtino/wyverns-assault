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
		EnemyHit,
		EnemySpecialHit,
		EnemyKill,
		ObjectHit,
		Collision,
		PlayerAttack,
		PlayerAttackSpecial,
		ItemCatch
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

	private:
		EventTypes mType;
		EventPriorities mPriority;
		EventData mData;
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
	class EnemyHitEvent : public Event
	{
	public:
		EnemyHitEvent(EnemyPtr e, PlayerPtr p);
		~EnemyHitEvent(){};
		
		EnemyPtr getEnemy(){return mEnemy;}
		PlayerPtr getPlayer(){return mPlayer;}

	private:
		EnemyPtr mEnemy;
		PlayerPtr mPlayer;

		Ogre::Real damage;
	};

	typedef boost::shared_ptr<EnemyHitEvent> EnemyHitEventPtr;

	// --------------------------------
	class EnemySpecialHitEvent : public Event
	{
	public:
		EnemySpecialHitEvent(EnemyPtr e, PlayerPtr p);
		~EnemySpecialHitEvent(){};
		
		EnemyPtr getEnemy(){return mEnemy;}
		PlayerPtr getPlayer(){return mPlayer;}

	private:
		EnemyPtr mEnemy;
		PlayerPtr mPlayer;

		Ogre::Real damage;
	};

	typedef boost::shared_ptr<EnemySpecialHitEvent> EnemySpecialHitEventPtr;

	// --------------------------------
	class EnemyKillEvent : public Event
	{
	public:
		EnemyKillEvent(EnemyPtr e, PlayerPtr p);
		~EnemyKillEvent(){};
		
		EnemyPtr getEnemy(){return mEnemy;}
		PlayerPtr getPlayer(){return mPlayer;}

	private:
		EnemyPtr mEnemy;
		PlayerPtr mPlayer;

		Ogre::Real damage;
	};

	typedef boost::shared_ptr<EnemyKillEvent> EnemyKillEventPtr;

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
		ItemCatchEvent(/*PlayerPtr p,*/ ItemPtr item);
		~ItemCatchEvent(){};
		
		/*PlayerPtr getPlayer(){return mPlayer;}*/
		ItemPtr getItem(){return mItem;}

	private:		
		/*PlayerPtr mPlayer;*/
		ItemPtr mItem;
	};

	typedef boost::shared_ptr<ItemCatchEvent> ItemCatchEventPtr;
}

#endif // __EVENT_H_
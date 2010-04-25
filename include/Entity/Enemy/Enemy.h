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
#ifndef __ENEMY_H__
#define __ENEMY_H__

#include <Ogre.h>

#include "..\EntityInterface.h"
#include "..\..\Physics\PhysicsInterface.h"
#include "..\..\Logic\LogicInterface.h"

#define ENEMY_BILLBOARD_SHOW_TIME 2.0f // seconds

namespace WyvernsAssault
{
	/** List of enemy types */
	enum EnemyTypes
	{
		Naked	= 0,
		Soldier	= 1,
		Wizard	= 2,
		Knight	= 3,
		Peasant	= 4,
		Woman	= 5
	};

	enum EnemyStates
	{
		Idle = 0,
		Sleeping = 1,
		What = 2,
		Alert = 3,
		Rage = 4,
		Love = 5,
		Fear = 6,
		Magic = 7,
		Patrol = 8
	};

	/**
	Class used to manage all the enemies
	*/
	class Enemy : public EntityInterface, public PhysicsInterface, public LogicInterface
	{
	public:
		Enemy(EnemyTypes type);
		~Enemy();

		virtual void updateEntity(const float elapsedSeconds);
		virtual void updateLogic(lua_State *L, const float elapsedSeconds);

		void setBillboardSet(BillboardSet* balloonSet);

		float getStateTimeout(){return mStateTimeout;}

		void setTarget(SceneNode* target);
		void autoTrackTarget();

	private:
		EnemyTypes mType;
		BillboardSet* mBalloonSet;
		Billboard* mBalloon;

		SceneNode* mTarget;

		EnemyStates mState;
		float mStateTimeout;

	public:
		static EnemyTypes StringToType(Ogre::String typeStr);
	};

	typedef boost::shared_ptr<Enemy> EnemyPtr;
}

#endif // __ENEMY_H__
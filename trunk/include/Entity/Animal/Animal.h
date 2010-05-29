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
#ifndef __ANIMAL_H__
#define __ANIMAL_H__

#include <Ogre.h>

#include "..\EntityInterface.h"
#include "..\..\Physics\PhysicsInterface.h"
#include "..\..\Logic\LogicInterface.h"

#define ANIMAL_SPEED_SLOW 10.0f
#define ANIMAL_SPEED_MEDIUM 30.0f
#define ANIMAL_SPEED_FAST 50.0f

#define ANIMAL_ROTATION_SPEED 3.0f

namespace WyvernsAssault
{
	/** List of animal types */
	enum AnimalTypes
	{
		Chicken	= 0,
		Cow	= 1
	};

	enum AnimalStates
	{
		Initial_animal = 0,
		Idle_animal = 1,
		Alert_animal = 2
	};

	/**
	Class used to manage all the animals
	*/
	class Animal : public EntityInterface, public PhysicsInterface, public LogicInterface
	{
	public:
		Animal(AnimalTypes type);
		~Animal();

		void initializeEntity(Ogre::Entity* mesh, Ogre::SceneNode* sceneNode);
		void finalizeEntity();

		virtual void updateEntity(const float elapsedSeconds);
		virtual void updateLogic(lua_State *L, const float elapsedSeconds);

		float getStateTimeout(){return mStateTimeout;}

		void setTarget(SceneNode* target);
		void autoTrackTarget();

		void move(Real x, Real y, Real z);
		void move(Vector3 to);

		bool isHurt();

	private:
		AnimalTypes mType;

		SceneNode* mTarget; 

		AnimalStates mState;
		float mStateTimeout;
		
		float mSpeed;
		float mMaxLife;
		float mLife;
		Ogre::Vector3 mDirection;

	private:
		void chase();

	public:
		static AnimalTypes StringToType(Ogre::String typeStr);
	};

	typedef boost::shared_ptr<Animal> AnimalPtr;
}

#endif // __ANIMAL_H__
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
#ifndef __ANIMAL_MANAGER_H__
#define __ANIMAL_MANAGER_H__

#include <Ogre.h>
#include <OgreSingleton.h>
#include <boost/enable_shared_from_this.hpp>

#include <vector>

#include "..\..\Lua\LuaInterface.h"
#include "..\..\Entity\EntityManager.h"
#include "..\..\Events\EventsInterface.h"
#include "Animal.h"

namespace WyvernsAssault
{
	typedef std::map<Ogre::String,AnimalPtr> AnimalMap;
	typedef std::map<Ogre::String,AnimalPtr>::iterator AnimalMapIterator;

	typedef std::vector<AnimalPtr> AnimalList;
	typedef std::vector<AnimalPtr>::iterator AnimalListIterator;

	/**
	Class used to manage all the animals
	*/
	class AnimalManager : public Ogre::Singleton<AnimalManager>, public boost::enable_shared_from_this<AnimalManager>, public EntityManager, public LuaInterface, public EventsInterface
	{
		public:
		AnimalManager(Ogre::SceneManager* sceneManager);
		~AnimalManager();
		static AnimalManager& getSingleton(void);
		static AnimalManager* getSingletonPtr(void);
		
		void initialize();
		void finalize();
		SceneManager* getSceneManager(){return this->mSceneManager;}

		AnimalPtr createAnimal(AnimalTypes type);
		AnimalPtr createAnimal(AnimalTypes type, Ogre::String name, Ogre::String mesh);

		int getCount();

		AnimalPtr getAnimal(int index);
		AnimalPtr getAnimal(Ogre::String name);

		bool removeAnimal(Ogre::String name);

		/* TODO : Animal IA
		*/
		void update(const float elpasedSeconds);

		// ----------------
		// Events interface
		// ----------------
		// Register event handlers
		void registerHandlers();
		// Unregister handlers
		void unregisterHandlers();

		//void handleCollisionEvent(CollisionEventPtr evt);
		//void handleAnimalHitEvent(AnimalHitEventPtr evt);

	private:
		Ogre::String createUniqueId();

	private:
		int mCount;
		int mId;

		AnimalList mAnimalList;
		AnimalMap mAnimalMap;

		Ogre::SceneManager* mSceneManager;

	// --------------------------------
	// BEGIN Lua Interface Declarations
	// --------------------------------
	public:
		// Animals Lib (exported to Lua)
		LUA_LIBRARY("Animal", animallib);

		LUA_FUNCTION(createAnimal)
		LUA_FUNCTION(getAnimalCount)
		LUA_FUNCTION(getAnimalName)
		LUA_FUNCTION(getAnimalPosition)
		LUA_FUNCTION(setAnimalPosition)
		LUA_FUNCTION(setAnimalState)
		LUA_FUNCTION(setAnimalTarget)
		LUA_FUNCTION(getAnimalStateTimeout)
		LUA_FUNCTION(isAnimalHurt)
		LUA_FUNCTION(removeAnimal)

	public:
		void luaLoadScripts(){luaLoadScript(".\\data\\scripts\\AnimalLogic.lua");}

	private:
		static const struct luaL_reg l[];
	// ------------------------------
	// END Lua Interface Declarations
	// ------------------------------
	};

	typedef boost::shared_ptr<AnimalManager> AnimalManagerPtr;
}

#endif // __ANIMAL_MANAGER_H__
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
#ifndef __PARTICLE_MANAGER_H__
#define __PARTICLE_MANAGER_H__

#include <Ogre.h>
#include <OgreSingleton.h>
#include <ParticleUniversePlugin.h>

#include <boost/enable_shared_from_this.hpp>

#include "ParticleSystem.h"
#include "..\Camera\CameraManager.h"
#include "..\Events\EventsInterface.h"
#include "..\Lua\LuaInterface.h"

using namespace Ogre;

namespace WyvernsAssault 
{
	typedef std::map<Ogre::Real,ParticleUniverse::ParticleSystem*> ParticleSystemMap;
	typedef std::map<Ogre::Real,ParticleUniverse::ParticleSystem*>::iterator ParticleSystemMapIterator;

	/**
	Class used to manage all the particle systems
	*/
	class ParticleManager	: public Ogre::Singleton<ParticleManager>
							, public boost::enable_shared_from_this<ParticleManager>
							, public EventsInterface
							, public LuaInterface
	{
	public:
		ParticleManager(SceneManager* sceneManager);
		~ParticleManager();
		static ParticleManager& getSingleton(void);
		static ParticleManager* getSingletonPtr(void);

	public:
		void initialize();
		void finalize();

		void update(const float elapsedSeconds);

		// Particle system function	
		ParticleUniverse::ParticleSystem* create(String script);

		void add(SceneNode* node, String script, Real repeat = 0.0);
		void add(SceneNode* node, String id, String script, Real repeat = 0.0);
		void remove(String id);

		// Special particle systems
		void bloodLens();
		void magicBolt(SceneNode* node);

	private:
		Ogre::String createUniqueId();

	public:
		// ----------------
		// Events interface
		// ----------------
		EVENTS_INTERFACE()

		EVENTS_HANDLER(PlayerAttack)
		EVENTS_HANDLER(PlayerHit)
		EVENTS_HANDLER(EnemyAttack)
		EVENTS_HANDLER(EnemyHit)
		EVENTS_HANDLER(EnemyKilled)
		EVENTS_HANDLER(EnemyCustom)
		EVENTS_HANDLER(ItemCreation)
		EVENTS_HANDLER(ItemCatch)
		EVENTS_HANDLER(ProjectileUpdate)
		EVENTS_HANDLER(ProjectileHit)
		EVENTS_HANDLER(ObjectHit)
		EVENTS_HANDLER(ObjectKilled)
		EVENTS_HANDLER(ObjectCustom)

		// --------------------------------
		// BEGIN Lua Interface Declarations
		// --------------------------------
		LUA_INTERFACE();

		//Particle Lib (exported to Lua)
		LUA_LIBRARY("Particle",particlelib);

		LUA_FUNCTION(createSystem);
		LUA_FUNCTION(removeSystem);
		// ------------------------------
		// END Lua Interface Declarations
		// ------------------------------

	private:
		ParticleUniverse::ParticleSystemManager*	mParticleSystemManager;
		ParticleUniverse::ParticleSystem*			mParticleSystem;

		// Map for particle system that need to be re started in time (with a loop)
		ParticleSystemMap							mParticleSystemMap;

		int		mId;
		Real	mTimer;

	private:
		SceneManager*			mSceneManager;
	};

	typedef boost::shared_ptr<ParticleManager> ParticleManagerPtr;
}
	
#endif // __PARTICLE_MANAGER_H__

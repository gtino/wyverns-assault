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

#include "..\Events\EventsInterface.h"

using namespace Ogre;

namespace WyvernsAssault 
{
	/**
	Class used to manage all the particle systems
	*/
	class ParticleManager : public Ogre::Singleton<ParticleManager>, public boost::enable_shared_from_this<ParticleManager>, public EventsInterface
	{
	public:
		ParticleManager(SceneManager* sceneManager);
		~ParticleManager();
		static ParticleManager& getSingleton(void);
		static ParticleManager* getSingletonPtr(void);

	public:
		/** Initialize the particle manager */
		void initialize();
		/** Finalize the particle manager */
		void finalize();

		/** Update and run lua scripts */
		void update(const float elapsedSeconds);

		/** Create particle system function */		
		ParticleUniverse::ParticleSystem* create(String id, String material);

		/** Blood particles */
		void bloodHit(Vector3 position);
		void bloodKill(Vector3 position);
		void bloodLens();

		/** Hit particles */
		void hit(Vector3 position);

		// ----------------
		// Events interface
		// ----------------
		// Register event handlers
		void registerHandlers();
		// Unregister handlers
		void unregisterHandlers();

		void handleEnemyHitEvent(EnemyHitEventPtr evt);
		void handlePlayerHitEvent(PlayerHitEventPtr evt);

	private:
		ParticleUniverse::ParticleSystemManager*	mParticleSystemManager;
		ParticleUniverse::ParticleSystem*			mParticleSystem;

	private:
		SceneManager*			mSceneManager;

	};

	typedef boost::shared_ptr<ParticleManager> ParticleManagerPtr;
}
	
#endif // __PARTICLE_MANAGER_H__
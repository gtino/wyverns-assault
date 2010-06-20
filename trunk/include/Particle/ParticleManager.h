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
	typedef std::map<Ogre::String,ParticleSystemPtr> ParticleSystemMap;
	typedef std::map<Ogre::String,ParticleSystemPtr>::iterator ParticleSystemMapIterator;

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
		/** Initialize the particle manager */
		void initialize();
		/** Finalize the particle manager */
		void finalize();

		/** Update and run lua scripts */
		void update(const float elapsedSeconds);

		/** Create particle system function */		
		ParticleUniverse::ParticleSystem* create(String id, String script);
		
		/** Add particle system to node and start it */
		void add(SceneNode* node, String id, String script);		

		/** Remove particle system from node and stop it */
		void remove(SceneNode* node, String id);

		/** Blood particles */
		void bloodHit(SceneNode* node);
		void bloodKill(SceneNode* node);
		void bloodLens();

		/** Hit particles */
		void hit(SceneNode* node);

		/** Fire particles **/
		void fireHit(SceneNode* node);
		void fireKill(SceneNode* node);

		/** Animal particles **/
		void chickenKill(SceneNode* node);

		/** Glow particles */
		void glow(SceneNode* node);

		// ----------------
		// Events interface
		// ----------------
		EVENTS_INTERFACE()

		EVENTS_HANDLER(EnemyHit)
		EVENTS_HANDLER(EnemyKill)
		EVENTS_HANDLER(ItemCatch)

		// --------------------------------
		// BEGIN Lua Interface Declarations
		// --------------------------------
	public:
		//Particle Lib (exported to Lua)
		LUA_LIBRARY("Particle",particlelib);

		LUA_FUNCTION(luaCreateSystem);
		LUA_FUNCTION(luaRemoveSystem);

	public:
		void luaLoadScripts(){};
		// ------------------------------
		// END Lua Interface Declarations
		// ------------------------------

	private:
		ParticleUniverse::ParticleSystemManager*	mParticleSystemManager;
		ParticleUniverse::ParticleSystem* mParticleSystem;

		ParticleSystemMap mParticleSystemMap;

	private:
		SceneManager*			mSceneManager;
	};

	typedef boost::shared_ptr<ParticleManager> ParticleManagerPtr;
}
	
#endif // __PARTICLE_MANAGER_H__
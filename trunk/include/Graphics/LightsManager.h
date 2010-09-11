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
#ifndef __LIGHTS_MANAGER_H__
#define __LIGHTS_MANAGER_H__

#define LIGHT_NODE_NAME "LightNode"

#include <Ogre.h>
#include <OgreSingleton.h>
#include <boost/enable_shared_from_this.hpp>

#include "..\Debug\Debug.h"
#include "..\Utils\Utils.h"
#include "..\Lua\LuaInterface.h"
#include "..\Events\EventsInterface.h"

// Light types
#define AMBIENT		1
#define POINT		2
#define DIRECTIONAL	3
#define SPOT		4

using namespace Ogre;

namespace WyvernsAssault 
{
	/**
	Class used to manage all the lights
	*/
	class LightsManager :	public Ogre::Singleton<LightsManager>, 
							public boost::enable_shared_from_this<LightsManager>, 
							public LuaInterface,
							public EventsInterface
	{
	public:
		LightsManager(SceneManager* sceneManager);
		~LightsManager();
		static LightsManager& getSingleton(void);
		static LightsManager* getSingletonPtr(void);

	public:
		/** Initialize the lights manager */
		void initialize();
		/** Finalize the lights manager */
		void finalize();
		/** Update and run lua scripts */
		void update(const float elapsedSeconds);

		/** Create light function **/		
		void createLight(String name, Light::LightTypes type, ColourValue diffuse, ColourValue specular, Vector3 position = Vector3::ZERO, Vector3 direction = Vector3::ZERO);

		Light* getLight(String name);

		/** Modify scene ambient light **/
		void setAmbientLight(ColourValue color);
		ColourValue getAmbientLight();

		Ogre::SceneNode* _getSceneNode() const { return mLightNode; }

	private:
		Light* mLight;

		SceneManager* mSceneManager;
		SceneNode* mLightNode;

	public:
		// ----------------
		// Events interface
		// ----------------
		EVENTS_INTERFACE()

		EVENTS_HANDLER(WeatherChanged)

	// --------------------------------
	// BEGIN Lua Interface Declarations
	// --------------------------------
	public:
		LUA_INTERFACE();

		// Export library lib as "Light" to be called from Lua scripts
		LUA_LIBRARY("Light",lightlib)

		// From Lua to C++
		LUA_FUNCTION(getLightDiffuseColor)
		LUA_FUNCTION(setLightDiffuseColor)
		LUA_FUNCTION(getLightPosition)
		LUA_FUNCTION(setLightPosition)
		LUA_FUNCTION(getAmbientLight)
		LUA_FUNCTION(setAmbientLight)

	private:
		// From C++ to Lua
		bool runDayLight(const float totalSeconds);
	// ------------------------------
	// END Lua Interface Declarations
	// ------------------------------
	};

	typedef boost::shared_ptr<LightsManager> LightsManagerPtr;
}

#endif // __LIGHTS_MANAGER_H__
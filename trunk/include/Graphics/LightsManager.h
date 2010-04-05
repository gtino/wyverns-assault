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

#include <Ogre.h>

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
	class LightsManager
	{
	public:
		LightsManager(SceneManager* sceneManager);
		~LightsManager();

	public:
		/** Initialize the lights manager */
		void initialize();
		/** Finalize the lights manager */
		void finalize();

		/** Create light function **/		
		void createLight(String name, Light::LightTypes type, ColourValue diffuse, ColourValue specular, Vector3 position = Vector3::ZERO, Vector3 direction = Vector3::ZERO);

		/** Modify scene ambient light **/
		void setAmbientLight(ColourValue color);

		/** Disable Light **/
		void disable() { mSceneManager->destroyAllLights(); }

	private:
		Light* mLight;

	private:
		SceneManager* mSceneManager;
	};
}

#endif // __LIGHTS_MANAGER_H__
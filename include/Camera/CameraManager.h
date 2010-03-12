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
#ifndef __CAMERA_MANAGER_H_
#define __CAMERA_MANAGER_H_

#include <Ogre.h>

#define MAXCAMERAS	10

using namespace Ogre;

namespace WyvernsAssault
{
	/**
		Class used to manage differents cameras
	*/
	class CameraManager
	{
	public:
		CameraManager(SceneManager* sceneManager, Root* root);
		~CameraManager();

	public: 
		/** Initialize the camera manager */
		void initialize();
		/** Finalize the camera manager */
		void finalize();
		/** Load camera resources */
		void loadResources();
		/** Unload camera resources */
		void unloadResources();

		/** Set camera from camera array **/
		void setCamera(String name, Vector3 position, Vector3 lootAt);
		/** Get camera from camera array **/
		Camera* getCamera(String name);
		/** Enable camera **/
		void enableCamera(String name);

		/** Set default camera **/
		void setDefaultCamera();
		/** Get camera camera **/
		String getDefaultCamera();
		/** Enable default camera **/
		void enableDetaultCamera();
	
	private:
		Camera*			mCameras[MAXCAMERAS];
		Camera*			mDefaultCamera;
		Viewport*		mViewport;

	private:
		Root*			mRoot;
		SceneManager*	mSceneManager;
	};
}

#endif // __CAMERA_MANAGER_H_
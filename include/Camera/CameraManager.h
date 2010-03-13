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

#define FIXEDCAMERAS	8
#define PLAYERHEIGHT	5
// Camera types
#define GAMECAMERA		1
#define FPSCAMERA		2
#define FIXEDCAMERA		3
#define TRAVELLCAMERA	4

using namespace Ogre;

namespace WyvernsAssault
{
	/**
		Class used to manage differents cameras
	*/
	class CameraManager
	{
	public:
		CameraManager(SceneManager* sceneManager, RenderWindow* renderWindow);
		~CameraManager();

	public: 
		/** Initialize the camera manager */
		void initialize();
		/** Finalize the camera manager */
		void finalize();

		/** Get camera **/
		Camera* getCamera(){ return mCamera; }
		/** Get camera type **/
		int getCameraType(){ return mCameraType; }

		void positionCamera(Vector3 position);
		void lookAtCamera(Vector3 lookAt);
		void moveCamera(Vector3 move);
		void followNode(SceneNode* node, Vector3 offset = Vector3::ZERO);
		void updateCamera(SceneNode* node);

		void gameCamera(SceneNode* node);
		void fpsCamera(SceneNode* node);
		void fixedCamera(int id);

	
	private:
		Camera*			mCamera;
		int				mCameraType;
		Viewport*		mViewport;
		Vector3			mFixedCameras[FIXEDCAMERAS][2];

	private:
		SceneManager*	mSceneManager;
		RenderWindow*	mRenderWindow;
	};
}

#endif // __CAMERA_MANAGER_H_
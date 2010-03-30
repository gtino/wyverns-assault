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

#include <CCSCameraControlSystem.h>
#include "CCSBasicCameraModes.h"
#include "CCSFreeCameraMode.h"
#include "CCSOrbitalCameraMode.h"

#define FIXEDCAMERAS	8

// Camera types
#define GAMECAMERA		0x00
#define FPSCAMERA		0x01
#define FIXEDCAMERA		0x02
#define TRAVELCAMERA	0x03


using namespace Ogre;

namespace WyvernsAssault
{
	/**
		Class used to manage differents cameras
	*/
	class CameraManager
	{
	public:
		CameraManager(SceneManager* sceneManager, RenderWindow* renderWindow, Viewport* viewport);
		~CameraManager();

	public: 
		/** Initialize the camera manager */
		void initialize(SceneNode* player);
		/** Finalize the camera manager */
		void finalize();

		/** Get camera position **/
		Vector3 getCameraPosition(){ return mCameraCS->getCameraPosition(); }
		/** Get camera type **/
		int getCameraType(){ return mCameraType; }
		/** Get camera mode**/
		String getCameraMode(){ return mCameraCS->getCameraModeName(mCameraCS->getCurrentCameraMode()); }

		/** Camera functions **/
		void updateCamera(SceneNode* node);

		/** Camera types functions **/
		void gameCamera();
		void fpsCamera();
		void fixedCamera(int id);
		void travelCamera(int id);
		void scenarioCamera();
		void nextCamera() { mCameraCS->setCurrentCameraMode(mCameraCS->getNextCameraMode()); }

		/** Fixed cameras functions **/
		void setFixedCamera(int camera, Vector3 position, Real roll, Real yaw, Real pitch);

		/** Debug camera functions **/
		void switchtPolygonMode();
		String getPolygonMode();
	
	public:
		Camera*			mCamera;
		int				mCameraType;

		// Camera control system
		CCS::CameraControlSystem*			mCameraCS;
		// Camera modes
		CCS::PlaneBindedCameraMode*			mCamPlaneMode;
		CCS::FixedDirectionCameraMode*		mCamFixedDirMode;
		CCS::FixedCameraMode*				mCamFixedMode;
		CCS::FirstPersonCameraMode*			mCamFirstPersonMode;		
		CCS::ChaseCameraMode*				mCamChaseMode;
		CCS::AttachedCameraMode*			mCamAttachedMode;


	private:
		SceneManager*	mSceneManager;
		RenderWindow*	mRenderWindow;
		Viewport*		mViewport;
	};
}

#endif // __CAMERA_MANAGER_H_
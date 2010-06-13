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
#include <OgreSingleton.h>
#include <ois/OIS.h>
#include <boost/enable_shared_from_this.hpp>

#include "SdkCameraMan.h"

using namespace Ogre;

namespace WyvernsAssault
{
	/**
		Class used to manage differents cameras
	*/
	class CameraManager : public Ogre::Singleton<CameraManager>, public boost::enable_shared_from_this<CameraManager>
	{
	public:
		CameraManager(SceneManager* sceneManager, RenderWindow* window, Viewport* viewport);
		~CameraManager();
		static CameraManager& getSingleton(void);
		static CameraManager* getSingletonPtr(void);

	public: 
		/** Initialize the camera manager */
		void initialize();
		/** Finalize the camera manager */
		void finalize();

	public:
		/** Camera functions */
		Vector3 getCameraPosition();
		Vector3 getCameraLookAt();
		String getCameraMode() { return mCameraMode ; }
		int getGameArea(Vector3 position);

		Camera* getCamera(){return mCamera;}
		
		/** Camera moving direction */
		Vector3 getDirection(Vector3 direction);
		
		/** Update camera position in game mode */
		void updateCamera(Vector3 player, Real elapsedSeconds);
		
		/** Create transition animation */
		void createTransition(Vector3 begin, Vector3 end, Vector3 lbegin, Vector3 lend);

		/** Add game area  and camera segment */
		void addGameArea(Vector3 beginNear, Vector3 endNear, Vector3 beginFar, Vector3 endFar);
		void addCameraSegment(Vector3 pbegin, Vector3 pend, Vector3 lbegin, Vector3 lend);

		/** Fixed cameras functions */
		void setFixedCamera(int camera, Vector3 position, Vector3 lookAt);
		
		/** Camera effects */
		void zoom(Real zoom);
		void rumble(Real scale);
		void tremor(Real scale);
		void shake(Real scale);

		/** Camera modes */		
		void gameCamera();
		void freeCamera();
		void fixedCamera(int camera);
		void resumeCamera();

		/** Free camera functions */
		void freeCameraMouse(OIS::MouseEvent evt);
		void freeCameraKeyboardDown(OIS::KeyEvent evt);
		void freeCameraKeyboardUp(OIS::KeyEvent evt);

		/**  Rendering queue */
		bool frameRenderingQueued(FrameEvent evt);		

		/** Debug camera functions */
		void setPolygonMode(PolygonMode pm){ mCamera->setPolygonMode(pm);}
		PolygonMode getPolygonMode(){ return mCamera->getPolygonMode(); }
		void toogleAxes(){ mAxesNode->flipVisibility(); }
		void showAxes();
		void hideAxes();

	protected:
		SceneManager*			mSceneManager;
		RenderWindow*			mWindow;
		Viewport*				mViewport;

	protected:
		Camera*					mCamera;
		SceneNode*				mCameraNode;
		SceneNode*				mCameraLookAtNode;

		String					mCameraMode;
		Real					mCameraZoom;
		int						mGameArea;

		Entity*					mAxes;
		SceneNode*				mAxesNode;

		Animation*				mCameraAnimation;
		Animation*				mLookAtAnimation;
		Animation*				mAxesAnimation;
		Animation*				mCameraEffectAnimation;
		Animation*				mCameraEffectLookAnimation;

		AnimationState*			mCameraTransition;
		AnimationState*			mLookAtTransition;
		AnimationState*			mAxesTransition;
		AnimationState*			mCameraEffect;
		AnimationState*			mCameraEffectLook;

		// Sdk Camera Manager for free look camera
		OgreBites::SdkCameraMan*		mCameraMan;

	private:
		struct FixedCamera
		{
			int mId;
			Ogre::Vector3 mPosition;
			Ogre::Vector3 mLookAt;
		};

		std::vector<FixedCamera>		mFixedCameras;

		int								mFixedCamera;

	private:
		struct GameArea
		{
			Ogre::Vector3	mBeginNear;
			Ogre::Vector3	mEndNear;
			Ogre::Vector3	mBeginFar;
			Ogre::Vector3	mEndFar;
		};

		std::vector<GameArea> mGameAreas;

		struct CameraSegment
		{
			Ogre::Vector3	mPositionBegin;
			Ogre::Vector3	mLookAtBegin;
			
			Ogre::Vector3	mPositionEnd;
			Ogre::Vector3	mLookAtEnd;
		};

		std::vector<CameraSegment>		mCameraSegments;

	};

	typedef boost::shared_ptr<CameraManager> CameraManagerPtr;
}

#endif // __CAMERA_MANAGER_H_
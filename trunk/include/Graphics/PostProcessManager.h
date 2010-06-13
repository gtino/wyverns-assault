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
#ifndef __POSTPROCESS_MANAGER_H_
#define __POSTPROCESS_MANAGER_H_

#include <Ogre.h>
#include <OgreSingleton.h>

#include <boost/enable_shared_from_this.hpp>

#include "..\Events\EventsInterface.h"

using namespace Ogre;

namespace WyvernsAssault 
{
	/**
	Class used to manage all the postprocessing effects
	*/
	class PostProcessManager	: public Ogre::Singleton<PostProcessManager>
								, public boost::enable_shared_from_this<PostProcessManager>
								, public EventsInterface
	{
	public:
		PostProcessManager(Viewport* viewport);
		~PostProcessManager();
		static PostProcessManager& getSingleton(void);
		static PostProcessManager* getSingletonPtr(void);

		void initialize();
		void finalize();

		// Disable all compositors
		void disableAll();
		// Pause/Resume compositors
		void pause();
		void resume();

		// Compositor toogle
		void depthOfField();		
		void showDepth();
		void showNormal();		

		// Compositor with duration
		void motionBlur(float duration);
		void radialBlur(float duration);

		// Update compositor time
		void update(float elapsedSeconds);

	public:
		// ----------------
		// Events interface
		// ----------------
		// Register event handlers
		void registerHandlers(){};
		// Unregister handlers
		void unregisterHandlers(){};

	private:
		Viewport*			mViewport;

	private: 
		CompositorManager*	mCompositorManager;

		// Compositor control variables
		bool		mDOF;		
		bool		mShowDepth;
		bool		mShowNormal;

		float		mMotionBlur;
		float		mRadialBlur;
	};

	typedef boost::shared_ptr<PostProcessManager> PostProcessManagerPtr;
}

#endif //__POSTPROCESS_MANAGER_H_

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
#ifndef __AUDIO_MANAGER_H_
#define __AUDIO_MANAGER_H_

#include <Ogre.h>
#include <OgreSingleton.h>
#include <boost/enable_shared_from_this.hpp>

#include <vector>
#include <map>

#include "..\..\externals\Fmod\include\fmod.h"
#include "..\..\externals\Fmod\include\fmod_codec.h"
#include "..\..\externals\Fmod\include\fmod_dsp.h"
#include "..\..\externals\Fmod\include\fmod_errors.h"
#include "..\..\externals\Fmod\include\fmod_memoryinfo.h"
#include "..\..\externals\Fmod\include\fmod_output.h"

#include "..\..\include\Lua\LuaInterface.h"
#include "..\..\include\Events\EventsInterface.h"

#include "Sound.h"

namespace WyvernsAssault
{
	typedef std::map<Ogre::String, SoundPtr> SoundsMap;
	typedef std::map<Ogre::String, SoundPtr>::iterator SoundsMapIterator;
	
	typedef std::vector<SoundPtr> SoundsList;
	typedef std::vector<SoundPtr>::iterator SoundsListIterator;

	/**
		Class used to manager audio (sound track, sounds, fx)
	*/
	class AudioManager : public Ogre::Singleton<AudioManager>, public boost::enable_shared_from_this<AudioManager>, public LuaInterface, public EventsInterface
	{
	public:
		AudioManager();
		~AudioManager();
		static AudioManager& getSingleton(void);
		static AudioManager* getSingletonPtr(void);

	public: 
		/** Initialize the audio manager */
		void initialize();
		/** Finalize the audio manager */
		void finalize();
		/** Load audio resources */
		void loadResources();
		/** Unload audio resources */
		void unloadResources();
		/** Play audio */
		void playSound(int SoundId);
		/** Stop audio */
		void stopSound(int SoundId);
		
		SoundPtr createSound(SoundTypes type);
		SoundPtr createSound(SoundTypes type, Ogre::String name/*, Ogre::String audio*/);

		//FMOD::System* mSystem;
		//FMOD::Sound* mSound;
		int mChannel;
		unsigned int version;

		// ----------------
		// Events interface
		// ----------------
		// Register event handlers
		void registerHandlers();
		// Unregister handlers
		void unregisterHandlers();

		void handleEnemyHitEvent(EnemyHitEventPtr evt);

	private:
		Ogre::String createUniqueId();

		//int mSound;
		//String mSoundId;

		SoundsList mSoundList;
		SoundsMap  mSoundMap;
	// --------------------------------
	// BEGIN Lua Interface Declarations
	// --------------------------------
	public:

		
		//Audio Lib (exported to Lua)
		LUA_LIBRARY("Audio",audiolib);

		LUA_FUNCTION(luaPlaySound);

	public:
		void luaLoadScripts(){};
	// ------------------------------
	// END Lua Interface Declarations
	// ------------------------------
	};

	typedef boost::shared_ptr<AudioManager> AudioManagerPtr;
}

#endif // __AUDIO_MANAGER_H_


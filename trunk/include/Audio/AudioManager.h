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

#define MAX_SOUND_CHANNELS       200

#define INVALID_SOUND_INDEX      0
#define INVALID_SOUND_CHANNEL    -1

#define INITIAL_VECTOR_SIZE   100
#define INCREASE_VECTOR_SIZE  20

#define DOPPLER_SCALE         1.0
#define DISTANCE_FACTOR       1.0
#define ROLLOFF_SCALE         0.5

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

#include "..\..\include\Debug\Debug.h"

#include "..\..\include\Lua\LuaInterface.h"
#include "..\..\include\Events\EventsInterface.h"

#include "Sound.h"
#include "Channel.h"
#include "FileLocator.h"

namespace WyvernsAssault
{
	typedef std::map<Ogre::String, int> SoundNameToIdMap;
	typedef std::map<Ogre::String, int>::iterator SoundNameToIdMapIterator;

	typedef std::map<Ogre::String, int> NameToChannelMap;
	typedef std::map<Ogre::String, int>::iterator NameToChannelMapIterator;

	//typedef std::vector<SoundPtr> SoundsList;
	//typedef std::vector<SoundPtr>::iterator SoundsListIterator;

	typedef std::vector<SoundInstance *>   SoundInstanceVector;
	typedef SoundInstanceVector::iterator  SoundInstanceVectorItr;

	/**
	Class used to manager audio (sound track, sounds, fx)
	*/
	class AudioManager	: public Ogre::Singleton<AudioManager>
						, public boost::enable_shared_from_this<AudioManager>
						, public LuaInterface
						, public EventsInterface
	{
	public:
		AudioManager();
		~AudioManager();
		static AudioManager& getSingleton(void);
		static AudioManager* getSingletonPtr(void);

	public: 
		/** Initialize the audio manager */
		bool initialize();
		/** Finalize the audio manager */
		void finalize();
		/** Load audio resources */
		void loadResources();
		/** Unload audio resources */
		void unloadResources();

		// -------------
		// Sound Manager
		// -------------
		void                 stopAllSounds(void);
		void                 update(Ogre::Vector3 listenerPosition, Ogre::Quaternion listenerOrientation, Ogre::Real timeElapsed);

		int                  createSound(String &fileName);         // single-shot 3D sound.  returns soundIndex
		int                  createStream(String &fileName);        // single-shot 2D stream.  returns soundIndex
		int                  createLoopedSound(String &fileName);   // looping 3D sound.  returns soundIndex
		int                  createLoopedStream(String &fileName);  // looping 2D stream.  returns soundIndex

		int                  createSound(String &fileName, SOUND_TYPE soundType);

		void				 playSound(String soundName, Ogre::Vector3 soundPosition, int *channelIndex);
		void                 playSound(int soundIndex, Ogre::Vector3 soundPosition, int *channelIndex);
		void				 playSoundTrack(String name);
		void                 stopSound(int *channelIndex);
		int                  findSound(String &fileName, SOUND_TYPE soundType);    // returns soundIndex;

		void                 set3DMinMaxDistance(int channelIndex, float minDistance, float maxDistance);

		float                getSoundLength(int soundIndex);        // returns seconds
		SoundInstance*   	 getSoundInstance(int soundIndex);
		FMOD::Channel*       getFMODChannel(int channelIndex);

	private:
		bool mInitialized;

		int						mNextSoundInstanceIndex;
		FMOD::System*			mSystem;
		Ogre::Vector3			mPrevListenerPosition;
		SoundInstanceVector*	mSoundInstanceVector;
		NameToChannelMap		mSceneNodeToChannelMap;
		ChannelInstance			mChannelArray[MAX_SOUND_CHANNELS];
		int						mSoundTrackChannelIndex; // HACK! Remove this shit, use an entity based channel

		SoundNameToIdMap	 mSoundNameToIdMap;

		void                 incrementNextSoundInstanceIndex(void);

		// --------------
		// FMOD Callbacks
		// --------------
		static FMOD_RESULT F_CALLBACK  fmodFileOpenCallback(const char *fileName, int unicode, unsigned int *filesize, void **handle, void **userdata);
		static FMOD_RESULT F_CALLBACK  fmodFileCloseCallback(void *handle, void *userdata);
		static FMOD_RESULT F_CALLBACK  fmodFileReadCallback(void *handle, void *buffer, unsigned int sizebytes, unsigned int *bytesread, void *userdata);
		static FMOD_RESULT F_CALLBACK  fmodFileSeekCallback(void *handle, unsigned int pos, void *userdata);

		// ----------------
		// Events interface
		// ----------------
		EVENTS_INTERFACE()

		EVENTS_HANDLER(PlayerAttack)
		EVENTS_HANDLER(PlayerAttackSpecial)
		EVENTS_HANDLER(PlayerHit)
		EVENTS_HANDLER(PlayerKill)

		EVENTS_HANDLER(EnemyAttack)
		EVENTS_HANDLER(EnemyHit)
		EVENTS_HANDLER(EnemyKill)
		
		EVENTS_HANDLER(ItemCatch)

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


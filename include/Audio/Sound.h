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
#ifndef __SOUND_H__
#define __SOUND_H__

#include <Ogre.h>

namespace WyvernsAssault
{
	enum SoundTypes
	{
		Intro01  = 0,
		Intro02  = 1,
		Menu03	 = 2,
		Attack01 = 3,
		Attack02 = 4,
		Attack03 = 5
	};

	class Sound
	{
	public:
		Sound(SoundTypes type);
		~Sound();
		
		void initialise (void);//initialises sound

		//sound control
		void setVolume (float v); //sets the actual playing sound's volume
		void load (const char * filename); //loads a soundfile
		void unload (void); //frees the sound object
		void play (bool pause = false); //plays a sound (may be started paused; no argument for unpaused)

    //getters
		bool getSound (void); //checks whether the sound is on

    //setters
		void setPause (bool pause); //pause or unpause the sound
		void setSound (bool sound); //set the sound on or off

    //toggles
		void toggleSound (void); //toggles sound on and off
		void togglePause (void); //toggle pause on/off
	
		private:
		SoundTypes mType;
		//SoundStates mState;
		
		static bool mOn; //is sound on?
	    static bool mPossible; //is it possible to play sound?
		static char *mCurrentSound; //currently played sound
		
		static FMOD_RESULT mResult;
		//static FMOD_SYSTEM * mFmodsystem;
		//static FMOD_SOUND * mSound;
		//static FMOD_CHANNEL * mChannel;
		
	private:
		void chase();

	public:
		static SoundTypes StringToType(Ogre::String typeStr);
	};

	typedef boost::shared_ptr<Sound> SoundPtr;
}

#endif // __SOUND_H__
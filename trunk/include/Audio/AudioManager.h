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

#include "..\Lua\LuaInterface.h"

namespace WyvernsAssault
{
	/**
		Class used to manager audio (sound track, sounds, fx)
	*/
	class AudioManager : public LuaInterface
	{
	public:
		AudioManager();
		~AudioManager();

	public: 
		/** Initialize the audio manager */
		void initialize();
		/** Finalize the audio manager */
		void finalize();
		/** Load audio resources */
		void loadResources();
		/** Unload audio resources */
		void unloadResources();

	// --------------------------------
	// BEGIN Lua Interface Declarations
	// --------------------------------
	public:
		// Audio Lib (exported to Lua)
		LUA_LIBRARY("Audio",audiolib);

		LUA_FUNCTION(playSound)

	public:
		void luaLoadScripts(){};
		void luaInitialize(lua_State* L) {LuaInterface::luaInitialize(L);AudioManager::smAudioManager = this;}
		void luaFinalize() {AudioManager::smAudioManager = NULL;}
		void luaReload(){};

	private: // AudioManager
		static AudioManager* smAudioManager;
	// ------------------------------
	// END Lua Interface Declarations
	// ------------------------------
	};
}

#endif // __AUDIO_MANAGER_H_
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
#ifndef __WEATHER_MANAGER_H_
#define __WEATHER_MANAGER_H_

#define LIGHTNING_MIN_TIME	5.0
#define LIGHTNING_MAX_TIME	15.0

#include <Ogre.h>
#include <OgreRenderWindow.h>

#include <boost/enable_shared_from_this.hpp>

#include "..\Debug\Debug.h"
#include "..\Utils\Utils.h"
#include "..\Lua\LuaInterface.h"
#include "..\Events\EventsInterface.h"

//
// Other managers used by the WeatherManager
//

namespace WyvernsAssault
{
	/**
		Class used to manager weather changes
	*/
	class WeatherManager	: public Ogre::Singleton<WeatherManager>
							, public boost::enable_shared_from_this<WeatherManager>
							, public LuaInterface
							, public EventsInterface
	{
	public:
		WeatherManager();
		~WeatherManager();
		static WeatherManager& getSingleton(void);
		static WeatherManager* getSingletonPtr(void);

	public:
		/** Initialize the weather manager */
		bool initialize();
		/** Finalize the weather manager */
		void finalize();

		void update(const float elapsedSeconds);

		//
		// Enable/disable manager
		//
		void disable(){mEnabled = false;}
		void enable(){mEnabled = true;}
		bool isEnabled(){return mEnabled;}

	protected:
		bool mInitialized;
		bool mEnabled;

	private:
		int mCurrentGameArea;
		int mCurrentLevel;

		// Lightning
		float	mTimer;
		float	mRepeat;

	public:
		// ----------------
		// Events interface
		// ----------------
		EVENTS_INTERFACE()

		EVENTS_HANDLER(GameAreaChanged)
		EVENTS_HANDLER(GameAreaCleared)
		EVENTS_HANDLER(SpecialEffect)

	public:
		// --------------------------------
		// BEGIN Lua Interface Declarations
		// --------------------------------
		public:
			LUA_INTERFACE();

			// Weather Lib (exported to Lua)
			LUA_LIBRARY("Weather",weatherlib);

			LUA_FUNCTION(enable);
			LUA_FUNCTION(disable);
			LUA_FUNCTION(isEnabled);
		// ------------------------------
		// END Lua Interface Declarations
		// ------------------------------
	};

	typedef boost::shared_ptr<WeatherManager> WeatherManagerPtr;
}

#endif // __WEATHER_MANAGER_H_
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
#ifndef __CUTSCENE_H_
#define __CUTSCENE_H_

#include <Ogre.h>

#include <boost/shared_ptr.hpp>

#include "..\Utils\Utils.h"

using namespace Ogre;

namespace WyvernsAssault
{		

	class CutScene
	{
		public:
			CutScene(int id, Ogre::String script);
			~CutScene();

		public:
			int getId(){ return mId; }
			Ogre::String getScript(){ return mScript; }

			bool getPlayed(){return mPlayed;}
			void setPlayed(bool played){mPlayed = played;}

		private:
			int mId;
			Ogre::String mScript;
			bool mPlayed;
	};

	typedef boost::shared_ptr<CutScene> CutScenePtr;
}

#endif // __CUTSCENE_H__
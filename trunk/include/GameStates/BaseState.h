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
#ifndef __BASE_STATE_H_
#define __BASE_STATE_H_

namespace WyvernsAssault
{
	/**
		Base game state class
	*/
	class BaseState
	{
	public:
		BaseState();
		virtual ~BaseState() = 0;

	public:
		/** Initialize current state */
		virtual void initialise() = 0;
		/** Load resources */
		virtual void load() = 0;
		/** Update internal stuff */
		virtual void update() = 0;
		/** Render */
		virtual void render() = 0;
		/** Unload resources */
		virtual void unload() = 0;
		/** Destroy the state */
		virtual void destroy() = 0;
	};
}
#endif // __BASE_STATE_H_
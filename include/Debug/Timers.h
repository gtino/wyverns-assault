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
#ifndef __TIMERS_H_
#define __TIMERS_H_

#include <Ogre.h>

#define TIMER(t)		Ogre::Timer m_timer_##t; \
						float m_time_ms_##t
#define TIMER_START(t)	m_timer_##t##.reset()
#define TIMER_STOP(t)	m_time_ms_##t = m_timer_##t##.getMicroseconds() * 1.0 / 1000000;
#define TIMER_GET(t)	m_time_ms_##t
#define TIMER_PERCENT(n,d) Ogre::Math::Floor(TIMER_GET(n) / (TIMER_GET(d) > 0 ? TIMER_GET(d) : 0.000001f) * 100.0f)

#endif // __TIMERS_H_
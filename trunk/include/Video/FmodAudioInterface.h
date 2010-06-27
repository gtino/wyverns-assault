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
#ifndef __FMOD_AUDIO_INTERFACE_H__
#define __FMOD_AUDIO_INTERFACE_H__

#include "TheoraAudioInterface.h"
#include "TheoraVideoClip.h"
#include "TheoraTimer.h"

#include "OgrePrerequisites.h"
#include "OgreLogManager.h"
#include "OgreStringConverter.h"

#include <queue>

namespace Ogre
{
	class FmodAudioInterface : public TheoraAudioInterface, TheoraTimer
	{
		int mMaxBuffSize;
		int mBuffSize;
		short *mTempBuffer;
		float mTimeOffset;

		struct FmodBuffer
		{
			unsigned int id;
			int nSamples;
		}mBuffers[1000];
		std::queue<FmodBuffer> mBufferQueue;

		unsigned int mSource;
		int mNumProcessedSamples,mNumPlayedSamples;
	public:
		FmodAudioInterface(TheoraVideoClip* owner,int nChannels,int freq);
		~FmodAudioInterface();
		void insertData(float** data,int nSamples);
		void destroy();

		void update(float time_increase);

		void pause();
		void play();
		void seek(float time);
	};

	class FmodAudioInterfaceFactory : public TheoraAudioInterfaceFactory
	{
	public:
		FmodAudioInterfaceFactory();
		~FmodAudioInterfaceFactory();
		FmodAudioInterface* createInstance(TheoraVideoClip* owner,int nChannels,int freq);
	};
}

#endif //__FMOD_AUDIO_INTERFACE_H__
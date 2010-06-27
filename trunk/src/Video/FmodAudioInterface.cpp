#include "..\..\include\Video\FmodAudioInterface.h"

//ALCdevice* gDevice=0;
//ALCcontext* gContext=0;

namespace Ogre
{
	short float2short(float f)
	{
		if      (f >  1) f= 1;
		else if (f < -1) f=-1;
		return f*32767;
	}

	FmodAudioInterface::FmodAudioInterface(TheoraVideoClip* owner,int nChannels,int freq) :
		TheoraAudioInterface(owner,nChannels,freq)
	{
		mMaxBuffSize=freq*mNumChannels*2;
		mBuffSize=0;
		mNumProcessedSamples=0;
		mTimeOffset=0;

		mTempBuffer=new short[mMaxBuffSize];
		//alGenSources(1,&mSource);
		owner->setTimer(this);
		mNumPlayedSamples=0;
	}

	void FmodAudioInterface::destroy()
	{
		// todo
	}

	FmodAudioInterface::~FmodAudioInterface()
	{
		// todo: delete buffers and source
		if (mTempBuffer) delete mTempBuffer;
	}

	void FmodAudioInterface::insertData(float** data,int nSamples)
	{
		for (int i=0;i<nSamples;i++)
		{
			if (mBuffSize < mMaxBuffSize)
			{
				mTempBuffer[mBuffSize++]=float2short(data[0][i]);
				if (mNumChannels == 2)
					mTempBuffer[mBuffSize++]=float2short(data[1][i]);
			}
			if (mBuffSize == mFreq*mNumChannels/4)
			{	
				FmodBuffer buff;
				//alGenBuffers(1,&buff.id);
				//ALuint format = (mNumChannels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
				//alBufferData(buff.id,format,mTempBuffer,mBuffSize*2,mFreq);
				//alSourceQueueBuffers(mSource, 1, &buff.id);
				buff.nSamples=mBuffSize/mNumChannels;
				mNumProcessedSamples+=mBuffSize/mNumChannels;
				mBufferQueue.push(buff);

				mBuffSize=0;

				//int state;
				//alGetSourcei(mSource,AL_SOURCE_STATE,&state);
				//if (state != AL_PLAYING)
				//{
				//	//alSourcef(mSource,AL_PITCH,0.5); // debug
				//	alSourcef(mSource,AL_SAMPLE_OFFSET,mNumProcessedSamples-mFreq/4);
				//	alSourcePlay(mSource);

				//}

			}
		}
	}

	void FmodAudioInterface::update(float time_increase)
	{
		int i,state,nProcessed=0;
		FmodBuffer buff;

		// process played buffers

		//alGetSourcei(mSource,AL_BUFFERS_PROCESSED,&nProcessed);
		for (i=0;i<nProcessed;i++)
		{
			buff=mBufferQueue.front();
			mBufferQueue.pop();
			mNumPlayedSamples+=buff.nSamples;
			//alSourceUnqueueBuffers(mSource,1,&buff.id);
			//alDeleteBuffers(1,&buff.id);
		}

		// control playback and return time position
		//alGetSourcei(mSource,AL_SOURCE_STATE,&state);
		/*if (state == AL_PLAYING)
		{
			alGetSourcef(mSource,AL_SEC_OFFSET,&mTime);
			mTime+=(float) mNumPlayedSamples/mFreq;
			mTimeOffset=0;
		}
		else
		{
			mTime=(float) mNumProcessedSamples/mFreq+mTimeOffset;
			mTimeOffset+=time_increase;
		}*/

		float duration=mClip->getDuration();
		if (mTime > duration) mTime=duration;
	}

	void FmodAudioInterface::pause()
	{
		//alSourcePause(mSource);
		TheoraTimer::pause();
	}

	void FmodAudioInterface::play()
	{
		//alSourcePlay(mSource);
		TheoraTimer::play();
	}

	void FmodAudioInterface::seek(float time)
	{
		FmodBuffer buff;

		//alSourceStop(mSource);
		while (!mBufferQueue.empty())
		{
			buff=mBufferQueue.front();
			mBufferQueue.pop();
			//alSourceUnqueueBuffers(mSource,1,&buff.id);
			//alDeleteBuffers(1,&buff.id);
		}
//		int nProcessed;
//		alGetSourcei(mSource,AL_BUFFERS_PROCESSED,&nProcessed);
//		if (nProcessed != 0)
//			nProcessed=nProcessed;
		mBuffSize=0;
		mTimeOffset=0;

		mNumPlayedSamples=mNumProcessedSamples=time*mFreq;
	}

	FmodAudioInterfaceFactory::FmodAudioInterfaceFactory()
	{
		// openal init is here used only to simplify samples for this plugin
		// if you want to use this interface in your own program, you'll
		// probably want to remove the openal init/destory lines
		/*gDevice = alcOpenDevice("Generic Software");
		if (alcGetError(gDevice) != ALC_NO_ERROR) goto Fail;
		gContext = alcCreateContext(gDevice, NULL);
		if (alcGetError(gDevice) != ALC_NO_ERROR) goto Fail;
		alcMakeContextCurrent(gContext);
		if (alcGetError(gDevice) != ALC_NO_ERROR) goto Fail;

		return;
Fail:
		gDevice=NULL;
		gContext=NULL;*/
	}

	FmodAudioInterfaceFactory::~FmodAudioInterfaceFactory()
	{
		/*if (gDevice)
		{
			alcMakeContextCurrent(NULL);
			alcDestroyContext(gContext);
			alcCloseDevice(gDevice);
		}*/
	}

	FmodAudioInterface* FmodAudioInterfaceFactory::createInstance(TheoraVideoClip* owner,int nChannels,int freq)
	{
		return new FmodAudioInterface(owner,nChannels,freq);
	}
}
#include "..\..\..\include\Audio\Sound.h"


using namespace WyvernsAssault;

static struct SoundLogic 
{
	SoundTypes type;
	const char* function;
} SoundLogicList[] = {
	{SoundTypes::Intro01, "Intro"},
};


Sound::Sound(){
	//mOn=false; //is sound on?
	//mPossible=false; //is it possible to play sound?
	//mCurrentSound;//currently played sound
		
	//mResult;
	//mFmodsystem;
	//mSound;
	//mChannel;

}
Sound::Sound(SoundTypes type){
	mOn=false; //is sound on?
	mPossible=false; //is it possible to play sound?
	//mCurrentSound=null; //currently played sound
		
	//mResult;
	//mFmodsystem;
	//mSound;
	//mChannel=null;
}

Sound::~Sound()
{
}

void Sound::initialise () {
	FMOD::System * system;
	FMOD_RESULT result = FMOD::System_Create(&system);
	FMOD::Sound * sound;
	system->init(32, FMOD_INIT_NORMAL, 0);
	result = system->createSound("../media/mp3/Greensleeves.mp3",FMOD_LOOP_NORMAL,NULL,&sound);
	FMOD::Channel * channel = 0;
	result = system->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
	system->update();
	//create the sound system. If fails, sound is set to impossible
    //mResult = FMOD_System_Create(&mFmodsystem);
    //if (mResult != FMOD_OK) mPossible = false;
    //if initialise the sound system. If fails, sound is set to impossible
    //if (mPossible) mResult = FMOD_System_Init(mFmodsystem,2, FMOD_INIT_NORMAL, 0);
    //if (mResult != FMOD_OK) mPossible = false;
    //sets initial sound volume (mute)
    //if (mPossible) FMOD_Channel_SetVolume(mChannel,0.0f);
}

void Sound::setVolume (float v) {
    if (mPossible && mOn && v >= 0.0f && v <= 1.0f) {
        FMOD_Channel_SetVolume(mChannel,v);
    }
}

void Sound::load (const char * filename) {
    mCurrentSound = (char *)filename;
    if (mPossible && mOn) {
        mResult = FMOD_Sound_Release(mSound);
        mResult = FMOD_System_CreateStream(mFmodsystem,mCurrentSound, FMOD_SOFTWARE, 0, &mSound);
        if (mResult != FMOD_OK) mPossible = false;
    }
}

void Sound::unload (void) {
    if (mPossible) {
        mResult = FMOD_Sound_Release(mSound);
    }
}

void Sound::play (bool pause) {
    if (mPossible && mOn) {
        mResult = FMOD_System_PlaySound(mFmodsystem,FMOD_CHANNEL_FREE, mSound, pause, &mChannel);
        FMOD_Channel_SetMode(mChannel,FMOD_LOOP_NORMAL);
    }
}

void Sound::toggleSound (void) {
    mOn = !mOn;
    if (mOn == true) { load(mCurrentSound); play(); }
    if (mOn == false) { unload(); }
}

void Sound::setPause (bool pause) {
    FMOD_Channel_SetPaused (mChannel, pause);
}

void Sound::setSound (bool s) {
    mOn = s;
}

void Sound::togglePause (void) {
    FMOD_BOOL p;
    FMOD_Channel_GetPaused(mChannel,&p);
    FMOD_Channel_SetPaused(mChannel,!p);
}

bool Sound::getSound (void) {
    return mOn;
}

char * Sound::getSoundId(){
	return mCurrentSound;
}



#include "..\include\Audio\AudioManager.h"

using namespace WyvernsAssault;

// BEGIN SINGLETON
template<> AudioManager* Ogre::Singleton<AudioManager>::ms_Singleton = 0;
AudioManager* AudioManager::getSingletonPtr(void)
{
    return ms_Singleton;
}
AudioManager& AudioManager::getSingleton(void)
{  
    assert( ms_Singleton );  return ( *ms_Singleton );  
}
// END SINGLETON

AudioManager::AudioManager()
{
	//
	// TODO Constructor
	//
}

AudioManager::~AudioManager()
{
	//
	// TODO Destructor
	//
	finalize();
}

/** Initialize the audio manager */
void AudioManager::initialize()
{
	//
	// TODO Initialize
	//
	
	/* ETAPA DE INICIALIZACION */
    //FMOD::System     *system;
    //FMOD::Sound      *sound1;
    //FMOD::Channel    *channel = 0;
    //FMOD_RESULT       result;
    //unsigned int      version;


}

/** Finalize the audio manager */
void AudioManager::finalize()
{
	//
	// TODO Finalize
	//
	/*
       ETAPA DE FINALIZACION
    */
    //result = sound1->release();
    //ERRCHECK(result);
    //result = system->close();
    //ERRCHECK(result);
    //result = system->release();
    //ERRCHECK(result);
}

void AudioManager::loadResources()
{
	//
	// TODO Load resources
	//
	    /*
        Create a System object and initialize.
    */
    //result = FMOD::System_Create(&system);
    //ERRCHECK(result);

    //result = system->getVersion(&version);
    //ERRCHECK(result);

    //if (version < FMOD_VERSION)
    //{
    //    printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
    //    getch();
    //    return 0;
    //}

    //result = system->setOutput(FMOD_OUTPUTTYPE_ALSA);
    //ERRCHECK(result);

    //result = system->init(32, FMOD_INIT_NORMAL, 0);
    //ERRCHECK(result);

    //result = system->createSound("test.mp3", FMOD_SOFTWARE, 0, &sound1);
    //ERRCHECK(result);
    //result = sound1->setMode(FMOD_LOOP_OFF);
    //ERRCHECK(result);

}

void AudioManager::unloadResources()
{
	//
	// TODO Unload resources
	//

}

//void AudioManager::playAudio(String name){}

//void AudioManager::stopAudio(){}

// --------------------------------
// Lua Audio Lib
// --------------------------------
LUA_BEGIN_BINDING(AudioManager::audiolib)
LUA_BIND("playSound", AudioManager::playSound)
LUA_END_BINDING()

int AudioManager::playSound(lua_State *L)
{
	/*
        ETAPA DE REPRODUCCION
    */
    //result = system->playSound(FMOD_CHANNEL_FREE, sound1, 0, &channel);
    //ERRCHECK(result);
    //bool keep_looping = true;
    /*EL BUCLE ES NECESARIO PARA INCREMENTAR EL APUNTADOR SOBRE EL ARCHIVO Y CONTINUAR LA REPRODUCCION */
    //do
    //{
    //    system->update();
    //    channel->isPlaying(&keep_looping);
    //} while (keep_looping);

    //printf("\n");

	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 1;
}
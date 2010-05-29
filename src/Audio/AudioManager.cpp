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
    //mSystem=null;
    //mSound=null;
    //mChannel= 0;


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
    //mResult = mSound->release();
    //ERRCHECK(mResult);
    //mResult = mSystem->close();
    //ERRCHECK(result);
    //mResult = mSystem->release();
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
    //mResult = FMOD::System_Create(&mSystem);
    //ERRCHECK(result);

    //mResult = mSystem->getVersion(&version);
    //ERRCHECK(mResult);

    //mResult = mSystem->setOutput(FMOD_OUTPUTTYPE_ALSA);
    //ERRCHECK(mResult);

    //mResult = mSystem->init(32, FMOD_INIT_NORMAL, 0);
    //ERRCHECK(mResult);

    //mResult = mSystem->createSound("test.mp3", FMOD_SOFTWARE, 0, &mSound);
    //ERRCHECK(mResult);
    //mResult = mSound->setMode(FMOD_LOOP_OFF);
    //ERRCHECK(mResult);
	
}

void AudioManager::unloadResources()
{
	//
	// TODO Unload resources
	//

}

void AudioManager::playSound(int SoundId)
{
	/*
        ETAPA DE REPRODUCCION
    */
    //mResult = mSystem->playSound(FMOD_CHANNEL_FREE, mSound, 0, &mChannel);
    //ERRCHECK(mResult);
    //bool keep_looping = true;
    /*EL BUCLE ES NECESARIO PARA INCREMENTAR EL APUNTADOR SOBRE EL ARCHIVO Y CONTINUAR LA REPRODUCCION */
    //do
    //{
    //    mSystem->update();
    //    mChannel->isPlaying(&keep_looping);
    //} while (keep_looping);

    //printf("\n");

}

void AudioManager::stopSound(int SoundId)
{
}


SoundPtr AudioManager::createSound(SoundTypes type)
{
	Ogre::String audio;

	/*switch(type)
	{
	case Naked:
		mesh = Ogre::String("naked.mesh");
		break;
	case Wizard:
		mesh = Ogre::String("wizard.mesh");
		break;
	default:
		mesh = Ogre::String("naked.mesh");
		break;
	}*/

	Ogre::String name = createUniqueId();

	return createSound(type, name);
}

SoundPtr AudioManager::createSound(SoundTypes type, Ogre::String name)
{
	// Sound name == Effect Name!
	SoundPtr sound = SoundPtr(new Sound(type));
	mSoundList.push_back(sound);
	mSoundMap[name] = sound;

	//mSound++;

	return sound;
}

Ogre::String AudioManager::createUniqueId(){
	Ogre::String name;
	return name;
}

// --------------
// Event handlers
// --------------
void AudioManager::registerHandlers()
{
	boost::shared_ptr<AudioManager> this_ = shared_from_this();

	registerHandler(EventHandlerPtr(new EventHandler<AudioManager,EnemyHitEvent>(this_,&AudioManager::handleEnemyHitEvent)),EventTypes::EnemyHit);
}

void AudioManager::unregisterHandlers()
{

}

void AudioManager::handleEnemyHitEvent(EnemyHitEventPtr evt)
{
	EnemyPtr enemy = evt->getEnemy();
	PlayerPtr player = evt->getPlayer();

	// The player has just hit the enemy
	// TODO : play sound
}

// --------------------------------
// Lua Audio Lib
// --------------------------------
LUA_BEGIN_BINDING(AudioManager::audiolib)
LUA_BIND("playSound", AudioManager::luaPlaySound)
LUA_END_BINDING()

int AudioManager::luaPlaySound(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 1;
}
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
}

/** Finalize the audio manager */
void AudioManager::finalize()
{
	//
	// TODO Finalize
	//
}

void AudioManager::loadResources()
{
	//
	// TODO Load resources
	//
}

void AudioManager::unloadResources()
{
	//
	// TODO Unload resources
	//
}

// --------------------------------
// Lua Audio Lib
// --------------------------------
LUA_BEGIN_BINDING(AudioManager::audiolib)
LUA_BIND("playSound", AudioManager::playSound)
LUA_END_BINDING()

int AudioManager::playSound(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 1;
}
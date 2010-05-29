#include "..\include\Audio\AudioManager.h"
#include "..\include\Audio\Sound.h"
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
	mId=0;
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
	//Sound s1, s2, s3;
	//s1.initialise();
	//s1.load("..\data\sound\Music\sound.mp3");
	//s1.play();
	//this->addSound(&s1);

	//s2.initialise();
	//s2.load("..\data\sound\Music\sound2.mp3");

	//this->addSound(&s2);
	
	//s3.initialise();
	//s3.load("..\data\sound\Music\sound3.mp3");
	
	//this->addSound(&s3);
	
	Sound s1;
	s1.initialise();
	//s1.load("..\..\data\sound\Music\sound2.mp3");
	//s1.play();
	
}

void AudioManager::unloadResources()
{
	//
	// TODO Unload resources
	//

}

void AudioManager::addSound(Sound* s)
{
	Ogre::String name = createUniqueId();
	
	SoundPtr sound = SoundPtr(s);
	mSoundList.push_back(sound);
	mSoundMap[name] = sound;

}
Ogre::String AudioManager::createUniqueId()
{
	Ogre::StringStream countStrStr;
	countStrStr << mId;

	Ogre::String uniqueId = "Audio_" + countStrStr.str();

	mId++;

	return uniqueId;
}
void AudioManager::playSound(Ogre::String sound)
{
	SoundPtr s = mSoundMap[sound];
	if (s->getSound()==false) s->setSound(true);
	s->play();
}

void AudioManager::stopSound(Ogre::String sound)
{
	SoundPtr s = mSoundMap[sound];
	s->unload();
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
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
: mSoundTrackChannelIndex(INVALID_SOUND_CHANNEL)
, mInitialized(false)
{
	mSystem = NULL;
	mPrevListenerPosition = Vector3(0, 0, 0);

	mSoundInstanceVector = new SoundInstanceVector();

	// Initialized to zero, but pre-incremented in GetNextSoundInstanceIndex(), so vector starts at one.
	mNextSoundInstanceIndex = 0;

	// Start off with INITIAL_VECTOR_SIZE soundInstanceVectors.  It can grow from here.
	mSoundInstanceVector->resize(INITIAL_VECTOR_SIZE);
	for (int vectorIndex = 0; vectorIndex < INITIAL_VECTOR_SIZE; vectorIndex++)
	{
		mSoundInstanceVector->at(vectorIndex) = new SoundInstance;
		mSoundInstanceVector->at(vectorIndex)->clear();
	}

	for (int channelIndex = 0; channelIndex < MAX_SOUND_CHANNELS; channelIndex++)
		mChannelArray[channelIndex].clear();
}


AudioManager::~AudioManager()
{
	if(mInitialized)
	{
		finalize();
	}
}

/** Initialize the audio manager */
bool AudioManager::initialize()
{
	FMOD_RESULT result;

	// Create the main mSystem object.
	result = FMOD::System_Create(&mSystem);
	if (result != FMOD_OK)
		OGRE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "FMOD error! (" + StringConverter::toString(result) + "): " + FMOD_ErrorString(result), "AudioManager::Initialize");

	result = mSystem->init(MAX_SOUND_CHANNELS, FMOD_INIT_NORMAL, 0);	// Initialize FMOD.
	if (result != FMOD_OK)
		OGRE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "FMOD error! (" + StringConverter::toString(result) + "): " + FMOD_ErrorString(result), "AudioManager::Initialize");

	mSystem->set3DSettings(DOPPLER_SCALE, DISTANCE_FACTOR, ROLLOFF_SCALE);

	result = mSystem->setFileSystem(&AudioManager::fmodFileOpenCallback, &AudioManager::fmodFileCloseCallback, &AudioManager::fmodFileReadCallback, &AudioManager::fmodFileSeekCallback, 2048);
	if (result != FMOD_OK)
		OGRE_EXCEPT(Exception::ERR_INTERNAL_ERROR, "FMOD error! (" + StringConverter::toString(result) + "): " + FMOD_ErrorString(result), "AudioManager::Initialize");

	Ogre::LogManager::getSingleton().logMessage("AudioManager Initialized");

	mInitialized = true;

	return mInitialized;
}

/** Finalize the audio manager */
void AudioManager::finalize()
{
	for (int vectorIndex = 0; vectorIndex < (int)mSoundInstanceVector->capacity(); vectorIndex++)
	{
		mSoundInstanceVector->at(vectorIndex)->fileName.clear();
		//      mSoundInstanceVector->at(vectorIndex)->streamPtr->close();
		delete mSoundInstanceVector->at(vectorIndex);
	}
	delete mSoundInstanceVector;

	if (mSystem)
		mSystem->release();

	mInitialized = false;
}

void AudioManager::loadResources()
{
	//
	// Load resources
	//

	// Sound FX	
	createStream( String("ArcherAttack01.wav"));
	createStream( String("ArcherAttack02.wav"));
	createStream( String("BatteringRamAttack01.wav"));
	createStream( String("BatteringRamAttack02.wav"));
	createStream( String("BatteringRamAttack03.wav"));
	createStream( String("BatteringRamHit01.wav"));
	createStream( String("BatteringRamHit02.wav"));
	createStream( String("BatteringRamDie01.wav"));
	createStream( String("ChickenDie01.wav"));
	createStream( String("CowDie01.wav"));
	createStream( String("EnemyAttack01.wav"));
	createStream( String("EnemyAttack02.wav"));
	createStream( String("EnemyAttack03.wav"));
	createStream( String("EnemyDie01.wav"));
	createStream( String("EnemyDie02.wav"));
	createStream( String("EnemyHit01.wav"));
	createStream( String("EnemyHit02.wav"));
	createStream( String("EnemyHit03.wav"));
	createStream( String("NakedDie01.wav"));
	createStream( String("NakedHit01.wav"));
	createStream( String("NakedHit02.wav"));
	createStream( String("PeasantHit01.wav"));
	createStream( String("PeasantHit02.wav"));
	createStream( String("PeasantDie01.wav"));
	createStream( String("WizardAttack01.wav"));
	createStream( String("WizardAttack02.wav"));
	createStream( String("WizardDie01.wav"));
	createStream( String("WizardHit01.wav"));
	createStream( String("WizardHit02.wav"));
	createStream( String("WomanHit01.wav"));
	createStream( String("WomanHit02.wav"));
	createStream( String("WomanDie02.wav"));
	createStream( String("WomanGive01.wav"));
	createStream( String("WomanGive02.wav"));
	createStream( String("WomanGive03.wav"));		

	createStream( String("Flame01.wav"));
	createStream( String("PlayerAttack01.wav"));
	createStream( String("PlayerAttack02.wav"));
	createStream( String("PlayerAttack03.wav"));
	createStream( String("PlayerDie01.wav"));
	createStream( String("PlayerHit01.wav"));
	createStream( String("PlayerHit02.wav"));
	createStream( String("PlayerYell01.wav"));
	createStream( String("PlayerYell02.wav"));
	createStream( String("PlayerYell03.wav"));
	createStream( String("Speech_Die.wav"));	
	createStream( String("Speech_IveRuledHereForEaons.wav"));
	createStream( String("Speech_KneelBeforeMeHuman.wav"));
	createStream( String("Speech_YourSkillsArePathetic.wav"));

	createStream( String("ObjectHit01.wav"));
	createStream( String("ObjectHit02.wav"));
	createStream( String("ObjectHit03.wav"));
	createStream( String("ObjectHit04.wav"));
	createStream( String("ObjectDie01.wav"));
	createStream( String("ObjectDie02.wav"));
	createStream( String("ObjectDie03.wav"));
	createStream( String("ObjectDie04.wav"));
	createStream( String("ObjectFireHit01.wav"));
	createStream( String("ObjectFireHit02.wav"));

	createStream( String("Item01.wav"));
	createStream( String("Drop01.wav"));

	createStream( String("BloodSplat01.wav"));
	createStream( String("BloodSplat02.wav"));
	createStream( String("BloodSplat03.wav"));
	createStream( String("ChangeOption.wav"));
	createStream( String("Select.wav"));

	// Musics
	createLoopedStream(String("soft_track.mp3"));
	createLoopedStream(String("hard_track.mp3"));
	createLoopedStream(String("main_track.mp3"));
	createLoopedStream(String("intro.mp3"));

	Debug::Out("AudioManager : Resources loaded");
}

void AudioManager::unloadResources()
{
	//
	// TODO Unload resources
	//
}


// Frame started! (EACH)
void AudioManager::update(Ogre::Vector3 position, Ogre::Quaternion orientation, Ogre::Real timeElapsed)
{
	int            channelIndex;
	FMOD::Channel *nextChannel;
	FMOD_VECTOR    listenerPosition;
	FMOD_VECTOR    listenerForward;
	FMOD_VECTOR    listenerUp;
	FMOD_VECTOR    listenerVelocity;
	Ogre::Vector3  vectorVelocity;
	Ogre::Vector3  vectorForward;
	Ogre::Vector3  vectorUp;

	if (timeElapsed > 0)
		vectorVelocity = (position - mPrevListenerPosition) / timeElapsed;
	else
		vectorVelocity = Vector3(0, 0, 0);

	vectorForward = orientation.zAxis();
	vectorForward.normalise();

	vectorUp = orientation.yAxis();
	vectorUp.normalise();

	listenerPosition.x = position.x;
	listenerPosition.y = position.y;
	listenerPosition.z = position.z;

	listenerForward.x = vectorForward.x;
	listenerForward.y = vectorForward.y;
	listenerForward.z = vectorForward.z;

	listenerUp.x = vectorUp.x;
	listenerUp.y = vectorUp.y;
	listenerUp.z = vectorUp.z;

	listenerVelocity.x = vectorVelocity.x;
	listenerVelocity.y = vectorVelocity.y;
	listenerVelocity.z = vectorVelocity.z;

	// update 'ears'
	mSystem->set3DListenerAttributes(0, &listenerPosition, &listenerVelocity, &listenerForward, &listenerUp);
	mSystem->update();

	mPrevListenerPosition = position;

	for (channelIndex = 0; channelIndex < MAX_SOUND_CHANNELS; channelIndex++)
	{
		mSystem->getChannel(channelIndex, &nextChannel);
		if (timeElapsed > 0)
			vectorVelocity = (mChannelArray[channelIndex].position - mChannelArray[channelIndex].prevPosition) / timeElapsed;
		else
			vectorVelocity = Vector3(0, 0, 0);

		listenerPosition.x = mChannelArray[channelIndex].position.x;
		listenerPosition.y = mChannelArray[channelIndex].position.y;
		listenerPosition.z = mChannelArray[channelIndex].position.z;

		listenerVelocity.x = vectorVelocity.x;
		listenerVelocity.y = vectorVelocity.y;
		listenerVelocity.z = vectorVelocity.z;

		nextChannel->set3DAttributes(&listenerPosition, &listenerVelocity);
		mChannelArray[channelIndex].prevPosition = mChannelArray[channelIndex].position;
	}
}

int AudioManager::createStream(String &fileName)
{
	return createSound(fileName, SOUND_TYPE_2D_SOUND);
}

int AudioManager::createSound(String &fileName)
{
	return createSound(fileName, SOUND_TYPE_3D_SOUND);
}

int AudioManager::createLoopedSound(String &fileName)
{
	return createSound(fileName, SOUND_TYPE_3D_SOUND_LOOPED);
}

int AudioManager::createLoopedStream(String &fileName)
{
	return createSound(fileName, SOUND_TYPE_2D_SOUND_LOOPED);
}

// fileName is actually a pointer to a Sound, passed in from createSound().
FMOD_RESULT AudioManager::fmodFileOpenCallback(const char *fileName, int unicode, unsigned int *filesize, void **handle, void **userdata)
{
	SoundInstance *soundInstance;

	assert(fileName);

	soundInstance = (SoundInstance *)fileName;
	assert(soundInstance->fileArchive);

	*handle = (void *)soundInstance;
	*userdata = NULL;

	soundInstance->streamPtr = soundInstance->fileArchive->open(soundInstance->fileName);
	if (soundInstance->streamPtr.isNull())
	{
		*filesize = 0;
		return FMOD_ERR_FILE_NOTFOUND;
	}

	*filesize = (unsigned int)soundInstance->streamPtr->size();
	return FMOD_OK;
}


FMOD_RESULT AudioManager::fmodFileCloseCallback(void *handle, void *userdata)
{
	return FMOD_OK;
}


FMOD_RESULT AudioManager::fmodFileReadCallback(void *handle, void *buffer, unsigned int sizeBytes, unsigned int *bytesRead, void *userData)
{
	SoundInstance *soundInstance;

	soundInstance = (SoundInstance *)handle;
	*bytesRead = (unsigned int)soundInstance->streamPtr->read(buffer, (size_t)sizeBytes);
	if (*bytesRead == 0)
		return FMOD_ERR_FILE_EOF;

	return FMOD_OK;
}


FMOD_RESULT AudioManager::fmodFileSeekCallback(void *handle, unsigned int pos, void *userdata)
{
	SoundInstance *soundInstance;

	soundInstance = (SoundInstance *)handle;
	soundInstance->streamPtr->seek((size_t)pos);
	return FMOD_OK;
}

int AudioManager::createSound(String &fileName, SOUND_TYPE soundType)
{
	Archive *      fileArchive;
	FMOD_RESULT    result;
	FMOD::Sound *  sound;
	String         fullPathName;
	SoundInstance	*newSoundInstance;

	int soundIndex;
	soundIndex = findSound(fileName, soundType);
	if (soundIndex != INVALID_SOUND_INDEX)
		return soundIndex;

	fullPathName = fileName;
	FileLocator * fileLocator = (FileLocator * )ResourceGroupManager::getSingletonPtr();
	fileArchive = fileLocator->find(fullPathName);
	if (!fileArchive)
	{
		Ogre::LogManager::getSingleton().logMessage("AudioManager::CreateSound could not find sound '" + fileName + "'");
		return INVALID_SOUND_INDEX;
	}

	incrementNextSoundInstanceIndex();
	newSoundInstance = mSoundInstanceVector->at(mNextSoundInstanceIndex);
	newSoundInstance->fileName = fileName;
	newSoundInstance->fileArchive = fileArchive;
	newSoundInstance->soundType = soundType;

	switch (soundType)
	{
	case SOUND_TYPE_3D_SOUND:
		{
			result = mSystem->createSound((const char *)newSoundInstance, FMOD_3D | FMOD_HARDWARE, 0, &sound);
			break;
		}

	case SOUND_TYPE_3D_SOUND_LOOPED:
		{
			result = mSystem->createSound((const char *)newSoundInstance, FMOD_LOOP_NORMAL | FMOD_3D | FMOD_HARDWARE, 0, &sound);
			break;
		}

	case SOUND_TYPE_2D_SOUND:
		{
			result = mSystem->createStream((const char *)newSoundInstance, FMOD_DEFAULT, 0, &sound);
			break;
		}

	case SOUND_TYPE_2D_SOUND_LOOPED:
		{
			result = mSystem->createStream((const char *)newSoundInstance, FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE, 0, &sound);
			break;
		}

	default:
		{
			Ogre::LogManager::getSingleton().logMessage("AudioManager::CreateSound could not load sound '" + fileName + "' (invalid soundType)");
			return INVALID_SOUND_INDEX;
		}
	}

	if (result != FMOD_OK)
	{
		Ogre::LogManager::getSingleton().logMessage("AudioManager::CreateSound could not load sound '" + fileName + "'  FMOD Error:" + FMOD_ErrorString(result));
		return INVALID_SOUND_INDEX;
	}

	newSoundInstance->fmodSound = sound;

	//
	// Add the relationship between sound name and sound idx to the internal quick map
	//
	mSoundNameToIdMap[fileName] = mNextSoundInstanceIndex;

	return mNextSoundInstanceIndex;
}

void AudioManager::incrementNextSoundInstanceIndex(void)
{
	int oldVectorCapacity;

	oldVectorCapacity = (int)mSoundInstanceVector->capacity();
	mNextSoundInstanceIndex += 1;
	if (mNextSoundInstanceIndex < oldVectorCapacity)
		return;

	int vectorIndex;
	SoundInstanceVector *newSoundInstanceVector;

	// Create a new, larger SoundInstanceVector
	newSoundInstanceVector = new SoundInstanceVector;
	newSoundInstanceVector->resize(oldVectorCapacity + INCREASE_VECTOR_SIZE);

	// Check Ogre.log for these messages, and change INITIAL_VECTOR_SIZE to be a more appropriate value
	Ogre::LogManager::getSingleton().logMessage("AudioManager::IncrementNextSoundInstanceIndex increasing size of mSoundInstanceVector to " + 
		StringConverter::toString(oldVectorCapacity + INCREASE_VECTOR_SIZE));

	// Copy values from old vector to new
	for (vectorIndex = 0; vectorIndex < oldVectorCapacity; vectorIndex++)
		newSoundInstanceVector->at(vectorIndex) = mSoundInstanceVector->at(vectorIndex);

	int newVectorCapacity;

	newVectorCapacity = (int)newSoundInstanceVector->capacity();
	// Clear out the rest of the new vector
	while (vectorIndex < newVectorCapacity)
	{
		newSoundInstanceVector->at(vectorIndex) = new SoundInstance;
		newSoundInstanceVector->at(vectorIndex)->clear();
		vectorIndex++;
	}

	// Clear out the old vector and point to the new one.
	mSoundInstanceVector->clear();
	delete(mSoundInstanceVector);
	mSoundInstanceVector = newSoundInstanceVector;
}

void AudioManager::playSound(String soundName, Ogre::Vector3 soundPosition, int *channelIndex)
{
	playSound(mSoundNameToIdMap[soundName], soundPosition, channelIndex);
}

void AudioManager::playSound(String soundName, int channelIndex)
{
	playSound(mSoundNameToIdMap[soundName], Vector3::ZERO, &channelIndex);
}

void AudioManager::playSound(int soundIndex, Ogre::Vector3 soundPosition, int *channelIndex)
{
	int            channelIndexTemp;
	FMOD_RESULT    result;
	FMOD_VECTOR    initialPosition;
	FMOD::Channel *channel;
	SoundInstance *soundInstance;

	if (soundIndex == INVALID_SOUND_INDEX)
		return;

	if (channelIndex)
		channelIndexTemp = *channelIndex;
	else
		channelIndexTemp = INVALID_SOUND_CHANNEL;

	assert((soundIndex > 0) && (soundIndex < (int)mSoundInstanceVector->capacity()));

	// If the channelIndex already has a sound assigned to it, test if it's the same sceneNode.
	if ((channelIndexTemp != INVALID_SOUND_CHANNEL))
	{
		result = mSystem->getChannel(channelIndexTemp, &channel);
		if (result == FMOD_OK)
		{
			bool isPlaying;

			result = channel->isPlaying(&isPlaying);
			if ((result == FMOD_OK) && (isPlaying == true))
				return;  // Already playing this sound attached to this node.
		}
	}

	soundInstance = mSoundInstanceVector->at(soundIndex);
	// Start the sound paused
	result = mSystem->playSound(FMOD_CHANNEL_FREE, soundInstance->fmodSound, true, &channel);
	if (result != FMOD_OK)
	{
		Ogre::LogManager::getSingleton().logMessage(String("AudioManager::PlaySound could not play sound  FMOD Error:") + FMOD_ErrorString(result));
		if (channelIndex)
			*channelIndex = INVALID_SOUND_CHANNEL;
		return;
	}

	channel->getIndex(&channelIndexTemp);
	mChannelArray[channelIndexTemp].position = soundPosition;
	//mSceneNodeToChannelMap[soundNode] = channelIndexTemp;

	//if (soundNode)
	//{
		mChannelArray[channelIndexTemp].prevPosition = soundPosition;

		initialPosition.x = soundPosition.x;
		initialPosition.y = soundPosition.y;
		initialPosition.z = soundPosition.z;
		channel->set3DAttributes(&initialPosition, NULL);
	//}

	result = channel->setVolume(1.0);
	// This is where the sound really starts.
	result = channel->setPaused(false);

	if (channelIndex)
		*channelIndex = channelIndexTemp;
}

void AudioManager::playSoundTrack(String name)
{
	stopSound(&mSoundTrackChannelIndex);
	playSound(name,Vector3::ZERO,&mSoundTrackChannelIndex);

	FMOD::Channel *channel = 0;
	mSystem->getChannel(mSoundTrackChannelIndex, &channel);
	
	if(channel)
		channel->setVolume(0.5);
}

void AudioManager::stopSoundTrack()
{
	stopSound(&mSoundTrackChannelIndex);
}

SoundInstance *AudioManager::getSoundInstance(int soundIndex)
{
	return mSoundInstanceVector->at(soundIndex);
}

FMOD::Channel *AudioManager::getFMODChannel(int channelIndex)
{
	if (channelIndex == INVALID_SOUND_CHANNEL)
		return NULL;

	FMOD::Channel *soundChannel;

	assert((channelIndex > 0) && (channelIndex < MAX_SOUND_CHANNELS));

	mSystem->getChannel(channelIndex, &soundChannel);
	return soundChannel;
}

void AudioManager::set3DMinMaxDistance(int channelIndex, float minDistance, float maxDistance)
{
	FMOD_RESULT    result;
	FMOD::Channel *channel;

	if (channelIndex == INVALID_SOUND_CHANNEL)
		return;

	result = mSystem->getChannel(channelIndex, &channel);
	if (result == FMOD_OK)
		channel->set3DMinMaxDistance(minDistance, maxDistance);
}



void AudioManager::stopAllSounds(void)
{
	int            channelIndex;
	FMOD_RESULT    result;
	FMOD::Channel *nextChannel;

	for (channelIndex = 0; channelIndex < MAX_SOUND_CHANNELS; channelIndex++)
	{
		result = mSystem->getChannel(channelIndex, &nextChannel);
		if ((result == FMOD_OK) && (nextChannel != NULL))
			nextChannel->stop();
		mChannelArray[channelIndex].clear();
	}
}


void AudioManager::stopSound(int *channelIndex)
{
	if (*channelIndex == INVALID_SOUND_CHANNEL)
		return;

	FMOD::Channel *soundChannel;

	assert((*channelIndex > 0) && (*channelIndex < MAX_SOUND_CHANNELS));

	mSystem->getChannel(*channelIndex, &soundChannel);
	soundChannel->stop();

	mChannelArray[*channelIndex].clear();
	*channelIndex = INVALID_SOUND_CHANNEL;
}



int AudioManager::findSound(String &fileName, SOUND_TYPE soundType)
{
	int            vectorIndex;
	int            vectorCapacity;
	SoundInstance *nextSoundInstance;

	vectorCapacity = (int)mSoundInstanceVector->capacity();
	for (vectorIndex = 0; vectorIndex < vectorCapacity; vectorIndex++)
	{
		nextSoundInstance = mSoundInstanceVector->at(vectorIndex);
		if ((soundType == nextSoundInstance->soundType) && (fileName == nextSoundInstance->fileName))
			//      if ((soundType == nextSoundInstance->soundType) && (fileName == nextSoundInstance->fileArchive->getName()))
			return vectorIndex;
	}

	return INVALID_SOUND_INDEX;
}



float AudioManager::getSoundLength(int soundIndex)
{
	if (soundIndex == INVALID_SOUND_INDEX)
		return 0.0;

	assert((soundIndex > 0) && (soundIndex < (int)mSoundInstanceVector->capacity()));

	unsigned int   soundLength;   // length in milliseconds
	FMOD_RESULT    result;
	SoundInstance *soundInstance;

	soundInstance = mSoundInstanceVector->at(soundIndex);
	if (soundInstance)
	{
		result = soundInstance->fmodSound->getLength(&soundLength, FMOD_TIMEUNIT_MS);
		if (result != FMOD_OK)
		{
			Ogre::LogManager::getSingleton().logMessage(String("AudioManager::GetSoundLength could not get length  FMOD Error:") + FMOD_ErrorString(result));
			return 0.0;
		}
	}
	else
	{
		Ogre::LogManager::getSingleton().logMessage(String("AudioManager::GetSoundLength could not find soundInstance"));
		return 0.0;
	}

	return (float)soundLength / 1000.0f;
}


// --------------
// Event handlers
// --------------
EVENTS_BEGIN_REGISTER_HANDLERS(AudioManager)
	EVENTS_REGISTER_HANDLER(AudioManager, PlayerAttack);
	EVENTS_REGISTER_HANDLER(AudioManager, PlayerAttackSpecial);
	EVENTS_REGISTER_HANDLER(AudioManager, PlayerHit);
	EVENTS_REGISTER_HANDLER(AudioManager, PlayerKilled);
	EVENTS_REGISTER_HANDLER(AudioManager, EnemyAttack);
	EVENTS_REGISTER_HANDLER(AudioManager, EnemyHit);
	EVENTS_REGISTER_HANDLER(AudioManager, EnemyKilled);
	EVENTS_REGISTER_HANDLER(AudioManager, EnemyCreateItem);
	EVENTS_REGISTER_HANDLER(AudioManager, ItemCatch);
	EVENTS_REGISTER_HANDLER(AudioManager, ProjectileFire);
	EVENTS_REGISTER_HANDLER(AudioManager, ProjectileHit);
	EVENTS_REGISTER_HANDLER(AudioManager, ObjectHit);
	EVENTS_REGISTER_HANDLER(AudioManager, ObjectKilled);
EVENTS_END_REGISTER_HANDLERS()

EVENTS_BEGIN_UNREGISTER_HANDLERS(AudioManager)
	EVENTS_UNREGISTER_HANDLER(AudioManager, PlayerAttack);
	EVENTS_UNREGISTER_HANDLER(AudioManager, PlayerAttackSpecial);
	EVENTS_UNREGISTER_HANDLER(AudioManager, PlayerHit);
	EVENTS_UNREGISTER_HANDLER(AudioManager, PlayerKilled);
	EVENTS_UNREGISTER_HANDLER(AudioManager, EnemyAttack);
	EVENTS_UNREGISTER_HANDLER(AudioManager, EnemyHit);
	EVENTS_UNREGISTER_HANDLER(AudioManager, EnemyKilled);	
	EVENTS_UNREGISTER_HANDLER(AudioManager, EnemyCreateItem);
	EVENTS_UNREGISTER_HANDLER(AudioManager, ItemCatch);
	EVENTS_UNREGISTER_HANDLER(AudioManager, ProjectileFire);
	EVENTS_UNREGISTER_HANDLER(AudioManager, ProjectileHit);
	EVENTS_UNREGISTER_HANDLER(AudioManager, ObjectHit);
	EVENTS_UNREGISTER_HANDLER(AudioManager, ObjectKilled);
EVENTS_END_UNREGISTER_HANDLERS()


EVENTS_DEFINE_HANDLER(AudioManager, PlayerAttack)
{
	Debug::Out("AudioManager : handlePlayerAttackEvent");

	PlayerPtr player = evt->getPlayer();

	int channelIndex = -1;

	SceneNode* sceneNode = player->_getSceneNode();

	// The player has just attacked
	if( player->wichAttack() == 1 )
		playSound("PlayerAttack01.wav",sceneNode->_getDerivedPosition(),&channelIndex);
	else if( player->wichAttack() == 2 )
 		playSound("PlayerAttack02.wav",sceneNode->_getDerivedPosition(),&channelIndex);
	else if( player->wichAttack() == 3 )
		playSound("PlayerAttack03.wav",sceneNode->_getDerivedPosition(),&channelIndex);
}

EVENTS_DEFINE_HANDLER(AudioManager, PlayerAttackSpecial)
{
	Debug::Out("AudioManager : handlePlayerAttackSpecialEvent");

	PlayerPtr player = evt->getPlayer();

	int channelIndex = -1;

	SceneNode* sceneNode = player->_getSceneNode();

	// The player has just used special attack
	playSound("Flame01.wav",sceneNode->_getDerivedPosition(),&channelIndex);
}

EVENTS_DEFINE_HANDLER(AudioManager, PlayerHit)
{
	Debug::Out("AudioManager : handlePlayerHitEvent");

	PlayerPtr player = evt->getPlayer();

	int channelIndex = -1;

	SceneNode* sceneNode = player->_getSceneNode();

	// The enemy has just hit the player
	if( (rand()%2) == 0)
		playSound("PlayerHit01.wav",sceneNode->_getDerivedPosition(),&channelIndex);
	else
		playSound("PlayerHit02.wav",sceneNode->_getDerivedPosition(),&channelIndex);
}

EVENTS_DEFINE_HANDLER(AudioManager, PlayerKilled)
{
	Debug::Out("AudioManager : handlePlayerKilledEvent");

	PlayerPtr player = evt->getPlayer();

	int channelIndex = -1;

	SceneNode* sceneNode = player->_getSceneNode();

	// The enemy has kill player	
	playSound("PlayerDie01.wav",sceneNode->_getDerivedPosition(),&channelIndex);
}

EVENTS_DEFINE_HANDLER(AudioManager, EnemyAttack)
{
	Debug::Out("AudioManager : handleEnemyAttackEvent");

	EnemyPtr enemy = evt->getEnemy();

	int channelIndex = -1;

	SceneNode* sceneNode = enemy->_getSceneNode();

	// The enemy has just attack
	if ( enemy->getEnemyType() == Enemy::EnemyTypes::Knight )
	{
		int sound = rand()%3;

		if( sound == 0 )
			playSound("EnemyAttack01.wav",sceneNode->_getDerivedPosition(),&channelIndex);
		else if( sound == 1 )
			playSound("EnemyAttack02.wav",sceneNode->_getDerivedPosition(),&channelIndex);
		else
			playSound("EnemyAttack03.wav",sceneNode->_getDerivedPosition(),&channelIndex);
	}
	else if ( enemy->getEnemyType() == Enemy::EnemyTypes::BatteringRam )
	{
		int sound = rand()%3;

		if( sound == 0 )
			playSound("BatteringRamAttack01.wav",sceneNode->_getDerivedPosition(),&channelIndex);
		else if( sound == 1 )
			playSound("BatteringRamAttack02.wav",sceneNode->_getDerivedPosition(),&channelIndex);
		else
			playSound("BatteringRamAttack03.wav",sceneNode->_getDerivedPosition(),&channelIndex);
	}
}

EVENTS_DEFINE_HANDLER(AudioManager, EnemyHit)
{
	Debug::Out("AudioManager : handleEnemyHitEvent");

	EnemyPtr enemy = evt->getEnemy();

	int channelIndex = -1;

	SceneNode* sceneNode = enemy->_getSceneNode();

	// The player has just hit the enemy
	if( enemy->getEnemyType() == Enemy::EnemyTypes::Wizard )
	{
		if( (rand()%2) == 0 )
			playSound("WizardHit01.wav",sceneNode->_getDerivedPosition(),&channelIndex);
		else 
			playSound("WizardHit02.wav",sceneNode->_getDerivedPosition(),&channelIndex);
	}
	else if( enemy->getEnemyType() == Enemy::EnemyTypes::Naked )
	{
		if( (rand()%2) == 0 )
			playSound("NakedHit01.wav",sceneNode->_getDerivedPosition(),&channelIndex);
		else 
			playSound("NakedHit02.wav",sceneNode->_getDerivedPosition(),&channelIndex);
	}
	else if( enemy->getEnemyType() == Enemy::EnemyTypes::Chicken )
	{
		playSound("ChickenDie01.wav",sceneNode->_getDerivedPosition(),&channelIndex);
	}
	else if( enemy->getEnemyType() == Enemy::EnemyTypes::Cow )
	{
		playSound("CowDie01.wav",sceneNode->_getDerivedPosition(),&channelIndex);
	}
	else if( enemy->getEnemyType() == Enemy::EnemyTypes::Woman )
	{
		if( (rand()%2) == 0 )
			playSound("WomanHit01.wav",sceneNode->_getDerivedPosition(),&channelIndex);
		else 
			playSound("WomanHit02.wav",sceneNode->_getDerivedPosition(),&channelIndex);
	}
	else if( enemy->getEnemyType() == Enemy::EnemyTypes::Peasant )
	{
		if( (rand()%2) == 0 )
			playSound("PeasantHit01.wav",sceneNode->_getDerivedPosition(),&channelIndex);
		else 
			playSound("PeasantHit02.wav",sceneNode->_getDerivedPosition(),&channelIndex);
	}
	else if( enemy->getEnemyType() == Enemy::EnemyTypes::BatteringRam )
	{
		if( (rand()%2) == 0 )
			playSound("BatteringRamHit01.wav",sceneNode->_getDerivedPosition(),&channelIndex);
		else 
			playSound("BatteringRamHit02.wav",sceneNode->_getDerivedPosition(),&channelIndex);
	}
	else if( enemy->getEnemyType() == Enemy::EnemyTypes::Knight || enemy->getEnemyType() == Enemy::EnemyTypes::Archer)
	{
		int sound = rand()%3;

		if( sound == 0 )
			playSound("EnemyHit01.wav",sceneNode->_getDerivedPosition(),&channelIndex);
		else if( sound == 1 )
			playSound("EnemyHit02.wav",sceneNode->_getDerivedPosition(),&channelIndex);
		else
			playSound("EnemyHit03.wav",sceneNode->_getDerivedPosition(),&channelIndex);
	}
}


EVENTS_DEFINE_HANDLER(AudioManager, EnemyKilled)
{
	Debug::Out("AudioManager : handleEnemyKilledEvent");

	EnemyPtr enemy = evt->getEnemy();
	PlayerPtr player = evt->getPlayer();

	int channelIndex = -1;

	SceneNode* sceneNode = enemy->_getSceneNode();

	// The player has just kill the enemy
	if( enemy->getEnemyType() == Enemy::EnemyTypes::Wizard )
	{
		playSound("WizardDie01.wav",sceneNode->_getDerivedPosition(),&channelIndex);
	}
	else if( enemy->getEnemyType() == Enemy::EnemyTypes::Naked )
	{
		playSound("NakedDie01.wav",sceneNode->_getDerivedPosition(),&channelIndex);
	}
	else if( enemy->getEnemyType() == Enemy::EnemyTypes::Chicken )
	{
		playSound("ChickenDie01.wav",sceneNode->_getDerivedPosition(),&channelIndex);
	}
	else if( enemy->getEnemyType() == Enemy::EnemyTypes::Cow )
	{
		playSound("CowDie01.wav",sceneNode->_getDerivedPosition(),&channelIndex);
	}
	else if( enemy->getEnemyType() == Enemy::EnemyTypes::Woman )
	{
		playSound("WomanDie01.wav",sceneNode->_getDerivedPosition(),&channelIndex);
	}
	else if( enemy->getEnemyType() == Enemy::EnemyTypes::Peasant )
	{
		playSound("PeasantDie01.wav",sceneNode->_getDerivedPosition(),&channelIndex);
	}
	else if( enemy->getEnemyType() == Enemy::EnemyTypes::BatteringRam )
	{
		playSound("BatteringRamDie01.wav",sceneNode->_getDerivedPosition(),&channelIndex);
	}
	else if( enemy->getEnemyType() == Enemy::EnemyTypes::Knight || enemy->getEnemyType() == Enemy::EnemyTypes::Archer)
	{
		if( (rand()%2) == 0 )
			playSound("EnemyDie01.wav",sceneNode->_getDerivedPosition(),&channelIndex);
		else
			playSound("EnemyDie02.wav",sceneNode->_getDerivedPosition(),&channelIndex);
	}
}

EVENTS_DEFINE_HANDLER(AudioManager, EnemyCreateItem)
{
	Debug::Out("AudioManager : handleEnemyCreateItemEvent");

	EnemyPtr enemy = evt->getEnemy();

	int channelIndex = -1;

	SceneNode* sceneNode = enemy->_getSceneNode();

	if( enemy->getEnemyType() == Enemy::EnemyTypes::Woman )
	{
		int sound = rand()% 3;

		if( sound == 0 )
			playSound("WomanGive01.wav",sceneNode->_getDerivedPosition(),&channelIndex);
		if( sound == 1 )
			playSound("WomanGive02.wav",sceneNode->_getDerivedPosition(),&channelIndex);
		else
			playSound("WomanGive03.wav",sceneNode->_getDerivedPosition(),&channelIndex);
	}
	else
		playSound("Drop01.wav",sceneNode->_getDerivedPosition(),&channelIndex);
}

EVENTS_DEFINE_HANDLER(AudioManager, ItemCatch)
{
	Debug::Out("AudioManager : handleItemCatchEvent");

	ItemPtr item = evt->getItem();

	int channelIndex = -1;

	SceneNode* sceneNode = item->_getSceneNode();

	// The player has just catch the item
	playSound("Item01.wav",sceneNode->_getDerivedPosition(),&channelIndex);
}

EVENTS_DEFINE_HANDLER(AudioManager, ProjectileFire)
{
	Debug::Out("AudioManager : handleProjectileFireEvent");

	EnemyPtr enemy = evt->getEnemy();

	int channelIndex = -1;

	SceneNode* sceneNode = enemy->_getSceneNode();

	// The enemy has just attack
	if( enemy->getEnemyType() == Enemy::EnemyTypes::Wizard )
	{
		if( (rand()%2) == 0 )
			playSound("WizardAttack01.wav",sceneNode->_getDerivedPosition(),&channelIndex);
		else 
			playSound("WizardAttack02.wav",sceneNode->_getDerivedPosition(),&channelIndex);
	}
	else
	{
		if( (rand()%2) == 0 )
			playSound("ArcherAttack01.wav",sceneNode->_getDerivedPosition(),&channelIndex);
		else 
			playSound("ArcherAttack02.wav",sceneNode->_getDerivedPosition(),&channelIndex);
	}
}

EVENTS_DEFINE_HANDLER(AudioManager, ProjectileHit)
{
	Debug::Out("AudioManager : handleProjectileHitEvent");

	PlayerPtr player = evt->getPlayer();

	int channelIndex = -1;

	SceneNode* sceneNode = player->_getSceneNode();

	// The enemy has just hit the player
	if( (rand()%2) == 0)
		playSound("PlayerHit01.wav",sceneNode->_getDerivedPosition(),&channelIndex);
	else
		playSound("PlayerHit02.wav",sceneNode->_getDerivedPosition(),&channelIndex);

}

EVENTS_DEFINE_HANDLER(AudioManager, ObjectHit)
{
	Debug::Out("AudioManager : handleObjectHitEvent");

	ObjectPtr object = evt->getObject();
	PlayerPtr player = evt->getPlayer();

	int channelIndex = -1;

	SceneNode* sceneNode = object->_getSceneNode();
	int sound = rand()%4;

	// The player has just hit the object
	if( player->isSpecial() )
	{
		if( (rand()%2) == 0)
			playSound("ObjectFireHit01.wav",sceneNode->_getDerivedPosition(),&channelIndex);
		else
			playSound("ObjectFireHit02.wav",sceneNode->_getDerivedPosition(),&channelIndex);
	}
	else
	{
		if( sound == 0)
			playSound("ObjectHit01.wav",sceneNode->_getDerivedPosition(),&channelIndex);
		else if( sound == 1)
			playSound("ObjectHit02.wav",sceneNode->_getDerivedPosition(),&channelIndex);
		else if( sound == 2)
			playSound("ObjectHit03.wav",sceneNode->_getDerivedPosition(),&channelIndex);
		else
			playSound("ObjectHit04.wav",sceneNode->_getDerivedPosition(),&channelIndex);
	}
}

EVENTS_DEFINE_HANDLER(AudioManager, ObjectKilled)
{
	Debug::Out("AudioManager : handleObjectKilledEvent");

	ObjectPtr object = evt->getObject();

	int channelIndex = -1;

	SceneNode* sceneNode = object->_getSceneNode();
	int sound = rand()%4;

	// The player has just killed the object
	if( !object->isBurning() )
	{
		if( sound == 0)
			playSound("ObjectDie01.wav",sceneNode->_getDerivedPosition(),&channelIndex);
		else if( sound == 1)
			playSound("ObjectDie02.wav",sceneNode->_getDerivedPosition(),&channelIndex);
		else if( sound == 2)
			playSound("ObjectDie03.wav",sceneNode->_getDerivedPosition(),&channelIndex);
		else
			playSound("ObjectDie04.wav",sceneNode->_getDerivedPosition(),&channelIndex);
	}
	else
	{
		if( (rand()%2) == 0)
			playSound("ObjectFireHit01.wav",sceneNode->_getDerivedPosition(),&channelIndex);
		else
			playSound("ObjectFireHit02.wav",sceneNode->_getDerivedPosition(),&channelIndex);
	}
}

// --------------------------------
// Lua Audio Lib
// --------------------------------
LUA_BEGIN_BINDING(AudioManager,audiolib)
LUA_BIND(AudioManager,playOnce)
LUA_BIND(AudioManager,playLoop)
LUA_END_BINDING()

//
// Load lua scripts that will be used by this manager
//
LUA_BEGIN_LOAD_SCRIPTS(AudioManager)
// 
// TODO : Load scripts if needed
//
LUA_END_LOAD_SCRIPTS()

LUA_DEFINE_FUNCTION(AudioManager, playOnce)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	Ogre::String name = luaL_checkstring(L, 1);

	int channelIndex = -1;

	AudioManager::getSingleton().playSound(name,Ogre::Vector3::ZERO,&channelIndex);

	/* return the number of results */
	return 0;
}

LUA_DEFINE_FUNCTION(AudioManager, playLoop)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	Ogre::String name = luaL_checkstring(L, 1);

	AudioManager::getSingleton().playSoundTrack(name);

	/* return the number of results */
	return 0;
}
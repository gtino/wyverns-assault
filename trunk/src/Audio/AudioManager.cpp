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

AudioManager::AudioManager() :
mSoundTrackChannelIndex(INVALID_SOUND_CHANNEL)
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
	finalize();
}

/** Initialize the audio manager */
void AudioManager::initialize()
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
}

void AudioManager::loadResources()
{
	//
	// TODO Load resources
	//
	int enemyHit0 = createStream( String("man_scream.mp3"));
	int itemCatch = createStream( String("knife.mp3"));
	int playerAttack = createStream(String("hit_blood_spat.wav"));
	int playerAttackSpecial = createStream(String("creature_hit.wav"));

	int softSoundTrack = createLoopedStream(String("soft_track.mp3"));
	int hardSoundTrack = createLoopedStream(String("hard_track.mp3"));
	int mainSoundTrack = createLoopedStream(String("main_track.mp3"));

	Debug::Out("AudioManager : Resources loaded");
}

void AudioManager::unloadResources()
{
	//
	// TODO Unload resources
	//
}


// Frame started! (EACH)
void AudioManager::update(Ogre::SceneNode *listenerNode, Ogre::Real timeElapsed)
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
		vectorVelocity = (listenerNode->_getDerivedPosition() - mPrevListenerPosition) / timeElapsed;
	else
		vectorVelocity = Vector3(0, 0, 0);

	vectorForward = listenerNode->_getDerivedOrientation().zAxis();
	vectorForward.normalise();

	vectorUp = listenerNode->_getDerivedOrientation().yAxis();
	vectorUp.normalise();

	listenerPosition.x = listenerNode->_getDerivedPosition().x;
	listenerPosition.y = listenerNode->_getDerivedPosition().y;
	listenerPosition.z = listenerNode->_getDerivedPosition().z;

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

	mPrevListenerPosition = listenerNode->_getDerivedPosition();

	for (channelIndex = 0; channelIndex < MAX_SOUND_CHANNELS; channelIndex++)
	{
		if (mChannelArray[channelIndex].sceneNode != NULL)
		{
			mSystem->getChannel(channelIndex, &nextChannel);
			if (timeElapsed > 0)
				vectorVelocity = (mChannelArray[channelIndex].sceneNode->_getDerivedPosition() - mChannelArray[channelIndex].prevPosition) / timeElapsed;
			else
				vectorVelocity = Vector3(0, 0, 0);

			listenerPosition.x = mChannelArray[channelIndex].sceneNode->_getDerivedPosition().x;
			listenerPosition.y = mChannelArray[channelIndex].sceneNode->_getDerivedPosition().y;
			listenerPosition.z = mChannelArray[channelIndex].sceneNode->_getDerivedPosition().z;

			listenerVelocity.x = vectorVelocity.x;
			listenerVelocity.y = vectorVelocity.y;
			listenerVelocity.z = vectorVelocity.z;

			nextChannel->set3DAttributes(&listenerPosition, &listenerVelocity);
			mChannelArray[channelIndex].prevPosition = mChannelArray[channelIndex].sceneNode->_getDerivedPosition();
		}
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

void AudioManager::playSound(String soundName, SceneNode *soundNode, int *channelIndex)
{
	playSound(mSoundNameToIdMap[soundName], soundNode, channelIndex);
}

void AudioManager::playSound(int soundIndex, SceneNode *soundNode, int *channelIndex)
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
	if ((channelIndexTemp != INVALID_SOUND_CHANNEL) && (mChannelArray[channelIndexTemp].sceneNode != NULL))
	{
		result = mSystem->getChannel(channelIndexTemp, &channel);
		if (result == FMOD_OK)
		{
			bool isPlaying;

			result = channel->isPlaying(&isPlaying);
			if ((result == FMOD_OK) && (isPlaying == true) && (mChannelArray[channelIndexTemp].sceneNode == soundNode))
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
	mChannelArray[channelIndexTemp].sceneNode = soundNode;
	mSceneNodeToChannelMap[soundNode] = channelIndexTemp;

	if (soundNode)
	{
		mChannelArray[channelIndexTemp].prevPosition = soundNode->_getDerivedPosition();

		initialPosition.x = soundNode->_getDerivedPosition().x;
		initialPosition.y = soundNode->_getDerivedPosition().y;
		initialPosition.z = soundNode->_getDerivedPosition().z;
		channel->set3DAttributes(&initialPosition, NULL);
	}

	result = channel->setVolume(1.0);
	// This is where the sound really starts.
	result = channel->setPaused(false);

	if (channelIndex)
		*channelIndex = channelIndexTemp;
}

void AudioManager::playSoundTrack(String name)
{
	stopSound(&mSoundTrackChannelIndex);
	playSound(name,0,&mSoundTrackChannelIndex);

	FMOD::Channel *channel = 0;
	mSystem->getChannel(mSoundTrackChannelIndex, &channel);
	
	if(channel)
		channel->setVolume(0.5);
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
void AudioManager::registerHandlers()
{
	boost::shared_ptr<AudioManager> this_ = shared_from_this();

	registerHandler(EventHandlerPtr(new EventHandler<AudioManager,EnemyHitEvent>(this_,&AudioManager::handleEnemyHitEvent)),EventTypes::EnemyHit);
	registerHandler(EventHandlerPtr(new EventHandler<AudioManager,PlayerAttackEvent>(this_,&AudioManager::handlePlayerAttackEvent)),EventTypes::PlayerAttack);
	registerHandler(EventHandlerPtr(new EventHandler<AudioManager,PlayerAttackSpecialEvent>(this_,&AudioManager::handlePlayerAttackSpecialEvent)),EventTypes::PlayerAttackSpecial);
	registerHandler(EventHandlerPtr(new EventHandler<AudioManager,ItemCatchEvent>(this_,&AudioManager::handleItemCatchEvent)),EventTypes::ItemCatch);
}

void AudioManager::unregisterHandlers()
{

}

void AudioManager::handleEnemyHitEvent(EnemyHitEventPtr evt)
{
	Debug::Out("AudioManager : handleEnemyHitEvent");

	EnemyPtr enemy = evt->getEnemy();
	PlayerPtr player = evt->getPlayer();

	if(player->isAttacking())
	{
		int channelIndex = -1;

		SceneNode* sceneNode = enemy->getSceneNode();
		bool channelFound = mSceneNodeToChannelMap.find(sceneNode) != mSceneNodeToChannelMap.end();

		if(channelFound)
			channelIndex = mSceneNodeToChannelMap[sceneNode];

		// The player has just hit the enemy
		playSound("man_scream.mp3",sceneNode,&channelIndex);
	}
}

void AudioManager::handlePlayerAttackEvent(PlayerAttackEventPtr evt)
{
	Debug::Out("AudioManager : handlePlayerAttackEvent");

	PlayerPtr player = evt->getPlayer();

	int channelIndex = -1;

	SceneNode* sceneNode = player->getSceneNode();
	bool channelFound = mSceneNodeToChannelMap.find(sceneNode) != mSceneNodeToChannelMap.end();

	if(channelFound)
		channelIndex = mSceneNodeToChannelMap[sceneNode];

	// The player has just hit the enemy
	playSound("hit_blood_spat.wav",sceneNode,&channelIndex);
}

void AudioManager::handlePlayerAttackSpecialEvent(PlayerAttackSpecialEventPtr evt)
{
	Debug::Out("AudioManager : handlePlayerAttackSpecialEvent");

	PlayerPtr player = evt->getPlayer();

	int channelIndex = -1;

	SceneNode* sceneNode = player->getSceneNode();
	bool channelFound = mSceneNodeToChannelMap.find(sceneNode) != mSceneNodeToChannelMap.end();

	if(channelFound)
		channelIndex = mSceneNodeToChannelMap[sceneNode];

	// The player has just hit the enemy
	playSound("creature_hit.wav",sceneNode,&channelIndex);
}

void AudioManager::handleItemCatchEvent(ItemCatchEventPtr evt)
{
	Debug::Out("AudioManager : handleItemCatchEvent");

	//PlayerPtr player = evt->getPlayer();
	ItemPtr item = evt->getItem();

	int channelIndex = -1;

	SceneNode* sceneNode = item->getSceneNode();
	bool channelFound = mSceneNodeToChannelMap.find(sceneNode) != mSceneNodeToChannelMap.end();

	if(channelFound)
		channelIndex = mSceneNodeToChannelMap[sceneNode];

	// The player has just hit the enemy
	playSound("knife.mp3",sceneNode,&channelIndex);
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
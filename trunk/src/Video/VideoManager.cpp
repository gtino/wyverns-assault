#include "..\include\Video\VideoManager.h"

using namespace WyvernsAssault;

// BEGIN SINGLETON
template<> VideoManager* Ogre::Singleton<VideoManager>::ms_Singleton = 0;
VideoManager* VideoManager::getSingletonPtr(void)
{
    return ms_Singleton;
}
VideoManager& VideoManager::getSingleton(void)
{  
    assert( ms_Singleton );  return ( *ms_Singleton );  
}
// END SINGLETON

VideoManager::VideoManager(SceneManager* sceneManager)
: mVideoNode(0)
, mTheoraVideoManager(0)
, mTheoraVideoClip(0)
, mInitialized(false)
{
	this->mSceneManager = sceneManager;

}

VideoManager::~VideoManager()
{
	finalize();
}

/** Initialize the lights manager */
void VideoManager::initialize()
{
	// and atach it to the root node
	mVideoNode = this->mSceneManager->getRootSceneNode()->createChildSceneNode(VIDEO_NODE_NAME);

	createQuad("video_quad","video_material",-1,1,1,-1);

	mTheoraVideoManager = TheoraVideoManager::getSingletonPtr();
	

	mTheoraVideoManager->setInputName(VIDEO_FILE);
	mTheoraVideoManager->createDefinedTexture("video_material");

	mTheoraVideoClip = mTheoraVideoManager->getVideoClipByName(VIDEO_FILE); 
	float dur=mTheoraVideoClip->getDuration();
	String s=StringConverter::toString(dur);
	String s2=StringConverter::toString(mTheoraVideoClip->getTimePosition(),4);

	mInitialized = true;
}


/** Finalize the lights manager */
void VideoManager::finalize()
{
	if(mInitialized)
	{
		stop();

		mSceneManager->destroyManualObject("video_quad");

		Utils::Destroy(mSceneManager,VIDEO_NODE_NAME);
		mVideoNode = NULL;

		mTheoraVideoManager->destroyVideoClip(mTheoraVideoClip);
		mTheoraVideoClip = NULL;
	}
}

void VideoManager::update(const float elapsedSeconds)
{
}

void VideoManager::play()
{
	mTheoraVideoClip->play();
}

void VideoManager::stop()
{
	mTheoraVideoClip->stop();
}

bool VideoManager::isDone()
{
	return mTheoraVideoClip->isDone();
}

//
// Internal stuff
//
void VideoManager::createQuad(String name,String material_name,float left,float top,float right,float bottom)
	{
		ManualObject* model = mSceneManager->createManualObject(name);
		model->begin(material_name);

		model->position(right,bottom,0); model->textureCoord(1,1);
		model->position(right,top   ,0); model->textureCoord(1,0);
		model->position(left ,top   ,0); model->textureCoord(0,0);
		model->position(left ,top   ,0); model->textureCoord(0,0);
		model->position(left, bottom,0); model->textureCoord(0,1);
		model->position(right,bottom,0); model->textureCoord(1,1);		

		model->end();
		// make the model 2D
		model->setUseIdentityProjection(true);
		model->setUseIdentityView(true);
		
		mVideoNode->attachObject(model);
	}


// --------------------------------
// Lua Video Lib
// --------------------------------
LUA_BEGIN_BINDING(VideoManager, videolib)
LUA_BIND(VideoManager, play)
LUA_BIND(VideoManager, stop)
LUA_END_BINDING()

//
// Load lua scripts that will be used by this manager
//
LUA_BEGIN_LOAD_SCRIPTS(VideoManager)
//
// TODO
//
LUA_END_LOAD_SCRIPTS()

//--------------------------------
// C++ Methods called FROM Lua
//--------------------------------
LUA_DEFINE_FUNCTION(VideoManager, play)
{
	//
	// Pass it to Lua script
	//
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 0;
}

LUA_DEFINE_FUNCTION(VideoManager, stop)
{
	//
	// Pass it to Lua script
	//
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 0;
}
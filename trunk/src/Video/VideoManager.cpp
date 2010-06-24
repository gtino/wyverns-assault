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
}


/** Finalize the lights manager */
void VideoManager::finalize()
{
}

void VideoManager::update(const float elapsedSeconds)
{
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
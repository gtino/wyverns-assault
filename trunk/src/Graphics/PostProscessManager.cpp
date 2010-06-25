#include "..\include\Graphics\PostProcessManager.h"

using namespace WyvernsAssault;

// BEGIN SINGLETON
template<> PostProcessManager* Ogre::Singleton<PostProcessManager>::ms_Singleton = 0;
PostProcessManager* PostProcessManager::getSingletonPtr(void)
{
    return ms_Singleton;
}
PostProcessManager& PostProcessManager::getSingleton(void)
{  
    assert( ms_Singleton );  return ( *ms_Singleton );  
}
// END SINGLETON

PostProcessManager::PostProcessManager(Viewport* viewport)
: mCompositorManager(0)
{
	assert(viewport);

	//
	// Save viewport pointer
	//
	this->mViewport = viewport;
}

PostProcessManager::~PostProcessManager()
{
	finalize();

	if(mCompositorManager)
	{
		mCompositorManager = NULL;
	}
}

void PostProcessManager::initialize()
{
	//
	// Save compositor manager pointer locally
	//
	mCompositorManager = CompositorManager::getSingletonPtr();	

	//
	// Add compositors to viewport one by one. Names from compositor script "PostProcess.compositor"
	//
	mCompositorManager->addCompositor(mViewport, "DOF");
	mCompositorManager->addCompositor(mViewport, "MotionBlur");
	mCompositorManager->addCompositor(mViewport, "RadialBlur");
	/** Debug compositors */
	mCompositorManager->addCompositor(mViewport, "ShowDepth");
	mCompositorManager->addCompositor(mViewport, "ShowNormal");

	//
	// Disable all by default
	//
	disableAll();
}

void PostProcessManager::finalize()
{
	disableAll();

	//
	// Remove compositors from viewport one by one
	//
	mCompositorManager->removeCompositor(mViewport, "DOF");
	mCompositorManager->removeCompositor(mViewport, "MotionBlur");
	mCompositorManager->removeCompositor(mViewport, "RadialBlur");
	/** Debug compositors */
	mCompositorManager->removeCompositor(mViewport, "ShowDepth");
	mCompositorManager->removeCompositor(mViewport, "ShowNormal");
}

void PostProcessManager::disableAll()
{
	//
	// Disable all compositors
	//
	mDOF = false;	
	mShowDepth = false;
	mShowNormal = false;
	mMotionBlur = 0;
	mRadialBlur = 0;

	mCompositorManager->setCompositorEnabled(mViewport, "DOF", false);
	mCompositorManager->setCompositorEnabled(mViewport, "MotionBlur", false);
	mCompositorManager->setCompositorEnabled(mViewport, "RadialBlur", false);
	/** Debug compositors */
	mCompositorManager->setCompositorEnabled(mViewport, "ShowDepth", false);
	mCompositorManager->setCompositorEnabled(mViewport, "ShowNormal", false);
}

void PostProcessManager::pause()
{
	//
	// Disable all compositors
	//
	mCompositorManager->setCompositorEnabled(mViewport, "DOF", false);
	mCompositorManager->setCompositorEnabled(mViewport, "MotionBlur", false);
	mCompositorManager->setCompositorEnabled(mViewport, "RadialBlur", false);
	/** Debug compositors */
	mCompositorManager->setCompositorEnabled(mViewport, "ShowDepth", false);
	mCompositorManager->setCompositorEnabled(mViewport, "ShowNormal", false);
}

void PostProcessManager::resume()
{
	//
	// Resume all compositors last state
	//
	mCompositorManager->setCompositorEnabled(mViewport, "DOF", mDOF);
	motionBlur(mMotionBlur);
	radialBlur(mRadialBlur);	
	/** Debug compositors */
	mCompositorManager->setCompositorEnabled(mViewport, "ShowDepth", mShowDepth);
	mCompositorManager->setCompositorEnabled(mViewport, "ShowNormal", mShowNormal);
}

// Depth of field compositor enable function
void PostProcessManager::depthOfField()
{
	mDOF = !mDOF;
	mCompositorManager->setCompositorEnabled(mViewport, "DOF", mDOF);
}

// Show normals on color channel
void PostProcessManager::showDepth()
{
	mShowDepth = !mShowDepth;
	mCompositorManager->setCompositorEnabled(mViewport, "ShowDepth", mShowDepth);
}

// Show depth on color channel
void PostProcessManager::showNormal()
{
	mShowNormal = !mShowNormal;
	mCompositorManager->setCompositorEnabled(mViewport, "ShowNormal", mShowNormal);
}

// Motion blur while duration
void PostProcessManager::motionBlur(float duration)
{
	mMotionBlur = duration;
	mCompositorManager->setCompositorEnabled(mViewport, "MotionBlur", (mMotionBlur > 0));
}

// Radial blur while duration
void PostProcessManager::radialBlur(float duration)
{
	mRadialBlur = duration;
	mCompositorManager->setCompositorEnabled(mViewport, "RadialBlur", (mRadialBlur > 0));
}

// Update life time of compositors with limited duration
void PostProcessManager::update(float elapsedSeconds)
{
	// Motion blur
	if(mMotionBlur > 0)
	{
		mMotionBlur = mMotionBlur - elapsedSeconds;
		if(mMotionBlur <= 0)
			motionBlur(0);
	}
	// Radial blur
	if(mRadialBlur > 0)
	{
		mRadialBlur = mRadialBlur - elapsedSeconds;
		if(mRadialBlur <= 0)
			radialBlur(0);
	}
}

// --------------
// Event handlers
// --------------
EVENTS_BEGIN_REGISTER_HANDLERS(PostProcessManager)
	EVENTS_REGISTER_HANDLER(PostProcessManager,ItemCatch)
EVENTS_END_REGISTER_HANDLERS()

EVENTS_BEGIN_UNREGISTER_HANDLERS(PostProcessManager)
	EVENTS_UNREGISTER_HANDLER(PostProcessManager,ItemCatch)
EVENTS_END_UNREGISTER_HANDLERS()

EVENTS_DEFINE_HANDLER(PostProcessManager, ItemCatch)
{
	ItemPtr item = evt->getItem();

	if ( item->getType() == Item::ItemTypes::PowerBig )
		this->motionBlur(6);
	else if( item->getType() == Item::ItemTypes::PowerMedium ) 
		this->motionBlur(4);
	else if( item->getType() == Item::ItemTypes::PowerSmall )	
		this->motionBlur(2);
}

// --------------------------------
// Lua Camera Lib
// --------------------------------
LUA_BEGIN_BINDING(PostProcessManager,postprocesslib)
LUA_BIND(PostProcessManager,enableCompositor)
LUA_BIND(PostProcessManager,disableCompositor)
LUA_END_BINDING()

//
// Load lua scripts that will be used by this manager
//
LUA_BEGIN_LOAD_SCRIPTS(PostProcessManager)
// 
// TODO : Load scripts if needed
//
LUA_END_LOAD_SCRIPTS()

LUA_DEFINE_FUNCTION( PostProcessManager, enableCompositor)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 1;
}

LUA_DEFINE_FUNCTION( PostProcessManager, disableCompositor)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 1;
}

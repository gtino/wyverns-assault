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

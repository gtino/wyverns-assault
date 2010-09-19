#include "..\include\States\SelectionState.h"

using namespace WyvernsAssault;

SelectionState::SelectionState(GraphicsManagerPtr graphicsManager, 
									 InputManagerPtr inputManager, 
									 AudioManagerPtr audioManager, 
									 CameraManagerPtr cameraManager, 
									 GuiManagerPtr guiManager, 
									 LevelManagerPtr levelManager)
: BaseState(graphicsManager,inputManager,audioManager, cameraManager, guiManager, levelManager)
, mLevel(0)
{
	//
	// TODO Constructor
	//
}

SelectionState::~SelectionState()
{
	//
	// TODO Destructor
	//
	exit();
}

/** Initialize current state */
void SelectionState::initialize()
{
	BaseState::initialize();
}

/** Manage input */
void SelectionState::input()
{
	//
	// TODO Read input
	//
	this->mInputManager->capture();
}

/** Load resources */
void SelectionState::load()
{
	//
	// Gui Screen for this state
	//
	mGuiScreen = mGuiManager->createScreen(GuiScreenId::SelectionGui, "Selection");
	
	GuiBackgroundPtr guiBackground = GuiBackgroundPtr(new GuiBackground());
	
	// Common aspect ratio is 4/3
	if( mWindow->getViewport(0)->getCamera()->getAspectRatio() > 1.34 )
		guiBackground->setImage("wide/Selection.png","SelectionBackground","General");
	else
		guiBackground->setImage("normal/Selection.png","SelectionBackground","General");

	mGuiScreen->setBackground(guiBackground);

	// Gui Widgets for this state
	mMenu = new GuiMenu(mWindow->getViewport(0)->getCamera()->getAspectRatio(), GuiScreenId::SelectionGui);
	
	// Add menu to screen
	mGuiScreen->addMenu(mMenu);
}

/** Update internal stuff */
void SelectionState::update(const float elapsedSeconds)
{
	//
	// TODO Update
	//
}

/** Render */
void SelectionState::render(const float elapsedSeconds)
{
	//
	// TODO Render
	//
}

/** Unload resources */
void SelectionState::unload() 
{
	if(mGuiScreen)
	{
		//
		// Remove gui
		//
		mGuiManager->removeScreen(GuiScreenId::SelectionGui);
		mGuiScreen.reset();
	}
}

/** Destroy the state */
void SelectionState::finalize()
{
	BaseState::finalize();
}

/** Get state Id */
GameStateId SelectionState::getStateId()
{
	//
	// BEWARE! Make sure this is the correspondent state or we fuck up everything!
	//
	return GameStateId::Selection;
}

/** Called when the state has to be paused */
void SelectionState::pause()
{
	//
	// TODO : Pause state
	//
}

/** Called when the state has to be resumed (from pause) */
void SelectionState::resume()
{
	//
	// TODO : Resume state
	//
}

/** Buffered input - keyboard key clicked */
bool SelectionState::keyReleased(const OIS::KeyEvent& e)
{
	switch(e.key)
	{
	case OIS::KC_RETURN:
		this->mNextGameStateId = GameStateId::LevelLoading;
		break;
	}

	return true;
}

bool SelectionState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID buttonId)
{
	/*switch(buttonId)
	{
		case OIS::MouseButtonID::MB_Left:
			this->mNextGameStateId = GameStateId::MainMenu;
			break;
	}*/

	return true;
}
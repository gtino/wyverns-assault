#include "..\include\States\OutroState.h"

using namespace WyvernsAssault;

OutroState::OutroState(GraphicsManagerPtr graphicsManager, InputManagerPtr inputManager, AudioManagerPtr audioManager, CameraManagerPtr cameraManager, GuiManagerPtr guiManager)
: BaseState(graphicsManager,inputManager,audioManager, cameraManager, guiManager)
{
	//
	// TODO Constructor logic HERE
	//
}

OutroState::~OutroState()
{
	//
	// TODO Distructor logic HERE
	//
	finalize();
}

/** Initialize current state */
void OutroState::initialize()
{
	BaseState::initialize();
}

/** Load resources */
void OutroState::load()
{
	//
	// Gui Screen for this state
	//
	mGuiScreen = new GuiScreen(mSceneManager, GuiScreenId::OutroGui, "OutroScreen");
	
	GuiBackground* guiBackground = new GuiBackground();
	guiBackground->setImage("Outro.png","OutroBackground","General");

	mGuiScreen->setBackground(guiBackground);

	// Gui Widgets for this state
	mMenu = new GuiMenu(mWindow->getViewport(0)->getCamera()->getAspectRatio(), GuiScreenId::OutroGui);
	
	// Add menu to screen
	mGuiScreen->addMenu(mMenu);

	//
	// Register the screen as input event listener, so it can receive events
	//
	mInputManager->addListener(mGuiScreen);
}

/** Manage input */
void OutroState::input()
{
	//
	// TODO Read input
	//
}

/** Update internal stuff */
void OutroState::update(const float elapsedSeconds)
{
	//
	// TODO Update
	//
}

/** Render */
void OutroState::render(const float elapsedSeconds)
{
	//
	// TODO Render
	//
}

/** Unload resources */
void OutroState::unload() 
{
	if(mGuiScreen)
	{
		//
		// Remove gui listener
		//
		mInputManager->removeListener(mGuiScreen);
		//
		// Remove gui
		//
		mGuiScreen->removeGui();
	}
}

/** Destroy the state */
void OutroState::finalize()
{
	BaseState::finalize();
}

/** Get state Id */
GameStateId OutroState::getStateId()
{
	//
	// BEWARE! Make sure this is the correspondent state or we fuck up everything!
	//
	return GameStateId::Outro;
}

/** Called when the state has to be paused */
void OutroState::pause()
{
	//
	// TODO : Pause state
	//
}

/** Called when the state has to be resumed (from pause) */
void OutroState::resume()
{
	//
	// TODO : Resume state
	//
}
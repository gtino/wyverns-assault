#include "..\include\States\OutroState.h"

using namespace WyvernsAssault;

OutroState::OutroState(GraphicsManager& graphicsManager, InputManager& inputManager, AudioManager& audioManager)
: BaseState(graphicsManager,inputManager,audioManager)
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
	mGuiScreen = new GuiScreen(mSceneManager, GuiScreenId::IntroGui, "OutroScreen");
	
	GuiBackground* guiBackground = new GuiBackground();
	guiBackground->setImage("Outro.png","OutroBackground","General");

	mGuiScreen->setBackground(guiBackground);

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
		// Register the screen as input event listener, so it can receive events
		//
		mInputManager->removeListener(mGuiScreen);

		delete mGuiScreen;
		mGuiScreen = 0;
	}
	//
	// TODO Unload
	//
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
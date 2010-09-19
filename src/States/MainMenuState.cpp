#include "..\include\States\MainMenuState.h"
using namespace WyvernsAssault;

MainMenuState::MainMenuState(GraphicsManagerPtr graphicsManager, 
							 InputManagerPtr inputManager, 
							 AudioManagerPtr audioManager, 
							 CameraManagerPtr cameraManager, 
							 GuiManagerPtr guiManager,
							 LevelManagerPtr levelManager)
: BaseState(graphicsManager,inputManager,audioManager, cameraManager, guiManager, levelManager)
{
	//
	// TODO Constructor logic HERE
	//
	this->mNextGameStateId = this->getStateId();
}

MainMenuState::~MainMenuState()
{
	//
	// TODO Distructor logic HERE
	//
	finalize();
}

/** Initialize current state */
void MainMenuState::initialize()
{
	BaseState::initialize();

	timer = 0.0;
	animControl = 0;
	isWideScreen = false;
}

/** Manage input */
void MainMenuState::input()
{
	//
	// TODO Read input
	//
	this->mInputManager->capture();
}

/** Load resources */
void MainMenuState::load()
{
	// Common aspect ratio is 4/3
	if( mWindow->getViewport(0)->getCamera()->getAspectRatio() > 1.34 )
		isWideScreen = true;

	//
	// Gui Screen for this state
	//
	mGuiScreen = mGuiManager->createScreen(GuiScreenId::MainMenuGui, "MainMenuScreen");
	
	guiBackground = GuiBackgroundPtr(new GuiBackground());
	if( isWideScreen )
		guiBackground->setImage("GUI/Title/Animation/Wide");
	else
		guiBackground->setImage("GUI/Title/Animation/Normal");

	guiBackground->setFrame(0);

	mGuiScreen->setBackground(guiBackground);

	// Gui Widgets for this state
	mMenu = new GuiMenu(mWindow->getViewport(0)->getCamera()->getAspectRatio(), GuiScreenId::MainMenuGui);	

	//
	// Play soft soundtrack
	//
	mAudioManager->playSoundTrack("hard_track.mp3");
}

/** Update internal stuff */
void MainMenuState::update(const float elapsedSeconds)
{
	timer = timer + elapsedSeconds;

	if( timer > 2.25 )
	{
		guiBackground->setFrame(6);
		mGuiScreen->addMenu(mMenu);
		animControl = 1;
	}		
	else if( timer > 1.75 )
		guiBackground->setFrame(5);
	else if( timer > 1.5 )
		guiBackground->setFrame(4);
	else if( timer > 1.25 )
		guiBackground->setFrame(3);
	else if( timer > 0.75 )
		guiBackground->setFrame(2);
	else if( timer > 0.25 )
		guiBackground->setFrame(1);	
}

/** Render */
void MainMenuState::render(const float elapsedSeconds)
{
	//
	// TODO Render
	//
}

/** Unload resources */
void MainMenuState::unload() 
{
	if(mGuiScreen)
	{
		//
		// Remove gui
		//
		mGuiManager->removeScreen(GuiScreenId::MainMenuGui);
		mGuiScreen.reset();
	}
}

/** Destroy the state */
void MainMenuState::finalize()
{
	BaseState::finalize();
}

/** Get state Id */
GameStateId MainMenuState::getStateId()
{
	//
	// BEWARE! Make sure this is the correspondent state or we fuck up everything!
	//
	return GameStateId::MainMenu;
}

/** Called when the state has to be paused */
void MainMenuState::pause()
{
	mGuiScreen->hide();
}

/** Called when the state has to be resumed (from pause) */
void MainMenuState::resume()
{
	//
	// Set next state to this state	
	//
	this->mNextGameStateId = this->getStateId();

	mGuiScreen->show();
}

/** Buffered input - keyboard key clicked */
bool MainMenuState::keyReleased(const OIS::KeyEvent& e)
{
	if( animControl > 0 )
	{
		switch(e.key)
		{
		case OIS::KC_LEFT:
			mMenu->previousOption();
			break;
		case OIS::KC_RIGHT:
			mMenu->nextOption();
			break;
		case OIS::KC_RETURN:
			switch(mMenu->getCurrentOption())
			{
			case GuiWidgetMenuId::PlayMenu:
				this->mNextGameStateId = GameStateId::Selection;
				break;
			case GuiWidgetMenuId::OptionsMenu:
				this->mNextGameStateId = GameStateId::Options;
				break;
			case GuiWidgetMenuId::CreditsMenu:
				this->mNextGameStateId = GameStateId::Credits;
				break;
			case GuiWidgetMenuId::QuitMenu:
				this->mNextGameStateId = GameStateId::Exit;
				break;
			}		
			break;
		case OIS::KC_ESCAPE:
			this->mNextGameStateId = GameStateId::Exit;
			break;
		}
	}

	return true;
}

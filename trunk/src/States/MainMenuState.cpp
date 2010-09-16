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
	float ratio = mWindow->getViewport(0)->getCamera()->getAspectRatio();

	// Common aspect ratio is 4/3
	if( mWindow->getViewport(0)->getCamera()->getAspectRatio() > 1.34 )
		isWideScreen = true;

	//
	// Gui Screen for this state
	//
	mGuiScreen = mGuiManager->createScreen(GuiScreenId::MainMenuGui, "MainMenuScreen");
	
	GuiBackgroundPtr guiBackground = GuiBackgroundPtr(new GuiBackground());
	if( isWideScreen )
		guiBackground->setImage("wide/MainMenu.png","MainMenuBackground","General");
	else
		guiBackground->setImage("normal/MainMenu.png","MainMenuBackground","General");

	guiTitle = GuiForegroundPtr(new GuiForeground());

	mGuiScreen->setBackground(guiBackground);
	mGuiScreen->setForeground(guiTitle);

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

	if( timer > 4.0 )
	{
		guiTitle->setFrame(6);
		mGuiScreen->addMenu(mMenu);
		animControl = 1;
	}		
	else if( timer > 3.0 )
		guiTitle->setFrame(5);
	else if( timer > 2.5 )
		guiTitle->setFrame(4);
	else if( timer > 2.0 )
		guiTitle->setFrame(3);
	else if( timer > 1.5 )
		guiTitle->setFrame(2);
	else if( timer > 1.0 )
		guiTitle->setFrame(1);
	else if( timer > 0.5 )
	{		
		if( isWideScreen )
			guiTitle->setImage("GUI/Title/Animation/Wide");
		else
			guiTitle->setImage("GUI/Title/Animation/Normal");

		guiTitle->setFrame(0);				
	}	
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
				this->mNextGameStateId = GameStateId::LevelLoading;
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

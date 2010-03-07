#include "..\include\Input\InputManager.h"

using namespace WyvernsAssault;

InputManager::InputManager()
: mInputManager( NULL )
, mKeyboard( NULL )
, mMouse( NULL )
, mJoyStick( NULL )
, mUseBufferedInputKeys(false)
, mUseBufferedInputMouse(false)
, mUseBufferedInputJoyStick(false)
, mInputTypeSwitchingOn(false)
{
	//
	// TODO Constructor
	//
}

InputManager::~InputManager()
{
	//
	// TODO Destructor
	//
}

/** Initialize the input manager */
void InputManager::initialize( Ogre::RenderWindow* window, bool showDefaultMousePointer )
{	
	size_t windowHandle = 0;
	window->getCustomAttribute( "WINDOW", &windowHandle );

	std::ostringstream windowHandleString;
	windowHandleString << windowHandle;

	OIS::ParamList parameters;
	parameters.insert( std::make_pair( std::string( "WINDOW" ), windowHandleString.str() ) );
	if ( showDefaultMousePointer )
	{
		parameters.insert( std::make_pair( std::string( "w32_mouse" ), std::string( "DISCL_FOREGROUND" ) ) );
		parameters.insert( std::make_pair( std::string( "w32_mouse" ), std::string( "DISCL_NONEXCLUSIVE" ) ) );
		parameters.insert( std::make_pair( std::string( "w32_keyboard" ), std::string( "DISCL_FOREGROUND" ) ) );
		parameters.insert( std::make_pair( std::string( "w32_keyboard" ), std::string( "DISCL_NONEXCLUSIVE" ) ) );
		parameters.insert( std::make_pair( std::string( "x11_mouse_grab" ), std::string( "false" ) ) );
		parameters.insert( std::make_pair( std::string( "x11_mouse_hide" ), std::string( "false" ) ) );
		parameters.insert( std::make_pair( std::string( "x11_keyboard_grab" ), std::string( "false" ) ) );
		parameters.insert( std::make_pair( std::string( "XAutoRepeatOn" ), std::string( "true" ) ) );
	}

	mInputManager = OIS::InputManager::createInputSystem( parameters );

	mKeyboard = static_cast< OIS::Keyboard* >( mInputManager->createInputObject( OIS::OISKeyboard, true ) );
	mMouse = static_cast< OIS::Mouse* >( mInputManager->createInputObject( OIS::OISMouse, true ) );
	//mJoyStick = static_cast< OIS::JoyStick* >( mInputManager->createInputObject( OIS::OISJoyStick, true ) );

	const OIS::MouseState& mouseState = mMouse->getMouseState();

	mouseState.width = window->getWidth();
	mouseState.height = window->getHeight();

	mMouse->setEventCallback( this );
	mKeyboard->setEventCallback( this );
	//mJoyStick->setEventCallback( this );
}

void InputManager::finalize()
{
	if ( mInputManager )
	{
		if ( mMouse )
		{
			mInputManager->destroyInputObject( mMouse );
		}

		if ( mKeyboard )
		{
			mInputManager->destroyInputObject( mKeyboard );
		}

		if( mJoyStick )
		{
			mInputManager->destroyInputObject( mJoyStick );
		}

		OIS::InputManager::destroyInputSystem( mInputManager );
	}
}

/** Add input listener */
void InputManager::addListener(InputListener* listener)
{
	// TODO Check if listener is already present!
	this->mRegisteredListeners.push_back(listener);
}

/** Remove input listener */
void InputManager::removeListener(InputListener* listener)
{
	// TODO Remove listener (if present)!
}

/** Acquire all inputs */
void InputManager::acquireAll()
{
	//
	// TBD Is this really needed?
	//
}

/** Relese all inputs */
void InputManager::unacquireAll()
{
	//
	// TBD Is this really needed?
	//
}

void InputManager::capture()
{
	mMouse->capture();
	mKeyboard->capture();
	//mJoyStick->capture(); // TODO Set up joystick properly
}

void InputManager::switchMouseMode()
{
	mUseBufferedInputMouse = !mUseBufferedInputMouse;
	mMouse->setBuffered(mUseBufferedInputMouse);
}

void InputManager::switchKeyMode()
{
	mUseBufferedInputKeys = !mUseBufferedInputKeys;
	mKeyboard->setBuffered(mUseBufferedInputKeys);
}

void InputManager::switchJoyStickMode()
{
	mUseBufferedInputJoyStick = !mUseBufferedInputJoyStick;
	mJoyStick->setBuffered(mUseBufferedInputJoyStick);
}

// ---------------
// Mouse Listeners
// ---------------
/** Buffered input - mouse moved */
bool InputManager::mouseMoved( const OIS::MouseEvent& e ) 
{
	WA_CALL_LISTENERS(mouseMoved(e))
	return true;
}

/** Buffered input - mouse button pressed */
bool InputManager::mousePressed( const OIS::MouseEvent& e, OIS::MouseButtonID button )
{
	WA_CALL_LISTENERS(mousePressed(e, button))
	return true;
}

/** Buffered input - mouse button released */
bool InputManager::mouseReleased( const OIS::MouseEvent& e, OIS::MouseButtonID button )
{
	WA_CALL_LISTENERS(mouseReleased(e, button))
	return true;
}

// ------------------
// Keyboard listeners
// ------------------
/** Buffered input - keyboard key clicked */
bool InputManager::keyClicked(const OIS::KeyEvent& e)
{
	WA_CALL_LISTENERS(keyClicked(e))
	return true;
}

/** Buffered input - keyboard key clicked */
bool InputManager::keyPressed( const OIS::KeyEvent& e )
{
	WA_CALL_LISTENERS(keyPressed(e))
	return true;
}

/** Buffered input - keyboard key clicked */
bool InputManager::keyReleased( const OIS::KeyEvent& e )
{
	WA_CALL_LISTENERS(keyReleased(e))
	return true;
}

// ----------------
// JoyStickListener
// ----------------
/** Buffered input - joystick button pressed */
bool InputManager::buttonPressed(const OIS::JoyStickEvent &evt, int index)
{
	WA_CALL_LISTENERS(buttonPressed(evt,index))
	return true;
}

/** Buffered input - joystick button released */
bool InputManager::buttonReleased(const OIS::JoyStickEvent &evt, int index)
{
	WA_CALL_LISTENERS(buttonReleased(evt,index))
	return true;
}

/** Buffered input - axis pad moved */
bool InputManager::axisMoved(const OIS::JoyStickEvent &evt, int index)
{
	WA_CALL_LISTENERS(axisMoved(evt,index))
	return true;
}

/** Buffered input - pov moved */
bool InputManager::povMoved(const OIS::JoyStickEvent &evt, int index)
{
	WA_CALL_LISTENERS(povMoved(evt,index))
	return true;
}
#include "..\include\Input\InputManager.h"

using namespace WyvernsAssault;

InputManager::InputManager()
: mInputManager( NULL )
, mKeyboard( NULL )
, mMouse( NULL )
, mJoyStick( NULL )
, mUseBufferedInputKeys(false)
, mUseBufferedInputMouse(false)
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

	const OIS::MouseState& mouseState = mMouse->getMouseState();

	mouseState.width = window->getWidth();
	mouseState.height = window->getHeight();

	mMouse->setEventCallback( this );
	mKeyboard->setEventCallback( this );

	//
	// TODO Initialize JoyStick!
	//
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

bool InputManager::mouseMoved( const OIS::MouseEvent& e ) 
{
	return true;
}

bool InputManager::mousePressed( const OIS::MouseEvent& e, OIS::MouseButtonID button )
{
	return true;
}

bool InputManager::mouseReleased( const OIS::MouseEvent& e, OIS::MouseButtonID button )
{
	return true;
}

bool InputManager::keyPressed( const OIS::KeyEvent& e )
{
	return true;
}

bool InputManager::keyReleased( const OIS::KeyEvent& e )
{
	return true;
}
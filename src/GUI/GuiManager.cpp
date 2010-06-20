#include "..\include\GUI\GuiManager.h"

using namespace Ogre;
using namespace WyvernsAssault;

// BEGIN SINGLETON
template<> GuiManager* Ogre::Singleton<GuiManager>::ms_Singleton = 0;
GuiManager* GuiManager::getSingletonPtr(void)
{
	return ms_Singleton;
}
GuiManager& GuiManager::getSingleton(void)
{  
	assert( ms_Singleton );  return ( *ms_Singleton );  
}
// END SINGLETON

GuiManager::GuiManager()
{
}

GuiManager::~GuiManager()
{
	//
	// TODO Destructor
	//
	finalize();
}

bool GuiManager::initialize(Ogre::Root* root, Ogre::SceneManager* sceneManager, Ogre::RenderWindow* window)
{
	mRoot = root;
	mSceneManager = sceneManager;
	mWindow = window;

	return true;
}

void GuiManager::finalize()
{
	//
	// TODO finalize
	//
}

GuiScreenPtr GuiManager::createGui(GuiScreenId id, const Ogre::String& name)
{
	if( this->hasGui(id) )
	{
		return mGuiScreenMap[id];
	}
	else
	{
		GuiScreenPtr guiScreen = GuiScreenPtr( new GuiScreen(mSceneManager, id, name) );
		mGuiScreenMap[id] = guiScreen;
		return guiScreen;
	}
}

bool GuiManager::hasGui(GuiScreenId id)
{
	return mGuiScreenMap[id] != NULL;
}

void GuiManager::removeGui(GuiScreenId id)
{
	mGuiScreenMap[id]->removeGui();
	mGuiScreenMap.erase(id);
}

//----------------------------------------------------------------//
bool GuiManager::mouseMoved( const OIS::MouseEvent &arg )
{
	return true;
}

//----------------------------------------------------------------//
bool GuiManager::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	return true;
}

//----------------------------------------------------------------//
bool GuiManager::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	return true;
}

//----------------------------------------------------------------//
bool GuiManager::keyPressed( const OIS::KeyEvent &arg )
{
	return true;
}

//----------------------------------------------------------------//
bool GuiManager::keyReleased( const OIS::KeyEvent &arg )
{
	return true;
}

// --------------
// Event handlers
// --------------
EVENTS_BEGIN_REGISTER_HANDLERS(GuiManager)	
	EVENTS_REGISTER_HANDLER(GuiManager, ItemCatch);
	EVENTS_REGISTER_HANDLER(GuiManager, PlayerAttackSpecial);
	EVENTS_REGISTER_HANDLER(GuiManager, EnemyKill);
	EVENTS_REGISTER_HANDLER(GuiManager, PlayerHit);
EVENTS_END_REGISTER_HANDLERS()

EVENTS_BEGIN_UNREGISTER_HANDLERS(GuiManager)
	EVENTS_UNREGISTER_HANDLER(GuiManager, ItemCatch);
	EVENTS_UNREGISTER_HANDLER(GuiManager, PlayerAttackSpecial);
	EVENTS_UNREGISTER_HANDLER(GuiManager, EnemyKill);
	EVENTS_UNREGISTER_HANDLER(GuiManager, PlayerHit);
EVENTS_END_UNREGISTER_HANDLERS()


EVENTS_DEFINE_HANDLER(GuiManager, ItemCatch)
{
	Debug::Out("GuiManager : handleItemCatchEvent");

	PlayerPtr player = evt->getPlayer();

	GuiWidgetPtr ui = mGuiScreenMap[GuiScreenId::PlayGui]->getWidget( player->getGuiId() );

	GuiUserInterface::UserInterfaceData userData;

	userData.life = player->getLife();
	userData.special = player->getSpecial();
	userData.points = player->getPoints();

	ui->setData(&userData);
}

EVENTS_DEFINE_HANDLER(GuiManager, PlayerAttackSpecial)
{
	Debug::Out("GuiManager : handlePlayerAttackSpecialEvent");

	PlayerPtr player = evt->getPlayer();

	GuiWidgetPtr ui = mGuiScreenMap[GuiScreenId::PlayGui]->getWidget( player->getGuiId() );

	GuiUserInterface::UserInterfaceData userData;

	userData.life = player->getLife();
	userData.special = player->getSpecial();
	userData.points = player->getPoints();

	ui->setData(&userData);
}

EVENTS_DEFINE_HANDLER(GuiManager, EnemyKill)
{
	Debug::Out("GuiManager : handleEnemyKillEvent");

	PlayerPtr player = evt->getPlayer();

	GuiWidgetPtr ui = mGuiScreenMap[GuiScreenId::PlayGui]->getWidget( player->getGuiId() );

	GuiUserInterface::UserInterfaceData userData;

	userData.life = player->getLife();
	userData.special = player->getSpecial();
	userData.points = player->getPoints();

	ui->setData(&userData);
}

EVENTS_DEFINE_HANDLER(GuiManager, PlayerHit)
{
	Debug::Out("GuiManager : handlePlayerHitEvent");

	PlayerPtr player = evt->getPlayer();

	GuiWidgetPtr ui = mGuiScreenMap[GuiScreenId::PlayGui]->getWidget( player->getGuiId() );

	GuiUserInterface::UserInterfaceData userData;

	userData.life = player->getLife();
	userData.special = player->getSpecial();
	userData.points = player->getPoints();

	ui->setData(&userData);
}

// --------------------------------
// Lua Gui Lib
// --------------------------------
LUA_BEGIN_BINDING(GuiManager::guilib)
LUA_BIND("showOverlay", GuiManager::luaShowOverlay)
LUA_BIND("hideOverlay", GuiManager::luaHideOverlay)
LUA_END_BINDING()

int GuiManager::luaShowOverlay(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 1;
}

int GuiManager::luaHideOverlay(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 1;
}

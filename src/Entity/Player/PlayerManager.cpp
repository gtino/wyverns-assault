#include "..\..\..\include\Entity\Player\PlayerManager.h"


using namespace WyvernsAssault;

// BEGIN SINGLETON
template<> PlayerManager* Ogre::Singleton<PlayerManager>::ms_Singleton = 0;
PlayerManager* PlayerManager::getSingletonPtr(void)
{
    return ms_Singleton;
}
PlayerManager& PlayerManager::getSingleton(void)
{  
    assert( ms_Singleton );  return ( *ms_Singleton );  
}
// END SINGLETON

PlayerManager::PlayerManager(Ogre::SceneManager* sceneManager):
mSceneManager(0),
mIsDebugEnabled(false)
{
	mSceneManager = sceneManager;
}

void PlayerManager::initialize()
{
}

PlayerManager::~PlayerManager()
{
	finalize();
}

void PlayerManager::finalize()
{
	mPlayerList.clear();
	mPlayerMap.clear();
}

PlayerPtr PlayerManager::createPlayer(Ogre::String name, Ogre::String mesh)
{
	// Player node
	Ogre::Entity* playerMesh = mSceneManager->createEntity(name, mesh);
	playerMesh->setQueryFlags(SceneManager::ENTITY_TYPE_MASK);
	Ogre::SceneNode* playerSceneNode = mSceneManager->getRootSceneNode()->createChildSceneNode(name);
	playerSceneNode->attachObject(playerMesh);

	PlayerPtr player = PlayerPtr(new Player(name));

	player->initializeEntity( playerMesh, playerSceneNode, mSceneManager );

	mPlayerList.push_back(player);
	mPlayerMap[name] = player;

	return player;
}


PlayerPtr PlayerManager::getPlayer(Ogre::String name)
{
	return mPlayerMap[name];
}

bool PlayerManager::removePlayer(Ogre::String name)
{
	//
	// TODO : maybe we don't really need a list, and we can just use a map...
	//
	PlayerPtr playerToErase = mPlayerMap[name];

	mPlayerMap.erase(name);
	
	PlayerListIterator it = find(mPlayerList.begin(), mPlayerList.end(), playerToErase);
	
	if( it != mPlayerList.end() )
		mPlayerList.erase(it);

	mSceneManager->destroyEntity(name);//getRootSceneNode()->removeChild(name);

	return true;
}

void PlayerManager::attack(Ogre::String name)
{
	PlayerPtr player = getPlayer(name);

	player->attackA();

	PlayerAttackEventPtr evt = PlayerAttackEventPtr(new PlayerAttackEvent(player));

	raiseEvent(evt);
}

void PlayerManager::attackSpecial(Ogre::String name)
{
	PlayerPtr player = getPlayer(name);

	player->attackSpecial();

	PlayerAttackSpecialEventPtr evt = PlayerAttackSpecialEventPtr(new PlayerAttackSpecialEvent(player));

	raiseEvent(evt);
}

void PlayerManager::setDebugEnabled(bool isDebugEnabled)
{
	if(mIsDebugEnabled != isDebugEnabled)
	{
		mIsDebugEnabled = isDebugEnabled;

		for(int i = 0; i < mPlayerList.size() ; i++)
		{
			PlayerPtr player =  mPlayerList[i];
			player->setDebugEnabled(mIsDebugEnabled);
		}
	}
}

// --------------
// Event handlers
// --------------
void PlayerManager::registerHandlers()
{
	boost::shared_ptr<PlayerManager> this_ = shared_from_this();

	registerHandler(EventHandlerPtr(new EventHandler<PlayerManager,PlayerHitEvent>(this_,&PlayerManager::handlePlayerHitEvent)),EventTypes::PlayerHit);
}

void PlayerManager::unregisterHandlers()
{

}

void PlayerManager::handlePlayerHitEvent(PlayerHitEventPtr evt)
{
}

// --------------------------------
// Lua Player Lib
// --------------------------------
LUA_BEGIN_BINDING(PlayerManager::playerlib)
LUA_BIND("getPosition", PlayerManager::getPlayerPosition)
LUA_END_BINDING()

int PlayerManager::getPlayerPosition(lua_State *L)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	Ogre::String playerName = lua_tostring(L,1);

	//Player player = PlayerManager::getSingleton().getPlayer(playerName);

	//Vector3 position = player->getPosition();

	Vector3 position = Vector3(1,2,3);

	/* push the X */
	lua_pushnumber(L, position.x);
	/* push the Y */
	lua_pushnumber(L, position.y);
	/* push the Z */
	lua_pushnumber(L, position.z);

	/* return the number of results */
	return 3;

}
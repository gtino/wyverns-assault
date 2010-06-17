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

PlayerManager::PlayerManager(Ogre::SceneManager* sceneManager)
: mSceneManager(0)
, mIsDebugEnabled(false)
, mPlayerNode(0)
{
	mSceneManager = sceneManager;
}

void PlayerManager::initialize()
{
	mPlayerNode = mSceneManager->getRootSceneNode()->createChildSceneNode(PLAYER_NODE_NAME);
}

PlayerManager::~PlayerManager()
{
	finalize();
}

void PlayerManager::finalize()
{
	mPlayerList.clear();
	mPlayerMap.clear();

	Utils::Destroy(mSceneManager, PLAYER_NODE_NAME);
	mPlayerNode = NULL;
}

void PlayerManager::update(const float elapsedSeconds)
{
	for(int i = 0; i < mPlayerList.size() ; i++)
	{
		PlayerPtr player =  mPlayerList[i];
		player->updateEntity(elapsedSeconds);
	}
}

PlayerPtr PlayerManager::createPlayer(Ogre::String name, Ogre::String mesh)
{
	// Player node
	Ogre::Entity* playerMesh = mSceneManager->createEntity(name, mesh);
	playerMesh->setQueryFlags(SceneManager::ENTITY_TYPE_MASK);
	Ogre::SceneNode* playerSceneNode = mPlayerNode->createChildSceneNode(name);
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

	if( player->attackStart() )
	{
		player->attackFinish();
		PlayerAttackEventPtr evt = PlayerAttackEventPtr(new PlayerAttackEvent(player));
		raiseEvent(evt);
	}
}

void PlayerManager::attackSpecial(Ogre::String name)
{
	PlayerPtr player = getPlayer(name);

	player->attackSpecial();

	if( player->attackStart() )
	{
		player->attackFinish();
		PlayerAttackSpecialEventPtr evt = PlayerAttackSpecialEventPtr(new PlayerAttackSpecialEvent(player));
		raiseEvent(evt);
	}
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
EVENTS_BEGIN_REGISTER_HANDLERS(PlayerManager)
	EVENTS_REGISTER_HANDLER(PlayerManager,PlayerHit)
	EVENTS_REGISTER_HANDLER(PlayerManager,ItemCatch)
EVENTS_END_REGISTER_HANDLERS()

EVENTS_BEGIN_UNREGISTER_HANDLERS(PlayerManager)
	EVENTS_UNREGISTER_HANDLER(PlayerManager,PlayerHit)
	EVENTS_UNREGISTER_HANDLER(PlayerManager,ItemCatch)
EVENTS_END_UNREGISTER_HANDLERS()

EVENTS_DEFINE_HANDLER(PlayerManager, PlayerHit)
{
	Debug::Out("PlayerManager : handlePlayerHitEvent");

	PlayerPtr player = evt->getPlayer();
	EnemyPtr enemy = evt->getEnemy();

	player->addLife(- enemy->getHitDamage());
}

EVENTS_DEFINE_HANDLER(PlayerManager, ItemCatch)
{
	Debug::Out("PlayerManager : handleItemCatchEvent");

	//PlayerPtr player = evt->getPlayer();
	ItemPtr item = evt->getItem();

	/*player->addLife(evt->getLife());
	player->addSpecial(evt->getSpecial());
	player->addScore(evt->getScore());*/
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
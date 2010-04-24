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

PlayerManager::PlayerManager(Ogre::SceneManager* sceneManager) :
mSceneManager(0)
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

PlayerPtr PlayerManager::createPlayer(Ogre::String name, Ogre::String name_model, Ogre::String mesh)
{
	// Player node
	Ogre::Entity* playerMesh = mSceneManager->createEntity("rwyvern", mesh);
	Ogre::SceneNode* playerSceneNode = mSceneManager->getRootSceneNode()->createChildSceneNode("rwyvern");
	Ogre::SceneNode* modelSceneNode = playerSceneNode->createChildSceneNode("rwyvern_model");
	modelSceneNode->attachObject(playerMesh);

	// Center node for player movement and camera targeting
	SceneNode* autoTrackingNode = mSceneManager->getRootSceneNode()->createChildSceneNode("Center");
	playerSceneNode->setAutoTracking(false, autoTrackingNode, Vector3::UNIT_X);

	PlayerPtr player = PlayerPtr(new Player(name));
	player->initialize( playerMesh, playerSceneNode, modelSceneNode ,autoTrackingNode );

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
	mPlayerMap.erase(name);
	//mPlayerList.erase();

	mSceneManager->destroyEntity(name);//getRootSceneNode()->removeChild(name);

	return true;
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
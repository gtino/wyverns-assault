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

PlayerManager::PlayerManager()
{
	initializeVariables();
}

PlayerManager::~PlayerManager()
{

}

void PlayerManager::initializeVariables()
{
	mSceneManager		=	NULL;	
	mPlayerMesh			=	NULL;
	mPlayerSceneNode	=	NULL;
}

void PlayerManager::initialize(String name, String mesh, SceneManager *levelSceneManager, Vector3 position)
{
	mSceneManager = levelSceneManager;

	// Create player
	mPlayerMesh = mSceneManager->createEntity(name, mesh);
	mPlayerSceneNode = mSceneManager->getRootSceneNode()->createChildSceneNode(name);
	mPlayerSceneNode->attachObject(mPlayerMesh);

	// Positioning
	mPlayerSceneNode->setPosition(position);
	//mPlayerSceneNode->setScale(4, 4, 4);

	// Center node for player movement and camera targeting
	SceneNode* centerNode = mSceneManager->getRootSceneNode()->createChildSceneNode("Center");
	centerNode->setPosition(position * Vector3::UNIT_Y);
	/*Entity* cube = mSceneManager->createEntity("CenterCube", "axes.mesh");
	centerNode->attachObject(cube);	
	centerNode->setScale(100, 100, 100);*/

	mPlayerSceneNode->setAutoTracking(true, centerNode, Vector3::UNIT_X);
	
	// Animations
	mPlayerAnimation = mPlayerMesh->getAnimationState("Iddle_01");
	mPlayerAnimation->setEnabled(true);
	mPlayerAnimation->setLoop(true);
}

void PlayerManager::finalize()
{
	initializeVariables();
}


void PlayerManager::move(Real x, Real y, Real z)
{
	// Direction vector
	Vector3 direction = Vector3(x, y, -z);

	if (direction != Vector3::ZERO)
	{
		// Change autotrack axis for facing movement direction
		mPlayerSceneNode->setAutoTracking(true, mSceneManager->getSceneNode("Center"), direction);

		// Translate forward (allways move forward into facing direction)
		mPlayerSceneNode->translate(Vector3(0, 0, 1)*SPEED, Ogre::Node::TransformSpace::TS_LOCAL);

		mPlayerAnimation = mPlayerMesh->getAnimationState("Run");
		mPlayerAnimation->setEnabled(true);
		mPlayerAnimation->setLoop(true);
	}
	else
	{
		mPlayerAnimation = mPlayerMesh->getAnimationState("Iddle_01");
		mPlayerAnimation->setEnabled(true);
		mPlayerAnimation->setLoop(true);
	}
}

void PlayerManager::updateAnimation(float elapsedSeconds)
{
	mPlayerAnimation->addTime(elapsedSeconds);
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
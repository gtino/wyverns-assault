#include "..\include\Scenario\DotSceneLoader.h"

#include <Ogre.h>
#include <tinyxml.h>

#include "..\include\Entity\Enemy\Enemy.h"
#include "..\include\Entity\Item\Item.h"

using namespace std;
using namespace Ogre;
using namespace WyvernsAssault;

void DotSceneLoader::parseDotScene(const String &SceneName, 
								   const String &groupName, 
								   SceneManager *levelSceneManager, 
								   WyvernsAssault::ScenarioManagerPtr scenarioManager,
								   WyvernsAssault::CameraManagerPtr cameraManager, 
								   WyvernsAssault::LightsManagerPtr lightsManager,
								   WyvernsAssault::EnemyManagerPtr enemysManager, 
								   WyvernsAssault::PhysicsManagerPtr physicsManager, 
								   WyvernsAssault::ItemManagerPtr itemsManager, 
								   WyvernsAssault::ParticleManagerPtr particleManager, 
								   SceneNode *pAttachNode, 
								   const String &sPrependNode)
{

	//Set SceneManager
	mSceneMgr = levelSceneManager;

	// Set ScenarioManager
	mScenarioManager = scenarioManager;

	//Set CameraManager
	mCameraManager = cameraManager;

	//Set LightsManager
	mLightsManager = lightsManager;

	//Set EnemyManager
	mEnemyManager = enemysManager;

	//Set PhysicsManager
	mPhysicsManager = physicsManager;

	//Set ItemsManager
	mItemManager = itemsManager;

	//Set ParticleManager
	mParticleManager = particleManager;

	//Set up shared object values
	m_sGroupName = groupName;
	m_sPrependNode = sPrependNode;
	
	staticObjects.clear();
	dynamicObjects.clear();

	TiXmlDocument   *XMLDoc = 0;
	TiXmlElement   *XMLRoot;

	try
	{
		//Strip the path
		Ogre::String basename, path;
		Ogre::StringUtil::splitFilename(SceneName, basename, path);

		//Open and read XML file
		DataStreamPtr pStream = ResourceGroupManager::getSingleton().openResource( basename, groupName );

		String data = pStream->getAsString();
		// Open the .scene File
		XMLDoc = new TiXmlDocument();
		XMLDoc->Parse( data.c_str() );
		pStream->close();
		pStream.setNull();

		if( XMLDoc->Error() )
		{
			//We'll just log, and continue on gracefully
			LogManager::getSingleton().logMessage("[DotSceneLoader] The TiXmlDocument reported an error");
			delete XMLDoc;
			return;
		}
	}
	catch(...)
	{
		LogManager::getSingleton().logMessage("[DotSceneLoader] Error creating TiXmlDocument");
		delete XMLDoc;
		return;
	}

	// Validate the File
	XMLRoot = XMLDoc->RootElement();
	if( String( XMLRoot->Value()) != "scene"  ) {
		LogManager::getSingleton().logMessage( "[DotSceneLoader] Error: Invalid .scene File. Missing <scene>" );
		delete XMLDoc;
		return;
	}

	// figure out where to attach any nodes we create
	mAttachNode = pAttachNode;
	if(!mAttachNode)
		mAttachNode = mSceneMgr->getRootSceneNode();

	// Process the scene
	processScene(XMLRoot);

	// Close the XML File
	delete XMLDoc;
}

void DotSceneLoader::processScene(TiXmlElement *XMLRoot)
{
	// Process the scene parameters
	String version = getAttrib(XMLRoot, "formatVersion", "unknown");

	String message = "[DotSceneLoader] Parsing dotScene file with version " + version;
	if(XMLRoot->Attribute("ID"))
		message += ", id " + String(XMLRoot->Attribute("ID"));
	if(XMLRoot->Attribute("sceneManager"))
		message += ", scene manager " + String(XMLRoot->Attribute("sceneManager"));
	if(XMLRoot->Attribute("minOgreVersion"))
		message += ", min. Ogre version " + String(XMLRoot->Attribute("minOgreVersion"));
	if(XMLRoot->Attribute("author"))
		message += ", author " + String(XMLRoot->Attribute("author"));

	LogManager::getSingleton().logMessage(message);

	TiXmlElement *pElement;

	// Process nodes
	pElement = XMLRoot->FirstChildElement("nodes");
	if(pElement)
		processNodes(pElement);
	
	LogManager::getSingleton().logMessage("[DotSceneLoader] Nodes processed.");
	
	// Process cameras
	pElement = XMLRoot->FirstChildElement("cameras");
	if(pElement){
		processCameras(pElement);
	}

	LogManager::getSingleton().logMessage("[DotSceneLoader] Camera processed.");

	// Process lights
	pElement = XMLRoot->FirstChildElement("lights");
	if(pElement)
		processLights(pElement);

	LogManager::getSingleton().logMessage("[DotSceneLoader] Light processed.");

	// Process skyBox
	pElement = XMLRoot->FirstChildElement("skyBox");
	if(pElement)
		processSkyBox(pElement);

	LogManager::getSingleton().logMessage("[DotSceneLoader] SkyBox processed.");
	
	// Process skyDome
	pElement = XMLRoot->FirstChildElement("skyDome");
	if(pElement)
		processSkyDome(pElement);

	LogManager::getSingleton().logMessage("[DotSceneLoader] SkyDome processed.");

	// Process game areas
	pElement = XMLRoot->FirstChildElement("gameAreas");
	if(pElement)
		processGameAreas(pElement);

	LogManager::getSingleton().logMessage("[DotSceneLoader] Game areas processed.");

	// Process camera segments
	pElement = XMLRoot->FirstChildElement("cameraSegments");
	if(pElement)
		processCameraSegments(pElement);

	LogManager::getSingleton().logMessage("[DotSceneLoader] Camera segments processed.");

	// Process phyics
	pElement = XMLRoot->FirstChildElement("physics");
	if(pElement)
		processPhysics(pElement);

	LogManager::getSingleton().logMessage("[DotSceneLoader] Physics processed.");

}

void DotSceneLoader::processNodes(TiXmlElement *XMLNode)
{
	TiXmlElement *pElement;

	// Process node
	pElement = XMLNode->FirstChildElement("node");
	while(pElement)
	{
		processNode(pElement);
		pElement = pElement->NextSiblingElement("node");
	}
		
	// Process position
	pElement = XMLNode->FirstChildElement("position");
	if(pElement)
	{
		mAttachNode->setPosition(parseVector3(pElement));
		mAttachNode->setInitialState();
	}

	// Process rotation
	pElement = XMLNode->FirstChildElement("rotation");
	if(pElement)
	{
		mAttachNode->setOrientation(parseQuaternion(pElement));
		mAttachNode->setInitialState();
	}

	// Process scale
	pElement = XMLNode->FirstChildElement("scale");
	if(pElement)
	{
		mAttachNode->setScale(parseVector3(pElement));
		mAttachNode->setInitialState();
	}

}
void DotSceneLoader::processCameras(TiXmlElement *XMLNode)
{
	TiXmlElement *pElement;

	// Process camera
	pElement = XMLNode->FirstChildElement("camera");
	while(pElement)
	{
		processCamera(pElement);
		pElement = pElement->NextSiblingElement("camera");
	}
}

void DotSceneLoader::processPhysics(TiXmlElement *XMLNode)
{
	TiXmlElement *pElement;
	TiXmlElement *pElementEntity;
	TiXmlElement *pElementGL;
	TiXmlElement *pElementPosition;
	TiXmlElement *pElementScale;

	// Process physics element
	pElement = XMLNode->FirstChildElement("physicGround");
	while(pElement)
	{
		Ogre::String name;
		Ogre::String mesh;
		Ogre::String maskType;
		Ogre::Vector3 position = Vector3::ZERO;
		Ogre::Vector3 scale= Vector3::UNIT_SCALE;
		
		// Process position
		pElementPosition = pElement->FirstChildElement("position");
		if(pElementPosition)
		{
			position = parseVector3(pElementPosition);
		}

		// Process scale
		pElementScale = pElement->FirstChildElement("scale");
		if(pElementScale)
		{
			scale = parseVector3(pElementScale);
		}

		// Process ground levels
		pElementGL = pElement->FirstChildElement("groundLevel"); 
		while(pElementGL)
		{

			pElementEntity = pElementGL->FirstChildElement("entity"); 
			
			name = getAttrib(pElementEntity, "name");
			mesh = getAttrib(pElementEntity, "meshFile");
			maskType = getAttrib(pElementEntity, "maskType");

			// Create ground physic element
			if(maskType == "GROUND_MASK") 
				mPhysicsManager->addPhysicScenario(mesh, name, WyvernsAssault::GROUND_MASK, position, scale);
			else if(maskType == "WALL_MASK") 
				mPhysicsManager->addPhysicScenario(mesh, name, WyvernsAssault::WALL_MASK, position, scale);
			else
				mPhysicsManager->addPhysicScenario(mesh, name, WyvernsAssault::GROUND_MASK, position, scale);

			pElementGL = pElementGL->NextSiblingElement("groundLevel");
		}

		pElement = pElement->NextSiblingElement("physicGround");
	}
		
}

void DotSceneLoader::processLights(TiXmlElement *XMLNode)
{
	TiXmlElement *pElement;

	// Process light
	pElement = XMLNode->FirstChildElement("light");
	while(pElement)
	{
		processLight(pElement);
		pElement = pElement->NextSiblingElement("light");
	}
}

void DotSceneLoader::processNode(TiXmlElement *XMLNode, SceneNode *pParent)
{
	// Construct the node's name
	String name = m_sPrependNode + getAttrib(XMLNode, "name");

	// Create the scene node
	SceneNode *pNode;
	if(name.empty())
	{
		// Let Ogre choose the name
		if(pParent)
			pNode = pParent->createChildSceneNode();
		else
			pNode = mAttachNode->createChildSceneNode();
	}
	else
	{
		// Provide the name
		if(pParent)
			pNode = pParent->createChildSceneNode(name);
		else
			pNode = mAttachNode->createChildSceneNode(name);
	}

	// Process other attributes
	String id = getAttrib(XMLNode, "id");
	bool isTarget = getAttribBool(XMLNode, "isTarget");

	TiXmlElement *pElement;

	// Process position
	pElement = XMLNode->FirstChildElement("position");
	if(pElement)
	{
		pNode->setPosition(parseVector3(pElement));
		pNode->setInitialState();
	}

	// Process rotation
	pElement = XMLNode->FirstChildElement("rotation");
	if(pElement)
	{
		pNode->setOrientation(parseQuaternion(pElement));
		pNode->setInitialState();
	}

	// Process scale
	pElement = XMLNode->FirstChildElement("scale");
	if(pElement)
	{
		pNode->setScale(parseVector3(pElement));
		pNode->setInitialState();
	}

	// Process yaw
	pElement = XMLNode->FirstChildElement("yaw");
	if(pElement)
	{
		pNode->yaw(Ogre::Radian(getAttribReal(pElement,"angle",0)));
		pNode->setInitialState();
	}

	// Process lookTarget
	pElement = XMLNode->FirstChildElement("lookTarget");
	if(pElement)
		processLookTarget(pElement, pNode);

	// Process trackTarget
	pElement = XMLNode->FirstChildElement("trackTarget");
	if(pElement)
		processTrackTarget(pElement, pNode);

	// Process node
	pElement = XMLNode->FirstChildElement("node");
	while(pElement)
	{
		processNode(pElement, pNode);
		pElement = pElement->NextSiblingElement("node");
	}

	// Process entity
	pElement = XMLNode->FirstChildElement("entity");
	while(pElement)
	{
		processEntity(pElement, pNode);
		pElement = pElement->NextSiblingElement("entity");
	}

	// Process light
	pElement = XMLNode->FirstChildElement("lights");
	while(pElement)
	{
		processLights(pElement);
		pElement = pElement->NextSiblingElement("lights");
	}

	// Process camera
	pElement = XMLNode->FirstChildElement("cameras");
	while(pElement)
	{
		processCameras(pElement);
		pElement = pElement->NextSiblingElement("cameras");
	}
}

void DotSceneLoader::processCamera(TiXmlElement *XMLNode, SceneNode *pParent)
{
	Vector3 position = Vector3::ZERO;
	Vector3 lookAt = Vector3::ZERO;
	TiXmlElement *pElement;
	
	int id = getAttribInt(XMLNode, "id");

	// Process position
	pElement = XMLNode->FirstChildElement("position");
	if(pElement)
	{
		position = parseVector3(pElement);
	}
	
	pElement = XMLNode->FirstChildElement("lookAt");
	if(pElement)
	{
		lookAt = parseVector3(pElement);
	}

	mCameraManager->setFixedCamera(id, position, lookAt);
}

void DotSceneLoader::processLight(TiXmlElement *XMLNode, SceneNode *pParent)
{
	String type, name;
	Light::LightTypes lType;
	Vector3 position, direction;
	ColourValue diffuse, specular;

	TiXmlElement *pElement;

	type = getAttrib(XMLNode, "type");

	// Process light type
	if(type == "ambient")
	{
		// Special light type, only color
		pElement = XMLNode->FirstChildElement("color");
		if(pElement)
		{
			diffuse = parseColour(pElement);
			mLightsManager->setAmbientLight(diffuse);
		}
	}
	else
	{
		// Other light types
		name = getAttrib(XMLNode, "name");
		if (type == "point")
		{
			lType = Light::LT_POINT;
			// Process position
			pElement = XMLNode->FirstChildElement("position");
			if(pElement)
			{
				position = parseVector3(pElement);
			}
			// Process diffuse color
			pElement = XMLNode->FirstChildElement("diffuse");
			if(pElement)
			{
				diffuse = parseColour(pElement);
			}
			// Process specular color
			pElement = XMLNode->FirstChildElement("specular");
			if(pElement)
			{
				specular = parseColour(pElement);
			}
			mLightsManager->createLight(name, lType, diffuse, specular, position);
		}
		else if(type == "directional")
		{
			lType = Light::LT_DIRECTIONAL;
			// Process direction
			pElement = XMLNode->FirstChildElement("direction");
			if(pElement)
			{
				direction = parseVector3(pElement);
			}
			// Process diffuse color
			pElement = XMLNode->FirstChildElement("diffuse");
			if(pElement)
			{
				diffuse = parseColour(pElement);
			}
			// Process specular color
			pElement = XMLNode->FirstChildElement("specular");
			if(pElement)
			{
				specular = parseColour(pElement);
			}
			mLightsManager->createLight(name, lType, diffuse, specular, Vector3::ZERO, direction);
		}
		else if(type == "spot")
		{
			lType = Light::LT_SPOTLIGHT;
			// Process position
			pElement = XMLNode->FirstChildElement("position");
			if(pElement)
			{
				position = parseVector3(pElement);
			}
			// Process direction
			pElement = XMLNode->FirstChildElement("direction");
			if(pElement)
			{
				direction = parseVector3(pElement);
			}
			// Process diffuse color
			pElement = XMLNode->FirstChildElement("diffuse");
			if(pElement)
			{
				diffuse = parseColour(pElement);
			}
			// Process specular color
			pElement = XMLNode->FirstChildElement("specular");
			if(pElement)
			{
				specular = parseColour(pElement);
			}
			mLightsManager->createLight(name, lType, diffuse, specular, position, direction);
		}
	}
}

void DotSceneLoader::processLookTarget(TiXmlElement *XMLNode, SceneNode *pParent)
{

	// Process attributes
	String nodeName = getAttrib(XMLNode, "nodeName");

	Node::TransformSpace relativeTo = Node::TS_PARENT;
	String sValue = getAttrib(XMLNode, "relativeTo");
	if(sValue == "local")
		relativeTo = Node::TS_LOCAL;
	else if(sValue == "parent")
		relativeTo = Node::TS_PARENT;
	else if(sValue == "world")
		relativeTo = Node::TS_WORLD;

	TiXmlElement *pElement;

	// Process position
	Vector3 position;
	pElement = XMLNode->FirstChildElement("position");
	if(pElement)
		position = parseVector3(pElement);

	// Process localDirection
	Vector3 localDirection = Vector3::NEGATIVE_UNIT_Z;
	pElement = XMLNode->FirstChildElement("localDirection");
	if(pElement)
		localDirection = parseVector3(pElement);

	// Setup the look target
	try
	{
		if(!nodeName.empty())
		{
			SceneNode *pLookNode = mSceneMgr->getSceneNode(nodeName);
			position = pLookNode->_getDerivedPosition();
		}

		pParent->lookAt(position, relativeTo, localDirection);
	}
	catch(Ogre::Exception &/*e*/)
	{
		LogManager::getSingleton().logMessage("[DotSceneLoader] Error processing a look target!");
	}
}

void DotSceneLoader::processTrackTarget(TiXmlElement *XMLNode, SceneNode *pParent)
{
	// Process attributes
	String nodeName = getAttrib(XMLNode, "nodeName");

	TiXmlElement *pElement;

	// Process localDirection (?)
	Vector3 localDirection = Vector3::NEGATIVE_UNIT_Z;
	pElement = XMLNode->FirstChildElement("localDirection");
	if(pElement)
		localDirection = parseVector3(pElement);

	// Process offset (?)
	Vector3 offset = Vector3::ZERO;
	pElement = XMLNode->FirstChildElement("offset");
	if(pElement)
		offset = parseVector3(pElement);

	// Setup the track target
	try
	{
		SceneNode *pTrackNode = mSceneMgr->getSceneNode(nodeName);
		pParent->setAutoTracking(true, pTrackNode, localDirection, offset);
	}
	catch(Ogre::Exception &/*e*/)
	{
		LogManager::getSingleton().logMessage("[DotSceneLoader] Error processing a track target!");
	}
}

void DotSceneLoader::processEntity(TiXmlElement *XMLNode, SceneNode *pParent)
{
	// Process attributes
	String name = getAttrib(XMLNode, "name");
	String id = getAttrib(XMLNode, "id");
	String type = getAttrib(XMLNode, "type");	
	String meshFile = getAttrib(XMLNode, "meshFile");
	String materialFile = getAttrib(XMLNode, "materialFile");
	bool isStatic = getAttribBool(XMLNode, "static", false);
	bool castShadows = getAttribBool(XMLNode, "castShadows", true);

	// TEMP: Maintain a list of static and dynamic objects
	if(isStatic)
		staticObjects.push_back(name);
	else
		dynamicObjects.push_back(name);

	TiXmlElement *pElement;

	// Create the entity
	Entity *pEntity = 0;
	try
	{
		//
		// Create entity
		//
		MeshManager::getSingleton().load(meshFile, m_sGroupName);
		pEntity = mSceneMgr->createEntity(name, meshFile);
		pEntity->setCastShadows(castShadows);
		pEntity->setQueryFlags(SceneManager::ENTITY_TYPE_MASK);

		//
		// Use the Object Manager to create an instance for the new Object
		//
		if( type == "DynamicObject" )
		{
			ObjectPtr object = mScenarioManager->createObject(ObjectTypes::DynamicObject, name, pEntity, pParent);
			mPhysicsManager->addPhysicObject(object);
		}
		else if( type == "Enemy" )
		{
			// Get own atributes
			String subType = getAttrib(XMLNode, "subType");

			Enemy::EnemyParameters params;

			params.animationTree = getAttrib(XMLNode, "animationTree");
			params.life = getAttribReal(XMLNode, "life");
			params.points = getAttribReal(XMLNode, "points");
			params.speed = getAttribReal(XMLNode, "speed");
			params.damage = getAttribReal(XMLNode, "damage");
			params.specialDamage = getAttribReal(XMLNode, "specialDamage");
			params.height = getAttribReal(XMLNode, "height");
			params.dieMesh = getAttrib(XMLNode, "dieMesh");
			params.dieAnimation = getAttrib(XMLNode, "dieAnimation");
			params.physicMesh = getAttrib(XMLNode, "physicMesh");

			// Add to EnemyManager
			EnemyPtr enemy = mEnemyManager->createEnemy(Enemy::StringToType(subType), name, pEntity, pParent, params);
			// Add the enemy to the physics manager
			mPhysicsManager->addPhysicEnemy(enemy);
		}
		else if( type == "Item" )
		{
			// Get own atributes
			String subType = getAttrib(XMLNode, "subType");

			Item::ItemParameters params;

			params.life = getAttribReal(XMLNode, "life");
			params.points = getAttribReal(XMLNode, "points");
			params.drunkTime = getAttribReal(XMLNode, "drunkTime");

			// Add to ItemManager
			ItemPtr item = mItemManager->createItem(Item::StringToType(subType), name, pEntity, pParent, params);
			// Add the item to the physics manager
			mPhysicsManager->addPhysicItem(item);
		}
		else
		{
			ObjectPtr object = mScenarioManager->createObject(ObjectTypes::Default, name, pEntity, pParent);
		}

		if(!materialFile.empty())
			pEntity->setMaterialName(materialFile);

		// Process subentities
		pElement = XMLNode->FirstChildElement("subentities");
		while(pElement)
		{
			processSubEntities(pElement, pEntity);
			pElement = pElement->NextSiblingElement("subentities");
		}
	}
	catch(Ogre::Exception &/*e*/)
	{
		LogManager::getSingleton().logMessage("[DotSceneLoader] Error loading an entity!");
	}

}

void DotSceneLoader::processSubEntities(TiXmlElement *XMLNode, Entity *pEntity)
{
		// Process subentities
		TiXmlElement* pElement = XMLNode->FirstChildElement("subentity");
		while(pElement)
		{
			processSubEntity(pElement, pEntity);
			pElement = pElement->NextSiblingElement("subentity");
		}
}

void DotSceneLoader::processSubEntity(TiXmlElement *XMLNode, Entity *pEntity)
{
	// Process attributes
	int index = getAttribInt(XMLNode, "index");
	String materialName = getAttrib(XMLNode, "materialName");

	TiXmlElement *pElement;

	// Create the entity
	Ogre::SubEntity *pSubEntity = 0;
	try
	{
		pSubEntity = pEntity->getSubEntity(index);

		if(!materialName.empty())
			pSubEntity->setMaterialName(materialName);
	}
	catch(Ogre::Exception &/*e*/)
	{
		LogManager::getSingleton().logMessage("[DotSceneLoader] Error setting a subentity!");
	}
}

void DotSceneLoader::processGameAreas(TiXmlElement *XMLNode)
{
	TiXmlElement *pElement;

	// Process game area
	pElement = XMLNode->FirstChildElement("gameArea");
	while(pElement)
	{
		processGameArea(pElement);
		pElement = pElement->NextSiblingElement("gameArea");
	}
}

void DotSceneLoader::processCameraSegments(TiXmlElement *XMLNode)
{
	TiXmlElement *pElement;

	// Process camera segment
	pElement = XMLNode->FirstChildElement("cameraSegment");
	while(pElement)
	{
		processCameraSegment(pElement);
		pElement = pElement->NextSiblingElement("cameraSegment");
	}
}

void DotSceneLoader::processGameArea(TiXmlElement *XMLNode, SceneNode *pParent)
{
	Vector3 beginNear;
	Vector3 endNear;
	Vector3 beginFar;
	Vector3 endFar;
	TiXmlElement *pElement;

	// Process begin near
	pElement = XMLNode->FirstChildElement("beginNear");
	if(pElement)
	{
		beginNear = parseVector3(pElement);
	}
	// Process end near
	pElement = XMLNode->FirstChildElement("endNear");
	if(pElement)
	{
		endNear = parseVector3(pElement);
	}
	// Process begin far
	pElement = XMLNode->FirstChildElement("beginFar");
	if(pElement)
	{
		beginFar = parseVector3(pElement);
	}
	// Process end far
	pElement = XMLNode->FirstChildElement("endFar");
	if(pElement)
	{
		endFar = parseVector3(pElement);
	}

	mCameraManager->addGameArea(beginNear, endNear, beginFar, endFar);
}

void DotSceneLoader::processCameraSegment(TiXmlElement *XMLNode, SceneNode *pParent)
{
	Vector3 positionBegin;
	Vector3 positionEnd;
	Vector3 lookAtBegin;
	Vector3 lookAtEnd;
	TiXmlElement *pElement;

	// Process position begin
	pElement = XMLNode->FirstChildElement("positionBegin");
	if(pElement)
	{
		positionBegin = parseVector3(pElement);
	}
	// Process position end
	pElement = XMLNode->FirstChildElement("positionEnd");
	if(pElement)
	{
		positionEnd = parseVector3(pElement);
	}
	// Process look at begin
	pElement = XMLNode->FirstChildElement("lookAtBegin");
	if(pElement)
	{
		lookAtBegin = parseVector3(pElement);
	}
	// Process look at end
	pElement = XMLNode->FirstChildElement("lookAtEnd");
	if(pElement)
	{
		lookAtEnd = parseVector3(pElement);
	}

	mCameraManager->addCameraSegment(positionBegin, positionEnd, lookAtBegin, lookAtEnd);
}

void DotSceneLoader::processSkyBox(TiXmlElement *XMLNode)
{
	// Process attributes
	String material = getAttrib(XMLNode, "material");
	Real distance = getAttribReal(XMLNode, "distance", 100);
	bool drawFirst = getAttribBool(XMLNode, "drawFirst", true);

	// Setup the sky box
	mSceneMgr->setSkyBox(true, material, distance, drawFirst);
}

void DotSceneLoader::processFog(TiXmlElement *XMLNode)
{
	// Process attributes
	//TiXmlElement* pElement = XMLNode->FirstChildElement("mode");
	//if(pElement)
	//{
	//	diffuse = parseFogMode(pElement);
	//}
	//pElement = XMLNode->FirstChildElement("colour");
	//if(pElement)
	//{
	//	diffuse = parseColour(pElement);
	//}
	Real expDensity = getAttribReal(XMLNode, "expDensity", 0.001);
	Real linearStart = getAttribReal(XMLNode, "linearStart", 0.0);
	Real linearEnd = getAttribBool(XMLNode, "linearEnd", 1.0);

	// Setup the sky dome
	//mSceneMgr->setFog();
}

void DotSceneLoader::processSkyDome(TiXmlElement *XMLNode)
{
	// Process attributes
	String material = getAttrib(XMLNode, "material");
	Real curvature = getAttribReal(XMLNode, "curvature", 10);
	Real tiling = getAttribReal(XMLNode, "tiling", 8);
	Real distance = getAttribReal(XMLNode, "distance", 4000);
	bool drawFirst = getAttribBool(XMLNode, "drawFirst", true);

	// Setup the sky dome
	mSceneMgr->setSkyDome(true, material, curvature, tiling, distance, drawFirst);
}

void DotSceneLoader::processLightRange(TiXmlElement *XMLNode, Light *pLight)
{
	// Process attributes
	Real inner = getAttribReal(XMLNode, "inner");
	Real outer = getAttribReal(XMLNode, "outer");
	Real falloff = getAttribReal(XMLNode, "falloff", 1.0);

	// Setup the light range
	pLight->setSpotlightRange(Angle(inner), Angle(outer), falloff);
}

void DotSceneLoader::processLightAttenuation(TiXmlElement *XMLNode, Light *pLight)
{
	// Process attributes
	Real range = getAttribReal(XMLNode, "range");
	Real constant = getAttribReal(XMLNode, "constant");
	Real linear = getAttribReal(XMLNode, "linear");
	Real quadratic = getAttribReal(XMLNode, "quadratic");

	// Setup the light attenuation
	pLight->setAttenuation(range, constant, linear, quadratic);
}


String DotSceneLoader::getAttrib(TiXmlElement *XMLNode, const String &attrib, const String &defaultValue)
{
	if(XMLNode->Attribute(attrib.c_str()))
		return XMLNode->Attribute(attrib.c_str());
	else
		return defaultValue;
}
Real DotSceneLoader::getAttribReal(TiXmlElement *XMLNode, const String &attrib, Real defaultValue)
{
	if(XMLNode->Attribute(attrib.c_str()))
		return StringConverter::parseReal(XMLNode->Attribute(attrib.c_str()));
	else
		return defaultValue;
}

int DotSceneLoader::getAttribInt(TiXmlElement *XMLNode, const String &attrib, int defaultValue)
{
	if(XMLNode->Attribute(attrib.c_str()))
		return StringConverter::parseInt(XMLNode->Attribute(attrib.c_str()));
	else
		return defaultValue;
}

bool DotSceneLoader::getAttribBool(TiXmlElement *XMLNode, const String &attrib, bool defaultValue)
{
	if(!XMLNode->Attribute(attrib.c_str()))
		return defaultValue;

	if(String(XMLNode->Attribute(attrib.c_str())) == "true")
		return true;

	return false;
}

Vector3 DotSceneLoader::parseVector3(TiXmlElement *XMLNode)
{
	return Vector3(
		StringConverter::parseReal(XMLNode->Attribute("x")),
		StringConverter::parseReal(XMLNode->Attribute("y")),
		StringConverter::parseReal(XMLNode->Attribute("z"))
	);
}

Quaternion DotSceneLoader::parseQuaternion(TiXmlElement *XMLNode)
{
	//! @todo Fix this crap!

	Quaternion orientation;

	if(XMLNode->Attribute("qx"))
	{
		orientation.x = StringConverter::parseReal(XMLNode->Attribute("qx"));
		orientation.y = StringConverter::parseReal(XMLNode->Attribute("qy"));
		orientation.z = StringConverter::parseReal(XMLNode->Attribute("qz"));
		orientation.w = StringConverter::parseReal(XMLNode->Attribute("qw"));
	}
	else if(XMLNode->Attribute("axisX"))
	{
		Vector3 axis;
		axis.x = StringConverter::parseReal(XMLNode->Attribute("axisX"));
		axis.y = StringConverter::parseReal(XMLNode->Attribute("axisY"));
		axis.z = StringConverter::parseReal(XMLNode->Attribute("axisZ"));
		Real angle = StringConverter::parseReal(XMLNode->Attribute("angle"));;
		orientation.FromAngleAxis(Ogre::Angle(angle), axis);
	}
	else if(XMLNode->Attribute("angleX"))
	{
		Vector3 axis;
		axis.x = StringConverter::parseReal(XMLNode->Attribute("angleX"));
		axis.y = StringConverter::parseReal(XMLNode->Attribute("angleY"));
		axis.z = StringConverter::parseReal(XMLNode->Attribute("angleZ"));
		//orientation.FromAxes(&axis);
		//orientation.F
	}

	return orientation;
}

ColourValue DotSceneLoader::parseColour(TiXmlElement *XMLNode)
{
	return ColourValue(
		StringConverter::parseReal(XMLNode->Attribute("r")),
		StringConverter::parseReal(XMLNode->Attribute("g")),
		StringConverter::parseReal(XMLNode->Attribute("b")),
		XMLNode->Attribute("a") != NULL ? StringConverter::parseReal(XMLNode->Attribute("a")) : 1
	);
}

String DotSceneLoader::getProperty(const String &ndNm, const String &prop)
{
	for ( unsigned int i = 0 ; i < nodeProperties.size(); i++ )
	{
		if ( nodeProperties[i].nodeName == ndNm && nodeProperties[i].propertyNm == prop )
		{
			return nodeProperties[i].valueName;
		}
	}

	return "";
}
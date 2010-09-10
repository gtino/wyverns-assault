#ifndef DOT_SCENELOADER_H
#define DOT_SCENELOADER_H

// Includes
#include "OgreString.h"
#include "OgreVector3.h"
#include "OgreQuaternion.h"
#include "vector"

#include "..\Scenario\ScenarioManager.h"
#include "..\Camera\CameraManager.h"
#include "..\Graphics\LightsManager.h"
#include "..\Physics\PhysicsManager.h"
#include "..\Entity\Enemy\EnemyManager.h"
#include "..\Entity\Item\ItemManager.h"
#include "..\Particle\ParticleManager.h"
#include "..\GameArea\GameAreaManager.h"

 
// Forward declarations
class TiXmlElement; 

namespace Ogre
{
	// Forward declarations
	class SceneManager;
	class SceneNode;

	class nodeProperty
	{
	public:
		String nodeName;
		String propertyNm;
		String valueName;
		String typeName;

		nodeProperty(const String &node, const String &propertyName, const String &value, const String &type)
			: nodeName(node), propertyNm(propertyName), valueName(value), typeName(type) {}
	};

	class DotSceneLoader
	{
	public:
		DotSceneLoader() : mSceneMgr(0) {}
		virtual ~DotSceneLoader() {}

		void parseDotScene(const String &SceneName, 
			const String &groupName,
			SceneManager *levelSceneManager,
			WyvernsAssault::PlayerManagerPtr playerManager,
			WyvernsAssault::ScenarioManagerPtr scenarioManager,
			WyvernsAssault::CameraManagerPtr cameraManager, 
			WyvernsAssault::LightsManagerPtr lightsManager,
			WyvernsAssault::EnemyManagerPtr enemysManager,
			WyvernsAssault::PhysicsManagerPtr physicsManager, 
			WyvernsAssault::ItemManagerPtr itemsManager, 
			WyvernsAssault::ParticleManagerPtr particleManager,
			WyvernsAssault::GameAreaManagerPtr gameAreaManager,
			SceneNode *pAttachNode = NULL, 
			const String &sPrependNode = "");

		String getProperty(const String &ndNm, const String &prop);

		std::vector<nodeProperty> nodeProperties;
		std::vector<String> staticObjects;
		std::vector<String> dynamicObjects;

	protected:
		void processScene(TiXmlElement *XMLRoot);

		void processNodes(TiXmlElement *XMLNode);
		void processNode(TiXmlElement *XMLNode, SceneNode *pParent = 0);

		void processMarkers(TiXmlElement *XMLNode);
		void processMarker(TiXmlElement *XMLNode, SceneNode *pParent = 0);

		void processEntity(TiXmlElement *XMLNode, SceneNode *pParent);
		void processSubEntities(TiXmlElement *XMLNode, Entity *pEntity);
		void processSubEntity(TiXmlElement *XMLNode, Entity *pEntity);

		void processCameras(TiXmlElement *XMLNode);
		void processCamera(TiXmlElement *XMLNode, SceneNode *pParent = 0, int level = 0);

		void processGameAreas(TiXmlElement *XMLNode);
		void processGameArea(TiXmlElement *XMLNode, SceneNode *pParent = 0, int level = 0);
		void processCameraSegments(TiXmlElement *XMLNode);		
		void processCameraSegment(TiXmlElement *XMLNode, SceneNode *pParent = 0, int level = 0);

		void processLights(TiXmlElement *XMLNode);		
		void processLight(TiXmlElement *XMLNode, SceneNode *pParent = 0);
		void processLightRange(TiXmlElement *XMLNode, Light *pLight);
		void processLightAttenuation(TiXmlElement *XMLNode, Light *pLight);
		
		void processPhysics(TiXmlElement *XMLNode);
		
		void processEnvironment(TiXmlElement *XMLNode);
		void processSkyBox(TiXmlElement *XMLNode);
		void processSkyDome(TiXmlElement *XMLNode);
		void processFog(TiXmlElement *XMLNode);
		void processAmbient(TiXmlElement *XMLNode);
		void processShadows(TiXmlElement *XMLNode);

		void processLookTarget(TiXmlElement *XMLNode, SceneNode *pParent);
		void processTrackTarget(TiXmlElement *XMLNode, SceneNode *pParent);

		String getAttrib(TiXmlElement *XMLNode, const String &parameter, const String &defaultValue = "");
		Real getAttribReal(TiXmlElement *XMLNode, const String &parameter, Real defaultValue = 0);
		int getAttribInt(TiXmlElement *XMLNode, const String &parameter, int defaultValue = 0);
		bool getAttribBool(TiXmlElement *XMLNode, const String &parameter, bool defaultValue = false);

		Vector3 parseVector3(TiXmlElement *XMLNode);
		Quaternion parseQuaternion(TiXmlElement *XMLNode);
		ColourValue parseColour(TiXmlElement *XMLNode);

	private:
		SceneManager *mSceneMgr;
		SceneNode *mAttachNode;
		String m_sGroupName;
		String m_sPrependNode;

		WyvernsAssault::PlayerManagerPtr mPlayerManager;
		WyvernsAssault::ScenarioManagerPtr mScenarioManager;
		WyvernsAssault::CameraManagerPtr mCameraManager;
		WyvernsAssault::LightsManagerPtr mLightsManager;
		WyvernsAssault::EnemyManagerPtr mEnemyManager;
		WyvernsAssault::PhysicsManagerPtr mPhysicsManager;
		WyvernsAssault::ItemManagerPtr mItemManager;
		WyvernsAssault::ParticleManagerPtr mParticleManager;
		WyvernsAssault::GameAreaManagerPtr mGameAreaManager;
	};
}

#endif // DOT_SCENELOADER_H

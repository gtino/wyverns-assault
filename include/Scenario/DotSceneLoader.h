#ifndef DOT_SCENELOADER_H
#define DOT_SCENELOADER_H

// Includes
#include "OgreString.h"
#include "OgreVector3.h"
#include "OgreQuaternion.h"
#include "vector"

#include "..\Camera\CameraManager.h"
#include "..\Graphics\LightsManager.h"
#include "..\Physics\PhysicsManager.h"
#include "..\Entity\Enemy\EnemyManager.h"
#include "..\Entity\Item\ItemManager.h"
#include "..\Particle\ParticleManager.h"

 
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
			WyvernsAssault::CameraManagerPtr cameraManager, 
			WyvernsAssault::LightsManagerPtr lightsManager,
			WyvernsAssault::EnemyManagerPtr enemysManager,
			WyvernsAssault::PhysicsManagerPtr physicsManager, 
			WyvernsAssault::ItemManagerPtr itemsManager, 
			WyvernsAssault::ParticleManagerPtr particleManager, 
			SceneNode *pAttachNode = NULL, 
			const String &sPrependNode = "");

		String getProperty(const String &ndNm, const String &prop);

		std::vector<nodeProperty> nodeProperties;
		std::vector<String> staticObjects;
		std::vector<String> dynamicObjects;

	protected:
		void processScene(TiXmlElement *XMLRoot);

		void processNodes(TiXmlElement *XMLNode);
		void processCameras(TiXmlElement *XMLNode);
		void processLights(TiXmlElement *XMLNode);		
		void processEnemys(TiXmlElement *XMLNode);	
		void processSkyBox(TiXmlElement *XMLNode);
		void processSkyDome(TiXmlElement *XMLNode);
		void processSubEntities(TiXmlElement *XMLNode, Entity *pEntity);
		void processGameAreas(TiXmlElement *XMLNode);
		void processCameraSegments(TiXmlElement *XMLNode);
		void processItems(TiXmlElement *XMLNode);
		void processPhysics(TiXmlElement *XMLNode);
		void processAnimals(TiXmlElement *XMLNode);

		void processNode(TiXmlElement *XMLNode, SceneNode *pParent = 0);
		void processCamera(TiXmlElement *XMLNode, SceneNode *pParent = 0);
		void processLight(TiXmlElement *XMLNode, SceneNode *pParent = 0);

		void processLookTarget(TiXmlElement *XMLNode, SceneNode *pParent);
		void processTrackTarget(TiXmlElement *XMLNode, SceneNode *pParent);
		void processEntity(TiXmlElement *XMLNode, SceneNode *pParent);
		void processSubEntity(TiXmlElement *XMLNode, Entity *pEntity);
		void processGameArea(TiXmlElement *XMLNode, SceneNode *pParent = 0);
		void processCameraSegment(TiXmlElement *XMLNode, SceneNode *pParent = 0);

		void processLightRange(TiXmlElement *XMLNode, Light *pLight);
		void processLightAttenuation(TiXmlElement *XMLNode, Light *pLight);

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

		WyvernsAssault::CameraManagerPtr mCameraManager;
		WyvernsAssault::LightsManagerPtr mLightsManager;
		WyvernsAssault::EnemyManagerPtr mEnemyManager;
		WyvernsAssault::PhysicsManagerPtr mPhysicsManager;
		WyvernsAssault::ItemManagerPtr mItemManager;
		WyvernsAssault::ParticleManagerPtr mParticleManager;
	};
}

#endif // DOT_SCENELOADER_H
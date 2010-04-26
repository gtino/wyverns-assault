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

		void parseDotScene(const String &SceneName, const String &groupName,SceneManager *levelSceneManager, WyvernsAssault::CameraManager* cameraManager, WyvernsAssault::LightsManager* lightsManager,WyvernsAssault::EnemyManager* enemysManager ,WyvernsAssault::PhysicsManager* physicsManager ,SceneNode *pAttachNode = NULL, const String &sPrependNode = "");
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
		void processWaypoints(TiXmlElement *XMLNode);

		void processNode(TiXmlElement *XMLNode, SceneNode *pParent = 0);
		void processCamera(TiXmlElement *XMLNode, SceneNode *pParent = 0);
		void processLight(TiXmlElement *XMLNode, SceneNode *pParent = 0);

		void processLookTarget(TiXmlElement *XMLNode, SceneNode *pParent);
		void processTrackTarget(TiXmlElement *XMLNode, SceneNode *pParent);
		void processEntity(TiXmlElement *XMLNode, SceneNode *pParent);
		void processSubEntity(TiXmlElement *XMLNode, Entity *pEntity);
		void processWaypoint(TiXmlElement *XMLNode, SceneNode *pParent = 0);

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
		WyvernsAssault::CameraManager* mCameraManager;
		WyvernsAssault::LightsManager* mLightsManager;
		WyvernsAssault::EnemyManager* mEnemyManager;
		WyvernsAssault::PhysicsManager* mPhysicsManager;
	};
}

#endif // DOT_SCENELOADER_H

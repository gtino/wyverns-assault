#ifndef _OGREODEPREREQS_H_
#define _OGREODEPREREQS_H_

#include "Ogre.h"
//#include "OgreNoMemoryMacros.h"
#include "ode/ode.h"
//#include "OgreMemoryMacros.h"

//#define dSINGLE

#if defined(dSINGLE)

#elif defined(dDOUBLE)

#else
#error You must #define dSINGLE or dDOUBLE in project settings. (preprocessor macros)
#error Make sure it uses same value as ODE project settings.
#error you can also define it in OgreOdePreReqs.h (just after the #include "Ogre.h" )
#endif



namespace OgreOde 
{
    #if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    #   if defined( OGREODE_EXPORTS )
    #       define _OgreOdeExport __declspec( dllexport )
    #   else
    #       define _OgreOdeExport __declspec( dllimport )
    #   endif
    #else
    #   define _OgreOdeExport
    #endif

/*
#ifndef dTriIndex
#if dTRIMESH_16BIT_INDICES
#if dTRIMESH_GIMPACT
	typedef uint32 dTriIndex;
#else // dTRIMESH_GIMPACT
	typedef uint16 dTriIndex;
#endif // dTRIMESH_GIMPACT
#else // dTRIMESH_16BIT_INDICES
	typedef int dTriIndex;
#endif // dTRIMESH_16BIT_INDICES
#endif
*/
	typedef dTriIndex TriangleIndex;


	class World;
	class Space;

	class Body;
	class Mass;

	class JointGroup;
	class Joint;

    class Geometry;
    typedef std::list<Geometry*>   GeometryList;
    typedef std::vector<Geometry*> GeometryArray;

    class RayGeometry;
    class CylinderGeometry;
    class SphereGeometry;
    class InfinitePlaneGeometry;
    class TransformGeometry;
	
	class TriangleMeshData;
	class TriangleMeshDataPtr;
	class TriangleMeshGeometry;
	class BoxGeometry;
    class CapsuleGeometry;


	class Contact;

    class StepListener;

    class StepHandler;
    class BasicStepper;
    class QuickStepper;
    class FastStepper;

    class StepMode;
    class BasicStepMode;
    class QuickStepMode;
    class FastStepMode;



	class CollisionListener;
	class CollisionCallback;
	class MaintainedItem;


	class EntityInformer;
    typedef std::vector<Ogre::Vector3> Vector3Array;
	typedef std::map<unsigned char,Vector3Array* > BoneMapping;
	typedef std::pair<unsigned short,Vector3Array* > BoneMappingKey;

	typedef size_t MaterialID;
	typedef std::map<MaterialID,Contact*> MaterialMap;
    typedef std::pair<MaterialID,MaterialMap* > MaterialMapPair;

    class DebugObject;
    class DebugContact;
    class DebugContactText;
    class DebugNormal;
	
	class TriangleMeshDataManager;
	class TriangleMeshData;
}


#endif


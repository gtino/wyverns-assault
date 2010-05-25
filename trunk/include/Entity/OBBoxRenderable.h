#ifndef OBBOXRENDERABLE_H_
#define OBBOXRENDERABLE_H_

#include <OgreSimpleRenderable.h>
#include <OgreAxisAlignedBox.h>

namespace WyvernsAssault
{
class OBBoxRenderable : public Ogre::SimpleRenderable
{
	Ogre::VertexData vertexes;
	public:
		OBBoxRenderable();
		OBBoxRenderable(Ogre::String material);
		void initialize(Ogre::String material);
		void setupVertices(const Ogre::AxisAlignedBox& aab);
		virtual ~OBBoxRenderable();
		Ogre::Real getSquaredViewDepth(const Ogre::Camera*)const;
		Ogre::Real getBoundingRadius()const;
		virtual void getWorldTransforms (Ogre::Matrix4 *xform)const;
};
}

#endif /*OBBOXRENDERABLE_H_*/
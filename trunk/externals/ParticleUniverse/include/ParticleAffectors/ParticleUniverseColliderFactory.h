/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2009 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#ifndef __PU_COLLIDER_FACTORY_H__
#define __PU_COLLIDER_FACTORY_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseColliderTokens.h"
#include "ParticleUniverseCollider.h"
#include "ParticleUniverseAffectorFactory.h"

namespace ParticleUniverse
{
	/** This factory is just a dummy factory.
    */
	class _ParticleUniverseExport ColliderFactory : public ParticleAffectorFactory
	{
		protected:
			ColliderWriter mColliderWriter;
			ColliderTranslator mColliderTranslator;

		public:
			ColliderFactory(void) {};
	        virtual ~ColliderFactory(void) {};

			/** See ParticleAffectorFactory */
			Ogre::String getAffectorType(void) const
			{
				return "Dummy02"; // Dummy Factory, only for setting up token definitions.
			}

			/** See ParticleAffectorFactory  */
			ParticleAffector* createAffector(void)
			{
				OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "PU: ColliderFactory is a dummy factory.", 
					"ColliderFactory::createAffector");
			}

			/** See ScriptReader */
			virtual bool translateChildProperty(Ogre::ScriptCompiler* compiler, const Ogre::AbstractNodePtr &node)
			{
				return mColliderTranslator.translateChildProperty(compiler, node);
			};

			/** See ScriptReader */
			virtual bool translateChildObject(Ogre::ScriptCompiler* compiler, const Ogre::AbstractNodePtr &node)
			{
				return mColliderTranslator.translateChildObject(compiler, node);
			};

			/*  */
			virtual void write(ParticleScriptSerializer* serializer , const IElement* element)
			{
				// Delegate
				mColliderWriter.write(serializer, element);
			}
	};

}
#endif
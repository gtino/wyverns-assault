/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2009 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#ifndef __PU_FORCE_AFFECTOR_FACTORY_H__
#define __PU_FORCE_AFFECTOR_FACTORY_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseForceAffectorTokens.h"
#include "ParticleUniverseForceAffector.h"
#include "ParticleUniverseAffectorFactory.h"

namespace ParticleUniverse
{
	/** This factory is just a dummy factory.
    */
	class _ParticleUniverseExport ForceAffectorFactory : public ParticleAffectorFactory
	{
		protected:
			ForceAffectorWriter mForceAffectorWriter;
			ForceAffectorTranslator mForceAffectorTranslator;

		public:
			ForceAffectorFactory(void) {};
	        virtual ~ForceAffectorFactory(void) {};

			/** See ParticleAffectorFactory */
			Ogre::String getAffectorType(void) const
			{
				return "Dummy01"; // Dummy Factory, only for setting up token definitions.
			}

			/** See ParticleAffectorFactory  */
			ParticleAffector* createAffector(void)
			{
				OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED, "PU: ForceAffectorFactory is a dummy factory.", 
					"ForceAffectorFactory::createAffector");
			}

			/** See ScriptReader */
			virtual bool translateChildProperty(Ogre::ScriptCompiler* compiler, const Ogre::AbstractNodePtr &node)
			{
				return mForceAffectorTranslator.translateChildProperty(compiler, node);
			};

			/** See ScriptReader */
			virtual bool translateChildObject(Ogre::ScriptCompiler* compiler, const Ogre::AbstractNodePtr &node)
			{
				return mForceAffectorTranslator.translateChildObject(compiler, node);
			};

			/*  */
			virtual void write(ParticleScriptSerializer* serializer, const IElement* element)
			{
				// Delegate
				mForceAffectorWriter.write(serializer, element);
			}
	};

}
#endif

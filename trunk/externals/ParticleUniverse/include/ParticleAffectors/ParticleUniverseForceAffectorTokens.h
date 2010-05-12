/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2009 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#ifndef __PU_FORCE_AFFECTOR_TOKENS_H__
#define __PU_FORCE_AFFECTOR_TOKENS_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseAffectorTokens.h"

namespace ParticleUniverse
{
	/** The ForceAffectorTranslator parses 'ForceAffector' tokens
	*/
	class _ParticleUniverseExport ForceAffectorTranslator : public ScriptTranslator
	{
		public:
			ForceAffectorTranslator(void){};
			~ForceAffectorTranslator(void){};
			virtual bool translateChildProperty(Ogre::ScriptCompiler* compiler, const Ogre::AbstractNodePtr &node);
			virtual bool translateChildObject(Ogre::ScriptCompiler* compiler, const Ogre::AbstractNodePtr &node);
	};
	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	/** 
    */
	class _ParticleUniverseExport ForceAffectorWriter : public ParticleAffectorWriter
	{
		public:

			ForceAffectorWriter(void) {};
			virtual ~ForceAffectorWriter(void) {};

			/** @see
				ScriptWriter::write
			*/
			virtual void write(ParticleScriptSerializer* serializer , const IElement* element);
	};

}
#endif

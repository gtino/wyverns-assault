/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2010 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#ifndef __PU_LIGHT_RENDERER_TOKENS_H__
#define __PU_LIGHT_RENDERER_TOKENS_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseRendererTokens.h"

namespace ParticleUniverse
{
	/** The LightRendererTranslator parses 'LightRenderer' tokens
	*/
	class _ParticleUniverseExport LightRendererTranslator : public ScriptTranslator
	{
		public:
			LightRendererTranslator(void){};
			~LightRendererTranslator(void){};
			virtual bool translateChildProperty(Ogre::ScriptCompiler* compiler, const Ogre::AbstractNodePtr &node);
			virtual bool translateChildObject(Ogre::ScriptCompiler* compiler, const Ogre::AbstractNodePtr &node);
	};
	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------
	//-------------------------------------------------------------------------

	/** 
    */
	class _ParticleUniverseExport LightRendererWriter : public ParticleRendererWriter
	{
		protected:

		public:

			LightRendererWriter(void) {};
			virtual ~LightRendererWriter(void) {};

			/** @see
				ScriptWriter::write
			*/
			virtual void write(ParticleScriptSerializer* serializer, const IElement* element);
	};

}
#endif

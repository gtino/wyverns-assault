/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2010 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#ifndef __PU_DO_EXPIRE_EVENT_HANDLER_TOKENS_H__
#define __PU_DO_EXPIRE_EVENT_HANDLER_TOKENS_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseEventHandlerTokens.h"

namespace ParticleUniverse
{
	/** 
    */
	class _ParticleUniverseExport DoExpireEventHandlerWriter : public ParticleEventHandlerWriter
	{
		public:
			DoExpireEventHandlerWriter(void) {};
			virtual ~DoExpireEventHandlerWriter(void) {};

			/** @see
				ScriptWriter::write
			*/
			virtual void write(ParticleScriptSerializer* serializer, const IElement* element);
	};

}
#endif

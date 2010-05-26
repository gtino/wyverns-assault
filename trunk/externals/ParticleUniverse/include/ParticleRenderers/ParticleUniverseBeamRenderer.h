/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2010 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#ifndef __PU_BEAM_RENDERER_H__
#define __PU_BEAM_RENDERER_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseRenderer.h"
#include "ParticleUniverseTechnique.h"
#include "ParticleUniverseTechniqueListener.h"
#include "OgreBillboardChain.h"

namespace ParticleUniverse
{
	/** Visual data specific for this type of renderer.
    */
	class _ParticleUniverseExport BeamRendererVisualData : public IVisualData
	{
		public:
			BeamRendererVisualData (size_t index) : 
				IVisualData(), 
				chainIndex(index),
				mTimeSinceLastUpdate(0.0f){};

			// Index of the chain
			virtual void setVisible(bool visible)
			{
				// Todo
			}

			// Index of the chain
			size_t chainIndex;
			Ogre::Vector3 half[100];
			Ogre::Vector3 destinationHalf[100];
			Ogre::Real mTimeSinceLastUpdate;
	};

	/** The BeamRenderer class is responsible to render particles as a BillboardChain.
    */
	class _ParticleUniverseExport BeamRenderer : public ParticleRenderer, public TechniqueListener
	{
		public:

			// Constants
			static const bool DEFAULT_USE_VERTEX_COLOURS;
			static const size_t DEFAULT_MAX_ELEMENTS;
			static const Ogre::Real DEFAULT_UPDATE_INTERVAL;
			static const Ogre::Real DEFAULT_DEVIATION;
			static const size_t DEFAULT_NUMBER_OF_SEGMENTS;
			static const Ogre::BillboardChain::TexCoordDirection DEFAULT_TEXTURE_DIRECTION;

			BeamRenderer(void);
	        virtual ~BeamRenderer(void);

			/** Getters and Setters
			*/
			bool isUseVertexColours(void) const;
			void setUseVertexColours(bool useVertexColours);

			size_t getMaxChainElements(void) const;
			void setMaxChainElements(size_t maxChainElements);

			Ogre::Real getUpdateInterval(void) const;
			void setUpdateInterval(Ogre::Real updateInterval);

			Ogre::Real getDeviation(void) const;
			void setDeviation(Ogre::Real deviation);

			size_t getNumberOfSegments(void) const;
			void setNumberOfSegments(size_t numberOfSegments);

			bool isJump(void) const;
			void setJump(bool jump);

			Ogre::BillboardChain::TexCoordDirection getTexCoordDirection(void) const;
			void setTexCoordDirection(Ogre::BillboardChain::TexCoordDirection texCoordDirection);

			/** @copydoc ParticleRenderer::_prepare */
			virtual void _prepare(ParticleTechnique* technique);

			/** @copydoc ParticleRenderer::_unprepare */
			virtual void _unprepare(ParticleTechnique* technique);

			/** Destroys the BillboarChain
			*/
			void _destroyAll(void);

			/** 
			*/
			inline virtual void _updateRenderQueue(Ogre::RenderQueue* queue, ParticlePool* pool);

			/** See ParticleRenderer
			*/
			inline virtual void _processParticle(ParticleTechnique* particleTechnique, 
				Particle* particle, 
				Ogre::Real timeElapsed, 
				bool firstParticle);

			/** 
			*/
			virtual void _notifyAttached(Ogre::Node* parent, bool isTagPoint = false){/* No implementation here */};

			/** @copydoc ParticleRenderer::_setMaterialName */
			virtual void _setMaterialName(const Ogre::String& materialName);

			/** 
			*/
			virtual void _notifyCurrentCamera(Ogre::Camera* cam);
	
			/** 
			*/
			virtual void _notifyParticleQuota(size_t quota);

			/** 
			*/
			virtual void _notifyDefaultDimensions(Ogre::Real width, Ogre::Real height, Ogre::Real depth){/* No implementation */};

			/** 
			*/
			virtual void _notifyParticleResized(void){/* No implementation */};

			/** 
			*/
			virtual void _notifyParticleZRotated(void){/* No implementation */};

			/** 
			*/
			virtual void setRenderQueueGroup(Ogre::uint8 queueId);

			/** @copydoc ParticleRenderer::_getSortMode */
			virtual Ogre::SortMode _getSortMode(void) const;

			/** @copydoc ParticleRenderer::setVisible */
			virtual void setVisible(bool visible);

			/** 
			*/
			virtual void copyAttributesTo (ParticleRenderer* renderer);

			/*  See TechniqueListener.
			*/
			virtual void particleEmitted(ParticleTechnique* particleTechnique, Particle* particle);

			/*  See TechniqueListener.
			*/
			virtual void particleExpired(ParticleTechnique* particleTechnique, Particle* particle);

		protected:

			Ogre::String mBillboardChainName;
			Ogre::BillboardChain* mBillboardChain;
			Ogre::vector<BeamRendererVisualData*>::type mAllVisualData;
			Ogre::vector<BeamRendererVisualData*>::type mVisualData;
			size_t mQuota;
			bool mUseVertexColours;
			size_t mMaxChainElements;
			Ogre::Real mUpdateInterval;
			Ogre::Real mDeviation;
			size_t mNumberOfSegments;
			bool mJump;
			Ogre::BillboardChain::TexCoordDirection mTexCoordDirection;
	};

}
#endif

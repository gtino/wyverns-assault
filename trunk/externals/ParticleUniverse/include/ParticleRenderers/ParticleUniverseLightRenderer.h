/*
-----------------------------------------------------------------------------------------------
This source file is part of the Particle Universe product.

Copyright (c) 2010 Henry van Merode

Usage of this program is licensed under the terms of the Particle Universe Commercial License.
You can find a copy of the Commercial License in the Particle Universe package.
-----------------------------------------------------------------------------------------------
*/

#ifndef __PU_LIGHT_RENDERER_H__
#define __PU_LIGHT_RENDERER_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseRenderer.h"
#include "ParticleUniverseTechnique.h"
#include "OgreSceneNode.h"
#include "OgreLight.h"

namespace ParticleUniverse
{
	/** Visual data specific for this type of renderer.
    */
	class _ParticleUniverseExport LightRendererVisualData : public IVisualData
	{
		public:
			LightRendererVisualData (Ogre::SceneNode* sceneNode) : 
				IVisualData(), 
				node(sceneNode), 
				light(0),
				flashFrequencyCount(0.0f),
				flashLengthCount(0.0f){};
			Ogre::SceneNode* node;
			Ogre::Light* light;
			Ogre::Real flashFrequencyCount;
			Ogre::Real flashLengthCount;
			virtual void setVisible(bool visible)
			{
				if (node)
				{
					node->setVisible(visible);
				}
			};
	};

	/** The LightRenderer class is responsible to render particles as a light.
	@remarks
		Note, that the diffuse colour cannot be set. This is, because the light inherits its diffuse colour from the particle. This makes
		it possible to manipulate the colour (for instance, using a Colour Affector).
    */
	class _ParticleUniverseExport LightRenderer : public ParticleRenderer
	{

		protected:
			Ogre::String mLightName; // Used for random light name prefix
			Ogre::vector<LightRendererVisualData*>::type mAllVisualData;
			Ogre::vector<LightRendererVisualData*>::type mVisualData;
			Ogre::vector<Ogre::Light*>::type mLights;
			size_t mQuota;
			Ogre::Light::LightTypes mLightType;
			Ogre::ColourValue mSpecularColour;
			Ogre::Real mAttenuationRange;
			Ogre::Real mAttenuationConstant;
			Ogre::Real mAttenuationLinear;
			Ogre::Real mAttenuationQuadratic;
			Ogre::Radian mSpotlightInnerAngle;
			Ogre::Radian mSpotlightOuterAngle;
			Ogre::Real mSpotlightFalloff;
			Ogre::Real mPowerScale;
			Ogre::Real mFlashFrequency;
			Ogre::Real mFlashLength;
			bool mFlashRandom;

			/** Make all nodes to which the entities are attached visible or invisible.
			*/
			void _makeNodesVisible(bool visible);

		public:
			// Constants
			static const Ogre::Light::LightTypes DEFAULT_LIGHT_TYPE;
			static const Ogre::ColourValue DEFAULT_DIFFUSE;
			static const Ogre::ColourValue DEFAULT_SPECULAR;
			static const Ogre::Real DEFAULT_ATT_RANGE;
			static const Ogre::Real DEFAULT_ATT_CONSTANT;
			static const Ogre::Real DEFAULT_ATT_LINEAR;
			static const Ogre::Real DEFAULT_ATT_QUADRATIC;
			static const Ogre::Radian DEFAULT_SPOT_INNER_ANGLE;
			static const Ogre::Radian DEFAULT_SPOT_OUTER_ANGLE;
			static const Ogre::Real DEFAULT_FALLOFF;
			static const Ogre::Real DEFAULT_POWER_SCALE;

			LightRenderer(void);
	        virtual ~LightRenderer(void);

			/** Return the type of light that is emitted.
			*/
			Ogre::Light::LightTypes getLightType(void) const;

			/** Set the type of light that is emitted.
			*/
			void setLightType(Ogre::Light::LightTypes lightType);

			/** 
			*/
			const Ogre::ColourValue& getSpecularColour(void) const;
			void setSpecularColour(const Ogre::ColourValue& specularColour);

			/** 
			*/
			Ogre::Real getAttenuationRange(void) const;
			void setAttenuationRange(Ogre::Real attenuationRange);

			/** 
			*/
			Ogre::Real getAttenuationConstant(void) const;
			void setAttenuationConstant(Ogre::Real attenuationConstant);

			/** 
			*/
			Ogre::Real getAttenuationLinear(void) const;
			void setAttenuationLinear(Ogre::Real attenuationLinear);

			/** 
			*/
			Ogre::Real getAttenuationQuadratic(void) const;
			void setAttenuationQuadratic(Ogre::Real attenuationQuadratic);

			/** 
			*/
			const Ogre::Radian& getSpotlightInnerAngle(void) const;
			void setSpotlightInnerAngle(const Ogre::Radian& spotlightInnerAngle);

			/** 
			*/
			const Ogre::Radian& getSpotlightOuterAngle(void) const;
			void setSpotlightOuterAngle(const Ogre::Radian& spotlightOuterAngle);

			/** 
			*/
			Ogre::Real getSpotlightFalloff(void) const;
			void setSpotlightFalloff(Ogre::Real spotlightFalloff);

			/** 
			*/
			Ogre::Real getPowerScale(void) const;
			void setPowerScale(Ogre::Real powerScale);

			/** 
			*/
			Ogre::Real getFlashFrequency(void) const;
			void setFlashFrequency(Ogre::Real flashFrequency);

			/** 
			*/
			Ogre::Real getFlashLength(void) const;
			void setFlashLength(Ogre::Real flashLength);

			/** 
			*/
			bool isFlashRandom(void) const;
			void setFlashRandom(bool flashRandom);

			/** Deletes all ChildSceneNodes en Lights.
			*/
			void _destroyAll(void);

			/** @copydoc ParticleRenderer::setVisible */
			//virtual void setVisible(bool visible = true);

			/** @copydoc ParticleRenderer::_prepare */
			virtual void _prepare(ParticleTechnique* technique);

			/** @copydoc ParticleRenderer::_unprepare */
			virtual void _unprepare(ParticleTechnique* technique);

			/** @copydoc ParticleRenderer::_updateRenderQueue */
			//inline virtual void _updateRenderQueue(Ogre::RenderQueue* queue, ParticlePool* pool);

			/** @copydoc ParticleRenderer::_processParticle */
			inline virtual void _processParticle(ParticleTechnique* particleTechnique, 
				Particle* particle, 
				Ogre::Real timeElapsed, 
				bool firstParticle);

			/** @copydoc ParticleRenderer::_setMaterialName */
			virtual void _setMaterialName(const Ogre::String& materialName);

			/** @copydoc ParticleRenderer::_notifyCurrentCamera */
			virtual void _notifyCurrentCamera(Ogre::Camera* cam);
	
			/** @copydoc ParticleRenderer::_notifyAttached */
			virtual void _notifyAttached(Ogre::Node* parent, bool isTagPoint = false);

			/** @copydoc ParticleRenderer::_notifyParticleQuota */
			virtual void _notifyParticleQuota(size_t quota);

			/** @copydoc ParticleRenderer::_notifyDefaultDimensions */
			virtual void _notifyDefaultDimensions(Ogre::Real width, Ogre::Real height, Ogre::Real depth);

			/** @copydoc ParticleRenderer::_notifyParticleResized */
			virtual void _notifyParticleResized(void);

			/** @copydoc ParticleRenderer::_getSortMode */
			virtual Ogre::SortMode _getSortMode(void) const;

			/** @copydoc ParticleRenderer::copyAttributesTo */
			virtual void copyAttributesTo (ParticleRenderer* renderer);
	};

}
#endif

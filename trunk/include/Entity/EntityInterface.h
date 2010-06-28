/*
-----------------------------------------------------------------------------
Wyverns' Assault 3D Videgame.
Copyright (C) 2010  Giorgio Tino, Javier Soto Huesa, Jordi Carreras Ribot, 
					Marc Serena, Elm Oliver Torres

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

-----------------------------------------------------------------------------
*/
#ifndef __ENTITY_INTERFACE_H__
#define __ENTITY_INTERFACE_H__

#define UP_RIGHT_DIRECTION Ogre::Vector3(1,0,-1)
#define DOWN_RIGHT_DIRECTION Ogre::Vector3(1,0,1)
#define RIGHT_DIRECTION Ogre::Vector3(1,0,0)
#define UP_LEFT_DIRECTION Ogre::Vector3(-1,0,-1)
#define DOWN_LEFT_DIRECTION Ogre::Vector3(-1,0,1)
#define LEFT_DIRECTION Ogre::Vector3(-1,0,0)
#define UP_DIRECTION Ogre::Vector3(0,0,-1)
#define DOWN_DIRECTION Ogre::Vector3(0,0,1)


#include <Ogre.h>

#include "..\Utils\Utils.h"
#include "..\GUI\GuiObjectTextDisplay.h"

namespace WyvernsAssault
{
	/**
	Interface implemented by objects with graphics/animations
	*/
	class EntityInterface
	{
	public:
		EntityInterface(Ogre::String name);
		virtual ~EntityInterface() = 0;

	public:
		virtual void initializeEntity(Ogre::Entity* entity, Ogre::SceneNode* sceneNode, Ogre::SceneManager* sceneManager) = 0;
		virtual void finalizeEntity();

		virtual Ogre::SceneNode* _getSceneNode() const { return mSceneNode; }
		
		virtual const Ogre::String& getName() { return mName; }

		virtual void setPosition(const Ogre::Vector3 position) { mSceneNode->setPosition(position); }
		virtual Ogre::Vector3 getPosition() { return mSceneNode->getPosition(); } 
		virtual Ogre::Vector3 getInitialPosition() { return mSceneNode->getInitialPosition(); } 

		virtual Ogre::Vector3 getDerivedPosition() {return mSceneNode->_getDerivedPosition();}

		virtual void setScale(const Ogre::Vector3 scale) { mSceneNode->setScale(scale); }
		virtual Ogre::Vector3 getScale(){return mSceneNode->getScale(); }

		virtual void setDirection(const Ogre::Vector3 direction) { mDirection = direction; }
		virtual Ogre::Vector3 getDirection(){return mDirection; }

		virtual void setOrientation(const Ogre::Quaternion orientation) { mSceneNode->setOrientation(orientation); }
		virtual Ogre::Quaternion getOrientation(){return mSceneNode->getOrientation(); }

		virtual void setMaterialName(const Ogre::String materialName) { mEntity->setMaterialName(materialName);}

		virtual void setVisible(bool visible){ mEntity->setVisible(visible); }
		virtual void isVisible(){ mEntity->isVisible(); }

		virtual Ogre::Entity* getEntity(){return mEntity; }

	protected:
		void _destroy(SceneNode* sceneNode);

	protected:
		Ogre::Entity* mEntity;
		Ogre::Entity* mPhysicEntity;
		Ogre::SceneNode* mSceneNode;
		Ogre::SceneManager* mSceneManager;
		Ogre::AnimationState* mAnimationState;
		Ogre::String mName;
		Ogre::Vector3 mDirection;
	};

	typedef boost::shared_ptr<EntityInterface> EntityInterfacePtr;
}

#endif // __ENTITY_INTERFACE_H__
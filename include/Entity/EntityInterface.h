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

#include <Ogre.h>

#include "..\GUI\GuiObjectTextDisplay.h"
#include "..\Camera\CameraManager.h"

namespace WyvernsAssault
{
	/**
	Interface implemented by objects with graphics/animations
	*/
	class EntityInterface
	{
	public:
		virtual void initializeEntity(Ogre::Entity* mesh, Ogre::SceneNode* sceneNode)
		{
			mSceneNode = sceneNode;
			mMesh = mesh;
	
			// Animations
			//mAnimationState = mMesh->getAnimationState("Iddle_01");
			//mAnimationState->setEnabled(true);
			//mAnimationState->setLoop(true);

			// DEBUG TEXT DISPLAY
			/*mGuiObjectTextDisplay = new GuiObjectTextDisplay(mesh, CameraManager::getSingleton().getCamera());
			mGuiObjectTextDisplay->enable(true);
			mGuiObjectTextDisplay->setText(getName());*/
		}

		virtual void finalizeEntity(){/*delete mGuiObjectTextDisplay;*/}

		virtual void updateEntity(const float elapsedSeconds){/*mGuiObjectTextDisplay->update();*/};

		virtual Ogre::SceneNode* getSceneNode() const { return mSceneNode; }
		virtual void setSceneNode(Ogre::SceneNode* sceneNode) { mSceneNode = sceneNode; }

		virtual const Ogre::String& getName() { return mMesh->getName(); }

		virtual void setPosition(const Ogre::Vector3 position) { mSceneNode->setPosition(position); }
		virtual Ogre::Vector3 getPosition() { return mSceneNode->getPosition(); } 

		virtual void setScale(const Ogre::Vector3 scale) { mSceneNode->setScale(scale); }
		virtual Ogre::Vector3 getScale(){return mSceneNode->getScale(); }

		//virtual void lookAt(const Ogre::Vector3 lookAt) { mSceneNode->lookAt(lookAt,Ogre::TraTransformSpace::); }

		virtual void setMaterialName(const Ogre::String materialName) { mMesh->setMaterialName(materialName);}

	protected:
		Ogre::Entity* mMesh;
		Ogre::SceneNode* mSceneNode;
		Ogre::AnimationState* mAnimationState;

		/*GuiObjectTextDisplay* mGuiObjectTextDisplay;*/
	};
}

#endif // __ENTITY_INTERFACE_H__
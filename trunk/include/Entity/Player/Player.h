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
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <Ogre.h>
#include "OgreOde_Core.h"

// Player default movement speed
#define SPEED	5

typedef struct{
	OgreOde::RayGeometry* charRay;
	Ogre::Real radius;
	Ogre::Vector3 last_contact;
}ODE_CHAR_INFO;

using namespace Ogre;

namespace WyvernsAssault
{
	/**
	Class used to manage all the players
	*/
	class Player
	{
	public:
		Player(Ogre::String name);
		~Player();
	
		void initialize(Ogre::Entity* mesh, Ogre::SceneNode* sceneNode, Ogre::SceneNode* sceneNode_model,Ogre::SceneNode* autoTrackingNode);
		void finalize();

		//Node
		Ogre::SceneNode* getSceneNode() const { return mSceneNode; }
		void setSceneNode(Ogre::SceneNode* sceneNode) { mSceneNode = sceneNode; }

		//Model Node
		Ogre::SceneNode* getSceneNodeModel() const { return mSceneNode_model; }
		void setSceneNodeModel(Ogre::SceneNode* sceneNodeModel) { mSceneNode_model = sceneNodeModel; }

		const Ogre::String& getName() { return mSceneNode->getName(); }

		void setPosition(Ogre::Vector3 position);
		Ogre::Vector3 getPosition() { return mSceneNode->getPosition(); } 

		void setScale(Ogre::Vector3 scale) { mSceneNode->setScale(scale); }
		Ogre::Vector3 getScale(){return mSceneNode->getScale(); }

		void setUpdated(bool update) { ray_updated = update; }
		bool getUpdated(){ return ray_updated; }

		void setTorso(OgreOde::Body* body){torso = body;}
		OgreOde::Body* getTorso(){ return torso; }

		void setRay(OgreOde::RayGeometry* ray_geom, Real radius) { 
			ray.charRay = ray_geom;
			ray.radius = radius;
			ray.last_contact = Vector3(0,0,0);
		};
		ODE_CHAR_INFO getRay(){return ray; }

		// Player movement functions
		void move(Real x, Real y, Real z);

		// Animation functions
		void updateAnimation(float elapsedSeconds);

	private:
		Ogre::Entity* mMesh;
		Ogre::SceneNode* mSceneNode;
		Ogre::SceneNode* mSceneNode_model;
		Ogre::SceneNode* mAutoTrackingNode;
		Ogre::AnimationState* mAnimationState;
		
		//Physic
		ODE_CHAR_INFO ray;
		OgreOde::Body* torso;
		bool ray_updated;

	};

	typedef boost::shared_ptr<Player> PlayerPtr;
}

#endif // __PLAYER_H__
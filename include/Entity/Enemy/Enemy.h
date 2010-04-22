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
#ifndef __ENEMY_H__
#define __ENEMY_H__

#include <Ogre.h>

namespace WyvernsAssault
{
	/**
	Class used to manage all the enemies
	*/
	class Enemy
	{
	public:
		Enemy(Ogre::String name);
		~Enemy();
	
		void initialize(Ogre::Entity* mesh, Ogre::SceneNode* sceneNode);
		void finalize();

		Ogre::SceneNode* getSceneNode() const { return mSceneNode; }
		void setSceneNode(Ogre::SceneNode* sceneNode) { mSceneNode = sceneNode; }

		const Ogre::String& getName() { return mSceneNode->getName(); }

		void setPosition(Ogre::Vector3 position) { mSceneNode->setPosition(position); }
		Ogre::Vector3 getPosition() { return mSceneNode->getPosition(); } 

		void setScale(Ogre::Vector3 scale) { mSceneNode->setScale(scale); }
		Ogre::Vector3 getScale(){return mSceneNode->getScale(); }

	private:
		Ogre::Entity* mMesh;
		Ogre::SceneNode* mSceneNode;
	};

	typedef boost::shared_ptr<Enemy> EnemyPtr;
}

#endif // __ENEMY_H__